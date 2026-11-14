#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PaperFlipbook.h"
#include "MapachePlayer.generated.h"

class UCameraComponent;
class UMaterialParameterCollection;
class UPaperFlipbookComponent;
class USpringArmComponent;

UENUM(BlueprintType, Category ="Animation")
enum class EAnimationDirection : uint8 
{
	Up,
	Down,
	Left,
	Rigth,
	UpperLeft,
	UpperRigth,
	DownLeft,
	DownRigth
};

USTRUCT(BlueprintType, Category = "Animation")
struct FAnimationFlipbooks
{
	GENERATED_BODY()

	//IDLE
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleUp{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleDown{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleLeft{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleRigth{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleUpperLeft{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleUpperRigth{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleDownLeft{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* IdleDownRigth{ nullptr };

	//Walk
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* WalkUp{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* WalkDown{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* WalkLeft{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* WalkRigth{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* WalkUpperLeft{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* WalkUpperRigth{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* WalkDownLeft{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* WalkDownRigth{ nullptr };
};

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
	void ZoomCamera(float Value);
	void MoveRelativeToCamera(EAxis::Type Axis, float Value);

	void ApplyViewSettings();
	void UpdateOcclusion();
	FVector GetCameraWorldLocation() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* Sprite = nullptr;

	UPROPERTY(Transient)
	UMaterialParameterCollection* LoadedOcclusionCollection = nullptr;

	FTimerHandle OcclusionTimer;
	FVector LastOcclusionPos = FVector::ZeroVector;
	bool bHasLastOcclusionPos = false;
public:
	UFUNCTION(BlueprintCallable, Category = "AnimationCharacter|Animation")
	void SetCurrentAnimationDirection(FVector const& Velocity);

	UFUNCTION(BlueprintCallable, Category = "AnimationCharacter|Animation")
	void Animate(float Deltatime, FVector OldLocation, FVector const OldVelocity);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AnimationCharacter|Animation")
	EAnimationDirection CurrentAnimationDirection{ EAnimationDirection::Down };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "AnimationCharacter|Animation")
	FAnimationFlipbooks Flipbooks;

	bool bIsMoving;
};
