#include "MapacheW_Historia.h"
#include "MapacheHUD.h"
#include "Components/AudioComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundWave.h"

void UMapacheW_Historia::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true;

	if (BtnSiguiente)
	{
		BtnSiguiente->OnClicked.AddDynamic(this, &UMapacheW_Historia::OnSiguienteClicked);
	}

	if (BtnAnterior)
	{
		BtnAnterior->OnClicked.AddDynamic(this, &UMapacheW_Historia::OnAnteriorClicked);
	}

	if (BtnSaltar)
	{
		BtnSaltar->OnClicked.AddDynamic(this, &UMapacheW_Historia::OnSaltarClicked);
	}

	CargarDiapositivasPorDefecto();
	CargarAudioPorDefecto();
	ReconstruirLista();

	if (IndicesConComunicacion.Num() == 0)
	{
		IndicesConComunicacion = { 0, 1, 2 };
	}

	ReiniciarHistoria();
	IniciarMusicaFondo();
}

void UMapacheW_Historia::NativeDestruct()
{
	DetenerMusicaFondo();
	Super::NativeDestruct();
}

void UMapacheW_Historia::CargarDiapositivasPorDefecto()
{
	static const TCHAR* Paths[] = {
		TEXT("/Game/UI/Historia/Historia_01.Historia_01"),
		TEXT("/Game/UI/Historia/Historia_02.Historia_02"),
		TEXT("/Game/UI/Historia/Historia_03.Historia_03"),
		TEXT("/Game/UI/Historia/Historia_04.Historia_04"),
		TEXT("/Game/UI/Historia/Historia_05.Historia_05"),
		TEXT("/Game/UI/Historia/Historia_06.Historia_06"),
		TEXT("/Game/UI/Historia/Historia_07.Historia_07")
	};

	UTexture2D** Slots[] = {
		&Historia_01, &Historia_02, &Historia_03, &Historia_04,
		&Historia_05, &Historia_06, &Historia_07
	};

	for (int32 i = 0; i < 7; ++i)
	{
		if (!(*Slots[i]))
		{
			*Slots[i] = LoadObject<UTexture2D>(nullptr, Paths[i]);
		}
	}
}

void UMapacheW_Historia::CargarAudioPorDefecto()
{
	if (!MusicaFondo)
	{
		MusicaFondo = LoadObject<USoundWave>(nullptr, TEXT("/Game/Audio/Historia/Historia_BGM.Historia_BGM"));
	}

	if (!SfxComunicacion)
	{
		SfxComunicacion = LoadObject<USoundWave>(nullptr, TEXT("/Game/Audio/Historia/Mapache_Comunicacion.Mapache_Comunicacion"));
	}

	if (USoundWave* Wave = Cast<USoundWave>(MusicaFondo))
	{
		Wave->bLooping = true;
	}
}

void UMapacheW_Historia::ReconstruirLista()
{
	Diapositivas.Reset();
	Diapositivas.Add(Historia_01);
	Diapositivas.Add(Historia_02);
	Diapositivas.Add(Historia_03);
	Diapositivas.Add(Historia_04);
	Diapositivas.Add(Historia_05);
	Diapositivas.Add(Historia_06);
	Diapositivas.Add(Historia_07);
}

int32 UMapacheW_Historia::GetNumDiapositivas() const
{
	return Diapositivas.Num();
}

UTexture2D* UMapacheW_Historia::GetDiapositiva(int32 Indice) const
{
	if (!Diapositivas.IsValidIndex(Indice))
	{
		return nullptr;
	}

	return Diapositivas[Indice];
}

void UMapacheW_Historia::ReiniciarHistoria()
{
	MostrarDiapositiva(0, false);
}

void UMapacheW_Historia::Siguiente()
{
	if (Diapositivas.Num() == 0)
	{
		IrAlJuego();
		return;
	}

	if (IndiceActual >= Diapositivas.Num() - 1)
	{
		IrAlJuego();
		return;
	}

	MostrarDiapositiva(IndiceActual + 1, true);
}

void UMapacheW_Historia::Anterior()
{
	if (IndiceActual <= 0)
	{
		IrAlPrincipal();
		return;
	}

	MostrarDiapositiva(IndiceActual - 1, true);
}

void UMapacheW_Historia::Saltar()
{
	IrAlJuego();
}

void UMapacheW_Historia::MostrarDiapositiva(int32 Indice, bool bPlaySfx)
{
	if (Diapositivas.Num() == 0)
	{
		return;
	}

	const int32 NuevoIndice = FMath::Clamp(Indice, 0, Diapositivas.Num() - 1);
	const bool bCambio = NuevoIndice != IndiceActual;
	IndiceActual = NuevoIndice;

	if (ImgHistoria)
	{
		if (UTexture2D* Texture = GetDiapositiva(IndiceActual))
		{
			ImgHistoria->SetBrushFromTexture(Texture, true);
		}
	}

	if (bPlaySfx && bCambio && DebeReproducirComunicacion(IndiceActual))
	{
		ReproducirComunicacion();
	}
}

void UMapacheW_Historia::IniciarMusicaFondo()
{
	DetenerMusicaFondo();

	if (!MusicaFondo)
	{
		return;
	}

	MusicaAudioComponent = UGameplayStatics::SpawnSound2D(
		this,
		MusicaFondo,
		1.f,
		1.f,
		0.f,
		nullptr,
		false,
		true);

	if (MusicaAudioComponent)
	{
		MusicaAudioComponent->bIsUISound = true;
		MusicaAudioComponent->bAllowSpatialization = false;
	}
}

void UMapacheW_Historia::DetenerMusicaFondo()
{
	if (MusicaAudioComponent)
	{
		MusicaAudioComponent->Stop();
		MusicaAudioComponent = nullptr;
	}
}

bool UMapacheW_Historia::DebeReproducirComunicacion(int32 Indice) const
{
	return IndicesConComunicacion.Contains(Indice);
}

void UMapacheW_Historia::ReproducirComunicacion()
{
	if (!SfxComunicacion)
	{
		return;
	}

	UGameplayStatics::PlaySound2D(this, SfxComunicacion, VolumenComunicacion);
}

void UMapacheW_Historia::IrAlJuego()
{
	DetenerMusicaFondo();

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->HideHistoria();
		HUD->ShowCarga();
	}
}

void UMapacheW_Historia::IrAlPrincipal()
{
	DetenerMusicaFondo();

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->HideHistoria();
		HUD->ShowPrincipal();
	}
}

void UMapacheW_Historia::OnSiguienteClicked()
{
	Siguiente();
}

void UMapacheW_Historia::OnAnteriorClicked()
{
	Anterior();
}

void UMapacheW_Historia::OnSaltarClicked()
{
	Saltar();
}
