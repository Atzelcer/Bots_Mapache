#include "MapacheW_Carga.h"
#include "MapacheHUD.h"
#include "Components/AudioComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundWave.h"
#include "TimerManager.h"

void UMapacheW_Carga::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true;

	CargarAssetsPorDefecto();
	ReconstruirFrames();
	SetProgreso(0.f);
}

void UMapacheW_Carga::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CargaTimerHandle);
	}

	DetenerAudio();
	Super::NativeDestruct();
}

void UMapacheW_Carga::CargarAssetsPorDefecto()
{
	static const TCHAR* FramePaths[] = {
		TEXT("/Game/UI/fondos/1c.1c"),
		TEXT("/Game/UI/fondos/2c.2c"),
		TEXT("/Game/UI/fondos/3c.3c"),
		TEXT("/Game/UI/fondos/4c.4c"),
		TEXT("/Game/UI/fondos/5c.5c")
	};

	UTexture2D** FrameSlots[] = {
		&Frame_1c, &Frame_2c, &Frame_3c, &Frame_4c, &Frame_5c
	};

	for (int32 i = 0; i < 5; ++i)
	{
		if (!(*FrameSlots[i]))
		{
			*FrameSlots[i] = LoadObject<UTexture2D>(nullptr, FramePaths[i]);
		}
	}

	if (!AudioAmbiente)
	{
		AudioAmbiente = LoadObject<USoundWave>(nullptr, TEXT("/Game/Audio/Carga/Carga_Ambiente.Carga_Ambiente"));
	}

	if (!AudioMagia)
	{
		AudioMagia = LoadObject<USoundWave>(nullptr, TEXT("/Game/Audio/Carga/Carga_Magia.Carga_Magia"));
	}

	if (USoundWave* AmbienteWave = Cast<USoundWave>(AudioAmbiente))
	{
		AmbienteWave->bLooping = true;
	}
}

void UMapacheW_Carga::ReconstruirFrames()
{
	Frames.Reset();
	Frames.Add(Frame_1c);
	Frames.Add(Frame_2c);
	Frames.Add(Frame_3c);
	Frames.Add(Frame_4c);
	Frames.Add(Frame_5c);
}

void UMapacheW_Carga::IniciarCarga()
{
	if (bCargaActiva)
	{
		return;
	}

	CargarAssetsPorDefecto();
	ReconstruirFrames();

	bCargaActiva = true;
	IndiceFrame = 0;
	SetProgreso(0.f);
	ActualizarFrame();
	IniciarAudio();

	UWorld* World = GetWorld();
	if (!World || Frames.Num() == 0)
	{
		FinalizarCarga();
		return;
	}

	const float Paso = DuracionCarga / static_cast<float>(Frames.Num());
	World->GetTimerManager().SetTimer(
		CargaTimerHandle,
		this,
		&UMapacheW_Carga::ActualizarFrame,
		Paso,
		true,
		Paso);
}

void UMapacheW_Carga::ActualizarFrame()
{
	if (!bCargaActiva)
	{
		return;
	}

	if (Frames.IsValidIndex(IndiceFrame) && barraCarga && Frames[IndiceFrame])
	{
		barraCarga->SetBrushFromTexture(Frames[IndiceFrame], true);
	}

	const float Denominador = FMath::Max(1.f, static_cast<float>(Frames.Num() - 1));
	SetProgreso(static_cast<float>(IndiceFrame) / Denominador);

	++IndiceFrame;

	if (IndiceFrame >= Frames.Num())
	{
		SetProgreso(1.f);

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(CargaTimerHandle);
			const float Paso = DuracionCarga / static_cast<float>(FMath::Max(1, Frames.Num()));
			World->GetTimerManager().SetTimer(
				CargaTimerHandle,
				this,
				&UMapacheW_Carga::FinalizarCarga,
				Paso,
				false);
		}
		else
		{
			FinalizarCarga();
		}
	}
}

void UMapacheW_Carga::SetProgreso(float Percent)
{
	Progreso = FMath::Clamp(Percent, 0.f, 1.f);
}

void UMapacheW_Carga::SetEstado(const FText& NuevoEstado)
{
	if (TxtEstado)
	{
		TxtEstado->SetText(NuevoEstado);
	}
}

void UMapacheW_Carga::IniciarAudio()
{
	DetenerAudio();

	if (AudioAmbiente)
	{
		AmbienteAudioComponent = UGameplayStatics::SpawnSound2D(
			this,
			AudioAmbiente,
			VolumenAmbiente,
			1.f,
			0.f,
			nullptr,
			false,
			true);

		if (AmbienteAudioComponent)
		{
			AmbienteAudioComponent->bIsUISound = true;
			AmbienteAudioComponent->bAllowSpatialization = false;
		}
	}

	if (AudioMagia)
	{
		MagiaAudioComponent = UGameplayStatics::SpawnSound2D(
			this,
			AudioMagia,
			VolumenMagia,
			1.f,
			0.f,
			nullptr,
			false,
			true);

		if (MagiaAudioComponent)
		{
			MagiaAudioComponent->bIsUISound = true;
			MagiaAudioComponent->bAllowSpatialization = false;
		}
	}
}

void UMapacheW_Carga::DetenerAudio()
{
	if (AmbienteAudioComponent)
	{
		AmbienteAudioComponent->Stop();
		AmbienteAudioComponent = nullptr;
	}

	if (MagiaAudioComponent)
	{
		MagiaAudioComponent->Stop();
		MagiaAudioComponent = nullptr;
	}
}

void UMapacheW_Carga::FinalizarCarga()
{
	bCargaActiva = false;
	DetenerAudio();

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	AMapacheHUD* HUD = Cast<AMapacheHUD>(PC->GetHUD());
	if (HUD)
	{
		HUD->HideCarga();
		HUD->ShowEnJuego();
	}
}
