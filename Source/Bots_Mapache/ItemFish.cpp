// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFish.h"

AItemFish::AItemFish()
{
	InitConfig(TEXT("/Game/libraryAll/Meshes/SM_Sack_Var01.SM_Sack_Var01"));
}

void AItemFish::Interact(AMapachePlayer * MapachePlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("Interactuando con: %s"), *Name.ToString());
}

