#include "MapachePlayer.h"

#include "Animation/AnimSequence.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	constexpr float CapsuleRadius = 25.f;
	constexpr float CapsuleHalfHeight = 30.f;
	constexpr float TurnRate = 720.f;
	constexpr float MovingSpeedSquared = 25.f;
	constexpr float MovedToleranceSquared = 1.f;
	constexpr int32 DashPriority = 5;
	const TCHAR* OcclusionCollectionPath = TEXT("/Game/Materials/MPC_Occlusion.MPC_Occlusion");
	const TCHAR* MeshPath = TEXT("/Game/Raccoon/Mapache_Caminar.Mapache_Caminar");
	const TCHAR* WalkAnimationPath = TEXT("/Game/Raccoon/Mapache_Caminar_Anim.Mapache_Caminar_Anim");
	const TCHAR* DashAnimationPath = TEXT("/Game/Raccoon/Mapache_Dash_solo_animacion.Mapache_Dash_solo_animacion");
	const TCHAR* IdleAnimationPath = TEXT("/Game/Raccoon/Mapache_Idle.Mapache_Idle");
}

AMapachePlayer::AMapachePlayer()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.f, TurnRate, 0.f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder(MeshPath);
	if (MeshFinder.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> WalkFinder(WalkAnimationPath);
	if (WalkFinder.Succeeded())
	{
		WalkAnimation = WalkFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> DashFinder(DashAnimationPath);
	if (DashFinder.Succeeded())
	{
		DashAnimation = DashFinder.Object;
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	OcclusionCollection = TSoftObjectPtr<UMaterialParameterCollection>(FSoftObjectPath(OcclusionCollectionPath));

	ApplyViewSettings();
}

void AMapachePlayer::BeginPlay()
{
	Super::BeginPlay();

	StartAnimation();

	if (OcclusionCollection.IsNull())
	{
		return;
	}

	LoadedOcclusionCollection = OcclusionCollection.LoadSynchronous();
	if (LoadedOcclusionCollection)
	{
		UpdateOcclusion();
		GetWorldTimerManager().SetTimer(OcclusionTimer, this, &AMapachePlayer::UpdateOcclusion, OcclusionUpdateInterval, true);
	}
}

void AMapachePlayer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ApplyViewSettings();
}

void AMapachePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMapachePlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMapachePlayer::MoveRight);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMapachePlayer::StartDash);
}

void AMapachePlayer::MoveForward(float Value)
{
	MoveRelativeToCamera(EAxis::X, Value);
}

void AMapachePlayer::MoveRight(float Value)
{
	MoveRelativeToCamera(EAxis::Y, Value);
}

void AMapachePlayer::MoveRelativeToCamera(EAxis::Type Axis, float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}

	const FRotationMatrix CameraYawMatrix(FRotator(0.f, CameraYaw, 0.f));
	AddMovementInput(CameraYawMatrix.GetUnitAxis(Axis), Value);
}

void AMapachePlayer::StartDash()
{
	UWorld* World = GetWorld();
	if (bDashing || !World || World->GetTimeSeconds() < NextDashTime)
	{
		return;
	}

	FVector Direction = GetLastMovementInputVector();
	Direction.Z = 0.f;
	if (!Direction.Normalize())
	{
		Direction = GetActorForwardVector();
		Direction.Z = 0.f;
		Direction.Normalize();
	}

	bDashing = true;
	NextDashTime = World->GetTimeSeconds() + DashDuration + DashCooldown;

	SetActorRotation(Direction.Rotation());

	TSharedPtr<FRootMotionSource_ConstantForce> DashForce = MakeShared<FRootMotionSource_ConstantForce>();
	DashForce->InstanceName = TEXT("MapacheDash");
	DashForce->AccumulateMode = ERootMotionAccumulateMode::Override;
	DashForce->Priority = DashPriority;
	DashForce->Force = Direction * DashSpeed;
	DashForce->Duration = DashDuration;
	DashForce->FinishVelocityParams.Mode = ERootMotionFinishVelocityMode::ClampVelocity;
	DashForce->FinishVelocityParams.ClampVelocity = WalkSpeed;
	GetCharacterMovement()->ApplyRootMotionSource(DashForce);

	SetAnimState(EAnimState::Dash);
	GetWorldTimerManager().SetTimer(DashTimer, this, &AMapachePlayer::EndDash, DashDuration, false);
}

void AMapachePlayer::EndDash()
{
	bDashing = false;
	AnimState = EAnimState::None;
	UpdateAnimation();
}

