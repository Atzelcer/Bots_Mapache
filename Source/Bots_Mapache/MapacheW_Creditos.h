#pragma once

#include "CoreMinimal.h"
#include "MapacheUserWidget.h"
#include "MapacheW_Creditos.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;

UCLASS(Blueprintable, BlueprintType)
class BOTS_MAPACHE_API UMapacheW_Creditos : public UMapacheUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI|Creditos")
	void Saltar();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "UI")
	UVerticalBox* CajaCreditos = nullptr;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "UI")
	UTextBlock* TxtTitulo = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnSaltar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Creditos", meta = (ClampMin = "10"))
	float Velocidad = 80.f;

	UFUNCTION()
	void OnSaltarClicked();

	void EnlazarPorNombre();
	void LlenarCreditos();
	void EstilizarLinea(UTextBlock* Linea, int32 Tamano, const FLinearColor& Color) const;
	UTextBlock* AgregarLinea(const FString& Texto, int32 Tamano, const FLinearColor& Color, float PaddingArriba, float PaddingAbajo);

	float PosY = 0.f;
	float AltoVista = 720.f;
	bool bDesplazamientoListo = false;
};
