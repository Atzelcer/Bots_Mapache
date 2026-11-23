#include "MapachePlayer.h"
#include "MapacheHUD.h"
#include "MapacheW_EnJuego.h"

#include "Animation/AnimSequence.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ProgressBar.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/RootMotionSource.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputCoreTypes.h"
#include "Interactable.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "TimerManager.h"
#include "InventoryComponent.h"
#include "GameFramework/PlayerController.h"
#include "InventoryWidget.h"
#include "CurrentInventoryWidget.h"
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
	const TCHAR* IdleAnimationPath = TEXT("/Game/Raccoon/Mapache_Idle_solo_animacion.Mapache_Idle_solo_animacion");
	const TCHAR* WalkAnimationPath = TEXT("/Game/Raccoon/Mapache_Caminar_Anim.Mapache_Caminar_Anim");
	const TCHAR* RunAnimationPath = TEXT("/Game/Raccoon/Mapache_Correr_solo_animacion.Mapache_Correr_solo_animacion");
	const TCHAR* DashAnimationPath = TEXT("/Game/Raccoon/Mapache_Dash_solo_animacion.Mapache_Dash_solo_animacion");
	const TCHAR* DigAnimationPath = TEXT("/Game/Raccoon/Mapache_Escarbar_solo_animacion.Mapache_Escarbar_solo_animacion");
	const TCHAR* WidgetInventoryPath = TEXT("/Game/UI/WBP_Inventory.WBP_Inventory_C");
	const TCHAR* WidgetCurrentInventoryPath = TEXT("/Game/UI/WBP_CurrentInventory.WBP_CurrentInventory_C");
}

