#include "Bots_MapacheGameModeBase.h"
#include "MapachePlayer.h"

ABots_MapacheGameModeBase::ABots_MapacheGameModeBase()
{
	//DefaultPawnClass = AMapachePlayer::StaticClass();
    static ConstructorHelpers::FClassFinder<AMapachePlayer> MapacheCharacter(
        TEXT("/Game/Blueprints/Characters/Mapache/MapachePlayer_BP")
    );

    if (MapacheCharacter.Succeeded())
    {
        DefaultPawnClass = MapacheCharacter.Class;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No se pudo cargar MapachePlayer_BP"));
    }

}
