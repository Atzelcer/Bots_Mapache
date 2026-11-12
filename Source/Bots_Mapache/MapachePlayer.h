#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MapachePlayer.generated.h"

class UCameraComponent;
class UMaterialParameterCollection;
class UPaperFlipbookComponent;
class USpringArmComponent;

UCLASS()
class BOTS_MAPACHE_API AMapachePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AMapachePlayer();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite")
	float SpriteTilt = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Oclusion")
	TSoftObjectPtr<UMaterialParameterCollection> OcclusionCollection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Oclusion", meta = (ClampMin = "0"))
	float HoleRadius = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Oclusion", meta = (ClampMin = "0.01"))
	float OcclusionUpdateInterval = 0.05f;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveRelativeToCamera(EAxis::Type Axis, float Value);

	void ApplyViewSettings();
	void UpdateOcclusion();
	FVector GetCameraWorldLocation() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* Sprite = nullptr;

	UPROPERTY(Transient)
	UMaterialParameterCollection* LoadedOcclusionCollection = nullptr;

	FTimerHandle OcclusionTimer;
	FVector LastOcclusionPos = FVector::ZeroVector;
	bool bHasLastOcclusionPos = false;
};
