#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ItemInterface.h"
#include "Blueprint/UserWidget.h"
#include "MapachePlayer.generated.h"


class UAnimSequence;
class UCameraComponent;
class UMaterialParameterCollection;
class USpringArmComponent;
class UInventoryComponent;
class UInventoryWidget;
class UCurrentInventoryWidget;

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

public:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> CurrentInventoryWidgetClass;

	UPROPERTY()
	UCurrentInventoryWidget* CurrentInventoryWidget;

	bool bInventoryVisible = false;
	void ToogleInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ChoseInventory(int32 Location);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OpenInventory(APlayerController* PC);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseInventory(APlayerController* PC);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ShowInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void HideInventory();

	UPROPERTY()
	TScriptInterface<IItemInterface> CurrentItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float InventoryCursorSpeed = 0.5f;

	void DoSomething();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* InventoryComponent = nullptr;

	int32 GetMaxSlots();
	int32 GetAvaibleInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemToInventory(TScriptInterface<IItemInterface> Item);

	UFUNCTION()
	void OnObjectBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
