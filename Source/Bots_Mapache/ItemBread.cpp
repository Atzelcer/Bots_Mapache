// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBread.h"

AItemBread::AItemBread()
{
	InitConfig(TEXT("/Game/libraryAll/Meshes/SM_Sack_Var01.SM_Sack_Var01"));
}

void AItemBread::Interact(AMapachePlayer * MapachePlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("Interactuando con: %s"), *Name.ToString());
}
