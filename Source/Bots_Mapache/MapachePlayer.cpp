#include "MapachePlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperFlipbookComponent.h"

AMapachePlayer::AMapachePlayer()
{
	// Sin Tick: todo se mueve por eventos de input. Mas liviano.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(25.f, 30.f);

	// El sprite no rota con el personaje; la direccion se muestra cambiando de Flipbook.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// El Mesh 3D heredado de ACharacter no se usa (el mapache es un sprite).
	GetMesh()->SetVisibility(false);
	GetMesh()->SetHiddenInGame(true);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetUsingAbsoluteRotation(true);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	Sprite->SetUsingAbsoluteRotation(true);
	Sprite->SetGenerateOverlapEvents(false);
	Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ApplyViewSettings();
}

void AMapachePlayer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ApplyViewSettings();
}

void AMapachePlayer::ApplyViewSettings()
{
	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, 0.f));
	CameraBoom->TargetArmLength = ArmLength;
	FollowCamera->SetFieldOfView(CameraFOV);

	// El sprite mira de frente a la camara (su cara visible apunta a -Y, de ahi el -90)
	// y se inclina un poco hacia ella.
	Sprite->SetRelativeRotation(FRotator(0.f, CameraYaw - 90.f, SpriteTilt));
}

void AMapachePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMapachePlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMapachePlayer::MoveRight);
}

// El movimiento es relativo a la camara: W siempre va "hacia arriba" en pantalla.
void AMapachePlayer::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		const FRotationMatrix YawMatrix(FRotator(0.f, CameraYaw, 0.f));
		AddMovementInput(YawMatrix.GetUnitAxis(EAxis::X), Value);
	}
}

void AMapachePlayer::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		const FRotationMatrix YawMatrix(FRotator(0.f, CameraYaw, 0.f));
		AddMovementInput(YawMatrix.GetUnitAxis(EAxis::Y), Value);
	}
}
