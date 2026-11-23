#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Container_Bas.generated.h"

class USphereComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FContainerSearchedSignature, AActor*, Searcher);

UCLASS()
class BOTS_MAPACHE_API AContainer_Bas : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AContainer_Bas();

	UPROPERTY(BlueprintAssignable, Category = "Basurero")
	FContainerSearchedSignature OnSearched;

	virtual bool CanInteract(const AActor* Interactor) const override;
	virtual FText GetInteractionPrompt() const override;
	virtual float GetInteractionDuration() const override;
	virtual void Interact(AActor* Interactor) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basurero", meta = (ClampMin = "0"))
	float SearchDuration = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basurero", meta = (ClampMin = "1"))
	int32 MaxSearches = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basurero")
	FText Prompt = NSLOCTEXT("Basurero", "Prompt", "Hurgar");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basurero", meta = (ClampMin = "10"))
	float InteractionRadius = 150.f;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basurero", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basurero", meta = (AllowPrivateAccess = "true"))
	USphereComponent* InteractionZone = nullptr;

	int32 SearchesLeft = 0;
};
