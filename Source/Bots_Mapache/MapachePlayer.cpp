#include "MapachePlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "PaperFlipbookComponent.h"
#include "TimerManager.h"

namespace
{
	constexpr float CapsuleRadius = 25.f;
	constexpr float CapsuleHalfHeight = 30.f;
	constexpr float SpriteFacingOffsetYaw = -90.f;
	constexpr float MovedToleranceSquared = 1.f;
	const TCHAR* OcclusionCollectionPath = TEXT("/Game/Materials/MPC_Occlusion.MPC_Occlusion");
}

AMapachePlayer::AMapachePlayer()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
	GetMesh()->SetVisibility(false);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	Sprite->SetUsingAbsoluteRotation(true);
	Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OcclusionCollection = TSoftObjectPtr<UMaterialParameterCollection>(FSoftObjectPath(OcclusionCollectionPath));

	
	bIsMoving = false;
	ApplyViewSettings();
}

void AMapachePlayer::BeginPlay()
{
	Super::BeginPlay();

	Sprite->SetVisibility(true);
	Sprite->SetHiddenInGame(false);

	if (Sprite && Flipbooks.IdleDown)
	{
		Sprite->SetFlipbook(Flipbooks.IdleDown);
		Sprite->PlayFromStart();
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("Sprite o IdleDown no esta asignado"));
	}

	OnCharacterMovementUpdated.AddDynamic(this, &AMapachePlayer::Animate);
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
	PlayerInputComponent->BindAxis("ZoomCamera", this, &AMapachePlayer::ZoomCamera);
}

void AMapachePlayer::MoveForward(float Value)
{
	MoveRelativeToCamera(EAxis::X, Value);
}

void AMapachePlayer::MoveRight(float Value)
{
	MoveRelativeToCamera(EAxis::Y, Value);
}

void AMapachePlayer::ZoomCamera(float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}

	CameraBoom->TargetArmLength -= Value * 100.f;

	CameraBoom->TargetArmLength = FMath::Clamp(
		CameraBoom->TargetArmLength,
		500.f,
		2400.f
	);
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

void AMapachePlayer::SetCurrentAnimationDirection(FVector const& Velocity)
{
	const float x = Velocity.GetSafeNormal().X;
	const float y = Velocity.GetSafeNormal().Y;

	bIsMoving = x != 0.0f || y != 0.0f;

	if (bIsMoving) {
		if (y > 0.0f && (abs(x) < 0.5f || x == 0.0)) {
			CurrentAnimationDirection = EAnimationDirection::Down;
		}
		else if(y > 0.5f && x < -0.5f) {
			CurrentAnimationDirection = EAnimationDirection::DownRigth;
		}
		else if (y > 0.5f && x < -0.5f) {
			CurrentAnimationDirection = EAnimationDirection::DownLeft;
		}
		else if (y < 0.5f && abs(x) > 0.5f) {
			CurrentAnimationDirection = EAnimationDirection::Up;
		}
		else if (y < -0.5f && x > 0.5f) {
			CurrentAnimationDirection = EAnimationDirection::UpperRigth;
		}
		else if (y < -0.5f && x < -0.5f) {
			CurrentAnimationDirection = EAnimationDirection::UpperLeft;
		}
		else if (abs(y) < 0.5f && x < 0.0f) {
			CurrentAnimationDirection = EAnimationDirection::Rigth;
		}
		else 
		{
			CurrentAnimationDirection = EAnimationDirection::Left;
		}
	}
}

void AMapachePlayer::Animate(float Deltatime, FVector OldLocation, FVector const OldVelocity)
{
	SetCurrentAnimationDirection(OldVelocity);

	if (OldVelocity.Size() > 0.0f) {
		switch (CurrentAnimationDirection) {
		case EAnimationDirection::Up: 
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.WalkUp);
			}
			break;
		case EAnimationDirection::Down: 
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.WalkDown);
			}
			break;
		case EAnimationDirection::Left: 
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.WalkLeft);
			}
			break;
		case EAnimationDirection::Rigth: 
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.WalkRigth);
			}
			break;
		case EAnimationDirection::UpperLeft: 
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.WalkUpperLeft);
			}
			break;
		case EAnimationDirection::UpperRigth: 
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.WalkUpperRigth);
			}
			break;
		case EAnimationDirection::DownLeft: 
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.WalkDownLeft);
			}
			break;
		case EAnimationDirection::DownRigth: 
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.WalkDownRigth);
			}
			break;
		default:;
		}
	}
	else {
		switch (CurrentAnimationDirection) {
		case EAnimationDirection::Up:
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.IdleUp);
			}
			break;
		case EAnimationDirection::Down:
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.IdleDown);
			}
			break;
		case EAnimationDirection::Left:
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.IdleLeft);
			}
			break;
		case EAnimationDirection::Rigth:
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.IdleRigth);
			}
			break;
		case EAnimationDirection::UpperLeft:
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.IdleUpperLeft);
			}
			break;
		case EAnimationDirection::UpperRigth:
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.IdleUpperRigth);
			}
			break;
		case EAnimationDirection::DownLeft:
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.IdleDownLeft);
			}
			break;
		case EAnimationDirection::DownRigth:
			if (Sprite)
			{
				Sprite->SetFlipbook(Flipbooks.IdleDownRigth);
			}
			break;
		default:;
		}
	}
}

void AMapachePlayer::ApplyViewSettings()
{
	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, 0.f));
	CameraBoom->TargetArmLength = ArmLength;
	FollowCamera->SetFieldOfView(CameraFOV);

	Sprite->SetRelativeRotation(FRotator(0.f, CameraYaw + SpriteFacingOffsetYaw, SpriteTilt));
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