AMapachePlayer::AMapachePlayer()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMapachePlayer::OnObjectBeginOverlap);

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

	static ConstructorHelpers::FObjectFinder<UAnimSequence> IdleFinder(IdleAnimationPath);
	if (IdleFinder.Succeeded())
	{
		IdleAnimation = IdleFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> WalkFinder(WalkAnimationPath);
	if (WalkFinder.Succeeded())
	{
		WalkAnimation = WalkFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> RunFinder(RunAnimationPath);
	if (RunFinder.Succeeded())
	{
		RunAnimation = RunFinder.Object;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgeBP(WidgetInventoryPath);
	if (WidgeBP.Class) {
		InventoryWidgetClass = WidgeBP.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgeCurrentBP(WidgetCurrentInventoryPath);
	if (WidgeCurrentBP.Class) {
		CurrentInventoryWidgetClass = WidgeCurrentBP.Class;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> DashFinder(DashAnimationPath);
	if (DashFinder.Succeeded())
	{
		DashAnimation = DashFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> DigFinder(DigAnimationPath);
	if (DigFinder.Succeeded())
	{
		DigAnimation = DigFinder.Object;
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetCollisionObjectType(ECC_Pawn);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	InteractionSphere->SetGenerateOverlapEvents(true);
	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMapachePlayer::OnInteractionBegin);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AMapachePlayer::OnInteractionEnd);

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
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
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

	if (InventoryWidgetClass) {
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		if (InventoryWidget) {
			InventoryWidget->AddToViewport(1);
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (CurrentInventoryWidgetClass) {
		CurrentInventoryWidget = CreateWidget<UCurrentInventoryWidget>(GetWorld(), CurrentInventoryWidgetClass);
		if (CurrentInventoryWidget) {
			CurrentInventoryWidget->AddToViewport(0);
			CurrentInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AMapachePlayer::BindEnJuegoUI(UMapacheW_EnJuego* Widget)
{
	EnJuegoWidget = Widget;
	BarEnergia = Widget ? Widget->GetBarEnergia() : nullptr;

	if (Widget)
	{
		bAtrapado = false;
	}
}

void AMapachePlayer::SetEnergia(float Percent)
{
	Energia = FMath::Clamp(Percent, 0.f, 1.f);

	if (EnJuegoWidget)
	{
		EnJuegoWidget->SetEnergia(Energia);
	}
	else if (BarEnergia)
	{
		BarEnergia->SetPercent(Energia);
	}
}

void AMapachePlayer::OnAtrapado()
{
	if (bAtrapado)
	{
		return;
	}

	bAtrapado = true;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}

	AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->ShowGameOver();
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
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMapachePlayer::TryInteract);
	PlayerInputComponent->BindKey(EKeys::LeftShift, IE_Pressed, this, &AMapachePlayer::StartRun);
	PlayerInputComponent->BindKey(EKeys::LeftShift, IE_Released, this, &AMapachePlayer::StopRun);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AMapachePlayer::ToogleInventory);
	PlayerInputComponent->BindAction("DoSomething", IE_Pressed, this, &AMapachePlayer::DoSomething);
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

void AMapachePlayer::StartRun()
{
	bRunning = true;
	ApplySpeed();
	UpdateAnimation();
}

void AMapachePlayer::StopRun()
{
	bRunning = false;
	ApplySpeed();
	UpdateAnimation();
}

void AMapachePlayer::ApplySpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = bRunning ? RunSpeed : WalkSpeed;
}

void AMapachePlayer::StartDash()
{
	UWorld* World = GetWorld();
	if (bDashing || bInteracting || !World || World->GetTimeSeconds() < NextDashTime)
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

	UCharacterMovementComponent* Movement = GetCharacterMovement();

	TSharedPtr<FRootMotionSource_ConstantForce> DashForce = MakeShared<FRootMotionSource_ConstantForce>();
	DashForce->InstanceName = TEXT("MapacheDash");
	DashForce->AccumulateMode = ERootMotionAccumulateMode::Override;
	DashForce->Priority = DashPriority;
	DashForce->Force = Direction * DashSpeed;
	DashForce->Duration = DashDuration;
	DashForce->FinishVelocityParams.Mode = ERootMotionFinishVelocityMode::ClampVelocity;
	DashForce->FinishVelocityParams.ClampVelocity = Movement->MaxWalkSpeed;
	Movement->ApplyRootMotionSource(DashForce);

	SetAnimState(EAnimState::Dash);
	GetWorldTimerManager().SetTimer(DashTimer, this, &AMapachePlayer::EndDash, DashDuration, false);
}

void AMapachePlayer::EndDash()
{
	bDashing = false;
	AnimState = EAnimState::None;
	UpdateAnimation();
}

void AMapachePlayer::TryInteract()
{
	if (bInteracting || bDashing)
	{
		return;
	}

	AActor* Best = nullptr;
	float BestDistance = TNumericLimits<float>::Max();
	for (const TWeakObjectPtr<AActor>& Candidate : NearbyInteractables)
	{
		AActor* Actor = Candidate.Get();
		const IInteractable* Interactable = Cast<IInteractable>(Actor);
		if (!Interactable || !Interactable->CanInteract(this))
		{
			continue;
		}

		const float Distance = FVector::DistSquared(Actor->GetActorLocation(), GetActorLocation());
		if (Distance < BestDistance)
		{
			BestDistance = Distance;
			Best = Actor;
		}
	}

	if (!Best)
	{
		return;
	}

	IInteractable* Interactable = Cast<IInteractable>(Best);
	const float Duration = Interactable->GetInteractionDuration();
	if (Duration <= 0.f)
	{
		Interactable->Interact(this);
		return;
	}

	bInteracting = true;
	InteractionTarget = Best;

	GetCharacterMovement()->StopMovementImmediately();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->SetIgnoreMoveInput(true);
	}

	FVector ToTarget = Best->GetActorLocation() - GetActorLocation();
	ToTarget.Z = 0.f;
	if (ToTarget.Normalize())
	{
		SetActorRotation(ToTarget.Rotation());
	}

	SetAnimState(EAnimState::Dig);
	GetWorldTimerManager().SetTimer(InteractionTimer, this, &AMapachePlayer::FinishInteraction, Duration, false);
}

void AMapachePlayer::FinishInteraction()
{
	bInteracting = false;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->SetIgnoreMoveInput(false);
	}

	if (IInteractable* Interactable = Cast<IInteractable>(InteractionTarget.Get()))
	{
		Interactable->Interact(this);
	}
	InteractionTarget.Reset();

	AnimState = EAnimState::None;
	UpdateAnimation();
}

void AMapachePlayer::OnInteractionBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && Cast<IInteractable>(OtherActor))
	{
		NearbyInteractables.AddUnique(OtherActor);
	}
}

void AMapachePlayer::OnInteractionEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	NearbyInteractables.Remove(OtherActor);
}

void AMapachePlayer::ApplyViewSettings()
{
	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, 0.f));
	CameraBoom->TargetArmLength = ArmLength;
	FollowCamera->SetFieldOfView(CameraFOV);

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = bRunning ? RunSpeed : WalkSpeed;
	Movement->MaxStepHeight = MaxStepHeight;
	Movement->SetWalkableFloorAngle(MaxWalkableAngle);
	Movement->bUseFlatBaseForFloorChecks = true;

	InteractionSphere->SetSphereRadius(InteractionRadius);

	const FRotator MeshRotation(0.f, MeshYawOffset, 0.f);

	float Radius = CapsuleRadius;
	float HalfHeight = CapsuleHalfHeight;
	FVector MeshLocation(0.f, 0.f, -CapsuleHalfHeight + MeshHeightOffset);

	if (const USkeletalMesh* Model = GetMesh()->SkeletalMesh)
	{
		const FBoxSphereBounds Bounds = Model->GetBounds();
		const FVector Extent = Bounds.BoxExtent * MeshScale;

		HalfHeight = Extent.Z;
		Radius = FMath::Min(FMath::Max(Extent.X, Extent.Y) * CapsuleFit, HalfHeight);

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

	UpdateAnimation();
	GetWorldTimerManager().SetTimer(AnimationTimer, this, &AMapachePlayer::UpdateAnimation, AnimationUpdateInterval, true);
}

