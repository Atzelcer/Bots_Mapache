#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CamaraVigilancia.generated.h"

class APawn;
class USceneComponent;
class USpotLightComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCamaraTargetSignature, AActor*, Target);

UCLASS()
class BOTS_MAPACHE_API ACamaraVigilancia : public AActor
{
	GENERATED_BODY()

public:
	ACamaraVigilancia();

	UPROPERTY(BlueprintAssignable, Category = "Camara")
	FCamaraTargetSignature OnTargetDetected;

	UPROPERTY(BlueprintAssignable, Category = "Camara")
	FCamaraTargetSignature OnTargetLost;

	UFUNCTION(BlueprintPure, Category = "Camara")
	bool IsTargetDetected() const { return bDetected; }

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modelo", meta = (ClampMin = "0.01"))
	float MeshScale = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modelo")
	float MeshYawOffset = -90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrulla", meta = (ClampMin = "0", ClampMax = "360"))
	float SweepAngle = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrulla", meta = (ClampMin = "1"))
	float SweepSpeed = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrulla", meta = (ClampMin = "0"))
	float PauseAtEnds = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrulla", meta = (ClampMin = "0"))
	float StartOffset = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrulla", meta = (ClampMin = "0.01"))
	float SweepUpdateInterval = 0.033f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foco", meta = (ClampMin = "1", ClampMax = "80"))
	float ConeAngle = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foco", meta = (ClampMin = "50"))
	float ConeDistance = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foco", meta = (ClampMin = "-89", ClampMax = "89"))
	float ConePitch = -35.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foco")
	FVector LightOffset = FVector(40.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foco", meta = (ClampMin = "0"))
	float LightIntensity = 20000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foco")
	FLinearColor NormalColor = FLinearColor(1.f, 0.85f, 0.5f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Foco")
	FLinearColor AlertColor = FLinearColor(1.f, 0.08f, 0.04f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Deteccion", meta = (ClampMin = "0.01"))
	float DetectionInterval = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Deteccion", meta = (ClampMin = "100"))
	float ActivationDistance = 4000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Deteccion", meta = (ClampMin = "0.05"))
	float ActivationCheckInterval = 0.5f;

private:
	void ApplySettings();
	void UpdateActivation();
	void SetActive(bool bNewActive);
	void UpdateSweep();
	void CheckDetection();
	bool CanSee(const AActor& Other) const;
	void SetDetected(bool bNewDetected, AActor* Other);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Pivot = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CameraMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* Spot = nullptr;

	TWeakObjectPtr<APawn> Target;

	FTimerHandle ActivationTimer;
	FTimerHandle SweepTimer;
	FTimerHandle DetectionTimer;
	bool bActive = false;
	bool bDetected = false;
};
