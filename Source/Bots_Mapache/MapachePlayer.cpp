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

	ApplyViewSettings();
}

void AMapachePlayer::BeginPlay()
{
	Super::BeginPlay();

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