void AMapachePlayer::UpdateAnimation()
{
	if (bDashing || bInteracting)
	{
		return;
	}

	if (GetVelocity().SizeSquared2D() <= MovingSpeedSquared)
	{
		SetAnimState(EAnimState::Idle);
		return;
	}

	SetAnimState(bRunning ? EAnimState::Run : EAnimState::Walk);
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
	case EAnimState::Run:
		PlayClip(RunAnimation ? RunAnimation : WalkAnimation, true, 1.f);
		break;
	case EAnimState::Dash:
		PlayClip(DashAnimation, false, DashAnimation ? DashAnimation->GetPlayLength() / DashDuration : 1.f);
		break;
	case EAnimState::Dig:
		PlayClip(DigAnimation, true, 1.f);
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

void AMapachePlayer::ToogleInventory()
{
	APlayerController* PC = Cast<APlayerController>(GetController());

	if (!PC || !InventoryWidget) return;

	if (!bInventoryVisible)
	{
		OpenInventory(PC);
		return;
	}
	CloseInventory(PC);
}

void AMapachePlayer::ChoseInventory(int32 Location)
{
    UE_LOG(LogTemp, Warning, TEXT("Seleccionando %i"), Location);

    APlayerController* PC = Cast<APlayerController>(GetController());

    if (!PC || !InventoryWidget || !InventoryComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("Faltan referencias para seleccionar el item"));
        return;
    }

    CurrentItem = InventoryComponent->GetItemByIndex(Location);

    if (!CurrentItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("No se encontro un item en esa posicion"));
        return;
    }

    if (CurrentInventoryWidget)
    {
		ShowInventory();
    }

    CloseInventory(PC);
}

void AMapachePlayer::OpenInventory(APlayerController* PC)
{
	InventoryWidget->RefreshInventory(InventoryComponent);

	InventoryWidget->SetVisibility(ESlateVisibility::Visible);

	PC->bShowMouseCursor = true;

	PC->SetIgnoreMoveInput(true);
	PC->SetIgnoreLookInput(true);

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);

	PC->SetInputMode(InputMode);
	bInventoryVisible = true;
}

void AMapachePlayer::CloseInventory(APlayerController* PC)
{
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

	PC->bShowMouseCursor = false;

	PC->SetIgnoreMoveInput(false);
	PC->SetIgnoreLookInput(false);

	PC->SetInputMode(FInputModeGameOnly());
	bInventoryVisible = false;
}

void AMapachePlayer::ShowInventory()
{
	CurrentInventoryWidget->RefreshCurrentInventory(CurrentItem);
	CurrentInventoryWidget->SetVisibility(ESlateVisibility::Visible);
}

void AMapachePlayer::HideInventory()
{
	CurrentInventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AMapachePlayer::DoSomething()
{
	if (!CurrentItem)
	{
		return;
	}

	if (CurrentItem.GetInterface())
	{
		CurrentItem->Interact(this);
	}

	if (InventoryComponent)
	{
		InventoryComponent->RemoveItem(CurrentItem);
	}

	CurrentItem = TScriptInterface<IItemInterface>();
	ShowInventory();
}

int32 AMapachePlayer::GetMaxSlots()
{
	if (!InventoryComponent) {
		return 0;
	}
	return InventoryComponent->MaxSlots;
}

int32 AMapachePlayer::GetAvaibleInventory() {
	return GetMaxSlots() - InventoryComponent->Items.Num();
}

void AMapachePlayer::AddItemToInventory(TScriptInterface<IItemInterface> Item)
{
	if (!InventoryComponent || !Item) return;
	InventoryComponent->AddItem(Item);
}

void AMapachePlayer::OnObjectBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Detectando algo"));
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