void AMapachePlayer::ApplyViewSettings()
{
	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, 0.f));
	CameraBoom->TargetArmLength = ArmLength;
	FollowCamera->SetFieldOfView(CameraFOV);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	const FRotator MeshRotation(0.f, MeshYawOffset, 0.f);

	float Radius = CapsuleRadius;
	float HalfHeight = CapsuleHalfHeight;
	FVector MeshLocation(0.f, 0.f, -CapsuleHalfHeight + MeshHeightOffset);

	if (const USkeletalMesh* Model = GetMesh()->SkeletalMesh)
	{
		const FBoxSphereBounds Bounds = Model->GetBounds();
		const FVector Extent = Bounds.BoxExtent * MeshScale;

		Radius = FMath::Max(Extent.X, Extent.Y) * CapsuleFit;
		HalfHeight = FMath::Max(Extent.Z, Radius);

		const FVector ScaledCenter = MeshRotation.RotateVector(FVector(Bounds.Origin.X, Bounds.Origin.Y, 0.f)) * MeshScale;
		const float ScaledBottom = (Bounds.Origin.Z - Bounds.BoxExtent.Z) * MeshScale;
		MeshLocation = FVector(-ScaledCenter.X, -ScaledCenter.Y, -HalfHeight - ScaledBottom + MeshHeightOffset);
	}

	GetCapsuleComponent()->SetCapsuleSize(Radius, HalfHeight);

	GetMesh()->SetRelativeLocation(MeshLocation);
	GetMesh()->SetRelativeRotation(MeshRotation);
	GetMesh()->SetRelativeScale3D(FVector(MeshScale));
}

void AMapachePlayer::StartAnimation()
{
	if (!WalkAnimation || !GetMesh()->SkeletalMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("MapachePlayer: no se pudo cargar el mesh o la animacion de caminar"));
		return;
	}

	IdleAnimation = LoadObject<UAnimSequence>(nullptr, IdleAnimationPath, nullptr, LOAD_NoWarn | LOAD_Quiet);

	UpdateAnimation();
	GetWorldTimerManager().SetTimer(AnimationTimer, this, &AMapachePlayer::UpdateAnimation, AnimationUpdateInterval, true);
}

void AMapachePlayer::UpdateAnimation()
{
	if (bDashing)
	{
		return;
	}

	const bool bMoving = GetVelocity().SizeSquared2D() > MovingSpeedSquared;
	SetAnimState(bMoving ? EAnimState::Walk : EAnimState::Idle);
}

void AMapachePlayer::SetAnimState(EAnimState NewState)
{
	if (NewState == AnimState)
	{
		return;
	}

	AnimState = NewState;

	switch (NewState)
	{
	case EAnimState::Walk:
		PlayClip(WalkAnimation, true, 1.f);
		break;
	case EAnimState::Dash:
		PlayClip(DashAnimation, false, DashAnimation ? DashAnimation->GetPlayLength() / DashDuration : 1.f);
		break;
	case EAnimState::Idle:
		if (IdleAnimation)
		{
			PlayClip(IdleAnimation, true, 1.f);
		}
		else
		{
			PlayClip(WalkAnimation, true, 1.f);
			GetMesh()->Stop();
		}
		break;
	default:
		break;
	}
}

void AMapachePlayer::PlayClip(UAnimSequence* Clip, bool bLoop, float Rate)
{
	if (!Clip)
	{
		return;
	}

	GetMesh()->PlayAnimation(Clip, bLoop);
	GetMesh()->SetPlayRate(Rate);
}

FVector AMapachePlayer::GetCameraWorldLocation() const
{
	return CameraBoom->GetComponentLocation() - FRotator(CameraPitch, CameraYaw, 0.f).Vector() * ArmLength;
}

void AMapachePlayer::UpdateOcclusion()
{
	const FVector PlayerPos = GetActorLocation();
	if (bHasLastOcclusionPos && FVector::DistSquared(PlayerPos, LastOcclusionPos) < MovedToleranceSquared)
	{
		return;
	}

	bHasLastOcclusionPos = true;
	LastOcclusionPos = PlayerPos;

	const FVector CameraPos = GetCameraWorldLocation();

	UKismetMaterialLibrary::SetVectorParameterValue(this, LoadedOcclusionCollection, TEXT("PlayerPos"), FLinearColor(PlayerPos));
	UKismetMaterialLibrary::SetVectorParameterValue(this, LoadedOcclusionCollection, TEXT("CameraPos"), FLinearColor(CameraPos));
	UKismetMaterialLibrary::SetScalarParameterValue(this, LoadedOcclusionCollection, TEXT("HoleRadius"), HoleRadius);
}
