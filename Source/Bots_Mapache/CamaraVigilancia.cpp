#include "CamaraVigilancia.h"

#include "Components/SceneComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	constexpr float SweepEaseExponent = 2.f;
	constexpr float InnerConeRatio = 0.6f;
	const TCHAR* CameraMeshPath = TEXT("/Game/Props/Scene/Camara_Rotatoria.Camara_Rotatoria");
}

ACamaraVigilancia::ACamaraVigilancia()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	Pivot->SetupAttachment(Root);

	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraMesh"));
	CameraMesh->SetupAttachment(Pivot);
	CameraMesh->SetMobility(EComponentMobility::Movable);
	CameraMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(CameraMeshPath);
	if (MeshFinder.Succeeded())
	{
		CameraMesh->SetStaticMesh(MeshFinder.Object);
	}

	Spot = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spot"));
	Spot->SetupAttachment(Pivot);
	Spot->SetMobility(EComponentMobility::Movable);
	Spot->SetCastShadows(false);

	ApplySettings();
}

void ACamaraVigilancia::BeginPlay()
{
	Super::BeginPlay();

	ApplySettings();
	Spot->SetVisibility(false);

	UpdateActivation();
	GetWorldTimerManager().SetTimer(ActivationTimer, this, &ACamaraVigilancia::UpdateActivation, ActivationCheckInterval, true);
}

void ACamaraVigilancia::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ApplySettings();
}

void ACamaraVigilancia::ApplySettings()
{
	Pivot->SetRelativeRotation(FRotator::ZeroRotator);

	CameraMesh->SetRelativeRotation(FRotator(0.f, MeshYawOffset, 0.f));
	CameraMesh->SetRelativeScale3D(FVector(MeshScale));

	Spot->SetRelativeLocation(LightOffset);
	Spot->SetRelativeRotation(FRotator(ConePitch, 0.f, 0.f));
	Spot->SetOuterConeAngle(ConeAngle);
	Spot->SetInnerConeAngle(ConeAngle * InnerConeRatio);
	Spot->SetAttenuationRadius(ConeDistance);
	Spot->SetIntensity(LightIntensity);
	Spot->SetLightColor(bDetected ? AlertColor : NormalColor);
}

void ACamaraVigilancia::UpdateActivation()
{
	if (!Target.IsValid())
	{
		Target = UGameplayStatics::GetPlayerPawn(this, 0);
	}

	const APawn* Pawn = Target.Get();
	const bool bNear = Pawn && FVector::DistSquared(Pawn->GetActorLocation(), GetActorLocation()) <= FMath::Square(ActivationDistance);
	SetActive(bNear);
}

void ACamaraVigilancia::SetActive(bool bNewActive)
{
	if (bNewActive == bActive)
	{
		return;
	}

	bActive = bNewActive;
	Spot->SetVisibility(bActive);

	FTimerManager& Timers = GetWorldTimerManager();
	if (bActive)
	{
		UpdateSweep();
		Timers.SetTimer(SweepTimer, this, &ACamaraVigilancia::UpdateSweep, SweepUpdateInterval, true);
		Timers.SetTimer(DetectionTimer, this, &ACamaraVigilancia::CheckDetection, DetectionInterval, true);
	}
	else
	{
		Timers.ClearTimer(SweepTimer);
		Timers.ClearTimer(DetectionTimer);
		SetDetected(false, Target.Get());
	}
}

void ACamaraVigilancia::UpdateSweep()
{
	const float Travel = SweepAngle / SweepSpeed;
	const float Cycle = 2.f * (Travel + PauseAtEnds);
	if (Cycle <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	const float Half = SweepAngle * 0.5f;
	const float Time = FMath::Fmod(GetWorld()->GetTimeSeconds() + StartOffset, Cycle);

	float Yaw = -Half;
	if (Time < Travel)
	{
		Yaw = FMath::InterpEaseInOut(-Half, Half, Time / Travel, SweepEaseExponent);
	}
	else if (Time < Travel + PauseAtEnds)
	{
		Yaw = Half;
	}
	else if (Time < 2.f * Travel + PauseAtEnds)
	{
		Yaw = FMath::InterpEaseInOut(Half, -Half, (Time - Travel - PauseAtEnds) / Travel, SweepEaseExponent);
	}

	Pivot->SetRelativeRotation(FRotator(0.f, Yaw, 0.f));
}

void ACamaraVigilancia::CheckDetection()
{
	APawn* Pawn = Target.Get();
	if (!Pawn)
	{
		return;
	}

	SetDetected(CanSee(*Pawn), Pawn);
}

bool ACamaraVigilancia::CanSee(const AActor& Other) const
{
	const FVector Origin = Spot->GetComponentLocation();
	const FVector TargetLocation = Other.GetActorLocation();
	const FVector ToTarget = TargetLocation - Origin;

	const float DistanceSquared = ToTarget.SizeSquared();
	if (DistanceSquared > FMath::Square(ConeDistance) || DistanceSquared < KINDA_SMALL_NUMBER)
	{
		return false;
	}

	const float CosAngle = FVector::DotProduct(Spot->GetForwardVector(), ToTarget.GetSafeNormal());
	if (CosAngle < FMath::Cos(FMath::DegreesToRadians(ConeAngle)))
	{
		return false;
	}

	FCollisionQueryParams Params(TEXT("CamaraVigilancia"), false, this);
	Params.AddIgnoredActor(&Other);
	return !GetWorld()->LineTraceTestByChannel(Origin, TargetLocation, ECC_Visibility, Params);
}

void ACamaraVigilancia::SetDetected(bool bNewDetected, AActor* Other)
{
	if (bNewDetected == bDetected)
	{
		return;
	}

	bDetected = bNewDetected;
	Spot->SetLightColor(bDetected ? AlertColor : NormalColor);

	if (bDetected)
	{
		OnTargetDetected.Broadcast(Other);
	}
	else
	{
		OnTargetLost.Broadcast(Other);
	}
}
