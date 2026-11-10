// Jugador: el mapache. Camara isometrica fija + sprite 2D (Paper2D) en mundo 3D.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MapachePlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UPaperFlipbookComponent;

UCLASS()
class BOTS_MAPACHE_API AMapachePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AMapachePlayer();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	// Se llama al construir el actor y al cambiar propiedades en el editor.
	virtual void OnConstruction(const FTransform& Transform) override;

	// --- Camara (editables desde un Blueprint hijo o en la instancia del nivel) ---

	/** Inclinacion de la camara. Negativo = mira hacia abajo. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camara")
	float CameraPitch = -50.f;

	/** Giro de la camara alrededor del mapache (0, -45, -90...). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camara")
	float CameraYaw = -45.f;

	/** Distancia de la camara al mapache. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camara", meta = (ClampMin = "100"))
	float ArmLength = 1200.f;

	/** Campo de vision. Bajo = menos deformacion (mas isometrico). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camara", meta = (ClampMin = "5", ClampMax = "120"))
	float CameraFOV = 35.f;

	// --- Sprite ---

	/** Inclinacion del sprite hacia la camara para que no se vea aplastado. Si se inclina al reves, usa valor negativo. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite")
	float SpriteTilt = 20.f;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	/** Aplica los valores de camara y sprite a los componentes. */
	void ApplyViewSettings();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camara", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* Sprite;
};
