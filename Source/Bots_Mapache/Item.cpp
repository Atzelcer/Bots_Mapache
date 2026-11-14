// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "MapachePlayer.h"
// Sets default values
namespace {
    const TCHAR* MeshPath = TEXT("/Game/UI/Item/orange/Orange.Orange");
}
AItem::AItem()
{

    UE_LOG(LogTemp, Warning, TEXT("ITEM INICIADO"));
  
    PrimaryActorTick.bCanEverTick = false;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));

    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);

    CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    CollisionBox->SetGenerateOverlapEvents(true);

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this,&AItem::OnItemOverlap);

    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    ItemMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(MeshPath);

    if (MeshFinder.Succeeded())
    {
        ItemMesh->SetStaticMesh(MeshFinder.Object);
    }

    ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (MeshAsset)
    {
        ItemMesh->SetStaticMesh(MeshAsset);
    }
}

void AItem::Interact(AMapachePlayer* MapachePlayer)
{
    UE_LOG(LogTemp, Warning, TEXT("Interactuando con: %s"), *Name.ToString());
}

void AItem::OnItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AMapachePlayer* Player = Cast<AMapachePlayer>(OtherActor))
    {
        if (this->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
        {
            TScriptInterface<IItemInterface> ItemInterface;

            ItemInterface.SetObject(this);
            ItemInterface.SetInterface(Cast<IItemInterface>(this));
            Player->AddItemToInventory(ItemInterface);
            this->Destroy();
        }
    }
}

