#include "Container_Bas.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	const TCHAR* ContainerMeshPath = TEXT("/Game/Building/OpenContainer/TrashContainer_Open.TrashContainer_Open");
}

AContainer_Bas::AContainer_Bas()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(ContainerMeshPath);
	if (MeshFinder.Succeeded())
	{
		Mesh->SetStaticMesh(MeshFinder.Object);
	}

	InteractionZone = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionZone"));
	InteractionZone->SetupAttachment(Mesh);
	InteractionZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionZone->SetCollisionObjectType(ECC_WorldDynamic);
	InteractionZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractionZone->SetGenerateOverlapEvents(true);

	SearchesLeft = MaxSearches;
	InteractionZone->SetSphereRadius(InteractionRadius);
}

void AContainer_Bas::BeginPlay()
{
	Super::BeginPlay();

	SearchesLeft = MaxSearches;
}

void AContainer_Bas::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	InteractionZone->SetSphereRadius(InteractionRadius);
}

bool AContainer_Bas::CanInteract(const AActor* Interactor) const
{
	return SearchesLeft > 0;
}

FText AContainer_Bas::GetInteractionPrompt() const
{
	return Prompt;
}

float AContainer_Bas::GetInteractionDuration() const
{
	return SearchDuration;
}

void AContainer_Bas::Interact(AActor* Interactor)
{
	if (SearchesLeft <= 0)
	{
		return;
	}

	--SearchesLeft;
	OnSearched.Broadcast(Interactor);
}
