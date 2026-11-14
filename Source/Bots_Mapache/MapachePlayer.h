#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MapachePlayer.generated.h"

class UAnimSequence;
class UCameraComponent;
class UMaterialParameterCollection;
class USpringArmComponent;
class UProgressBar;
class UMapacheW_EnJuego;

UCLASS()
class BOTS_MAPACHE_API AMapachePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AMapachePlayer();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void BindEnJuegoUI(UMapacheW_EnJuego* Widget);

	UFUNCTION(BlueprintCallable, Category = "UI|Energia")
	void SetEnergia(float Percent);

	UFUNCTION(BlueprintCallable, Category = "UI|Energia")
	float GetEnergia() const { return Energia; }

	UFUNCTION(BlueprintCallable, Category = "UI|Energia")
	UProgressBar* GetBarEnergia() const { return BarEnergia; }

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UMapacheW_EnJuego* EnJuegoWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "UI|Energia")
	UProgressBar* BarEnergia = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Energia", meta = (ClampMin = "0", ClampMax = "1"))
	float Energia = 1.f;

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camara")
	float CameraPitch = -60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camara")
	float CameraYaw = -45.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camara", meta = (ClampMin = "100"))
	float ArmLength = 2400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camara", meta = (ClampMin = "5", ClampMax = "120"))
	float CameraFOV = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modelo")
	float MeshYawOffset = -90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modelo", meta = (ClampMin = "0.01"))
	float MeshScale = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modelo")
	float MeshHeightOffset = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modelo", meta = (ClampMin = "0.1", ClampMax = "1.5"))
	float CapsuleFit = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movimiento", meta = (ClampMin = "10"))
	float WalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash", meta = (ClampMin = "10"))
	float DashSpeed = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash", meta = (ClampMin = "0.05"))
	float DashDuration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash", meta = (ClampMin = "0"))
	float DashCooldown = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animacion", meta = (ClampMin = "0.01"))
	float AnimationUpdateInterval = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Oclusion")
	TSoftObjectPtr<UMaterialParameterCollection> OcclusionCollection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Oclusion", meta = (ClampMin = "0"))
	float HoleRadius = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Oclusion", meta = (ClampMin = "0.01"))
	float OcclusionUpdateInterval = 0.05f;

private:
	enum class EAnimState : uint8
	{
		None,
		Idle,
		Walk,
		Dash
	};

	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveRelativeToCamera(EAxis::Type Axis, float Value);

	void StartDash();
	void EndDash();

	void ApplyViewSettings();
	void StartAnimation();
	void UpdateAnimation();
	void SetAnimState(EAnimState NewState);
	void PlayClip(UAnimSequence* Clip, bool bLoop, float Rate);

	void UpdateOcclusion();
	FVector GetCameraWorldLocation() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera = nullptr;

	UPROPERTY()
	UAnimSequence* IdleAnimation = nullptr;

	UPROPERTY()
	UAnimSequence* WalkAnimation = nullptr;

	UPROPERTY()
	UAnimSequence* DashAnimation = nullptr;

	UPROPERTY(Transient)
	UMaterialParameterCollection* LoadedOcclusionCollection = nullptr;

	FTimerHandle AnimationTimer;
	FTimerHandle DashTimer;
	FTimerHandle OcclusionTimer;
	FVector LastOcclusionPos = FVector::ZeroVector;
	EAnimState AnimState = EAnimState::None;
	float NextDashTime = 0.f;
	bool bHasLastOcclusionPos = false;
	bool bDashing = false;
};
