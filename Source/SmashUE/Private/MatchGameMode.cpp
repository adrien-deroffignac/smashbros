// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameMode.h"

#include "SmashCharacter.h"
#include "Arena/ArenaPlayerStart.h"
#include "Kismet/GameplayStatics.h"


void AMatchGameMode::FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*> &ResultsActors)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerStart::StaticClass(), FoundActors);

	for(int i = 0; i < FoundActors.Num(); i++)
	{
		 AArenaPlayerStart* ArenaPlayerStartActor = Cast<AArenaPlayerStart>(FoundActors[i]);
		if(ArenaPlayerStartActor == nullptr) continue;

		ResultsActors.Add(ArenaPlayerStartActor);
	}
}

TSubclassOf<ASmashCharacter> AMatchGameMode::GetSmashCharacterClassFromInputType(
	EAutoReceiveInput::Type inputType) const
{
	switch(inputType)
	{
		case EAutoReceiveInput::Player0:
			return CharacterClassP0;
		
		case EAutoReceiveInput::Player1:
			return CharacterClassP1;
		
		case EAutoReceiveInput::Player2:
			return CharacterClassP2;
		
		case EAutoReceiveInput::Player3:
			return CharacterClassP3;
		
		default:
			return nullptr;
	}
	
}


void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AArenaPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorsInArena(PlayerStartsPoints);

	for(AArenaPlayerStart* PlayerStartPoint : PlayerStartsPoints)
	{
		EAutoReceiveInput::Type inputType = PlayerStartPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> CharacterClass = GetSmashCharacterClassFromInputType(inputType);
		if(CharacterClass == nullptr) continue;
		
		GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,CharacterClass->GetFName().ToString());
	}
	
}
