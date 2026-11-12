#include "Bots_MapacheGameModeBase.h"
#include "MapachePlayer.h"

ABots_MapacheGameModeBase::ABots_MapacheGameModeBase()
{
	DefaultPawnClass = AMapachePlayer::StaticClass();
}
