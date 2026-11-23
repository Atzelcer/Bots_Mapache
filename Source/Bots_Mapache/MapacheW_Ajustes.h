#pragma once

#include "CoreMinimal.h"
#include "MapacheUserWidget.h"
#include "MapacheW_Ajustes.generated.h"

class UButton;
class UProgressBar;
class USlider;
class UTexture2D;

UCLASS(Blueprintable, BlueprintType)
class BOTS_MAPACHE_API UMapacheW_Ajustes : public UMapacheUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI|Ajustes")
	void SetMuteado(bool bNuevoMute);

	UFUNCTION(BlueprintCallable, Category = "UI|Ajustes")
	bool EstaMuteado() const { return bMuteado; }

	UFUNCTION(BlueprintCallable, Category = "UI|Ajustes")
	void SetVolumen(float NuevoVolumen);

	UFUNCTION(BlueprintCallable, Category = "UI|Ajustes")
	float GetVolumen() const { return Volumen; }

	UFUNCTION(BlueprintCallable, Category = "UI|Ajustes")
	void SetBrillo(float NuevoBrillo);

	UFUNCTION(BlueprintCallable, Category = "UI|Ajustes")
	float GetBrillo() const { return Brillo; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Ajustes", meta = (ClampMin = "0", ClampMax = "1"))
	float Volumen = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Ajustes", meta = (ClampMin = "0", ClampMax = "1"))
	float Brillo = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Ajustes")
	bool bMuteado = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Ajustes|Mute")
	UTexture2D* TexMutearNOR = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Ajustes|Mute")
	UTexture2D* TexMutearHOV = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Ajustes|Mute")
	UTexture2D* TexMutearCLI = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Ajustes|Mute")
	UTexture2D* TexDesmutearNOR = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Ajustes|Mute")
	UTexture2D* TexDesmutearHOV = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Ajustes|Mute")
	UTexture2D* TexDesmutearCLI = nullptr;

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnMute = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnVolver = nullptr;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "UI")
	UProgressBar* BarBrillo = nullptr;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly, Category = "UI")
	UProgressBar* BarSonido = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	USlider* SliderBrillo = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	USlider* SliderSonido = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnBrilloMenos = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnBrilloMas = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnSonidoMenos = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnSonidoMas = nullptr;

	UFUNCTION()
	void OnMuteClicked();

	UFUNCTION()
	void OnVolverClicked();

	UFUNCTION()
	void OnBrilloChanged(float Value);

	UFUNCTION()
	void OnSonidoChanged(float Value);

	UFUNCTION()
	void OnBrilloMenos();

	UFUNCTION()
	void OnBrilloMas();

	UFUNCTION()
	void OnSonidoMenos();

	UFUNCTION()
	void OnSonidoMas();

	void CargarAssetsPorDefecto();
	void EnlazarPorNombre();
	void CargarAjustesGuardados();
	void GuardarAjustesActuales();
	void AplicarVolumen();
	void AplicarBrillo();
	void ActualizarBarras();
	void ActualizarBotonMute();
	void AplicarEstiloBotonMute(UTexture2D* NOR, UTexture2D* HOV, UTexture2D* CLI);
	bool AplicarDesdeCursor(UProgressBar* Barra, const FVector2D& ScreenPos, bool bEsBrillo);
	void SoltarArrastre();

	float VolumenAntesDeMute = 1.f;
	bool bArrastrandoSonido = false;
	bool bArrastrandoBrillo = false;
};
