#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class BOTS_MAPACHE_API IInteractable
{
	GENERATED_BODY()

public:
	virtual bool CanInteract(const AActor* Interactor) const = 0;
	virtual FText GetInteractionPrompt() const = 0;
	virtual float GetInteractionDuration() const = 0;
	virtual void Interact(AActor* Interactor) = 0;
};
