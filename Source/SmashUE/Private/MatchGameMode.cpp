// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGameMode.h"

#include "ArenaSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "States/SmashCharacter.h"
#include "Arena/ArenaPlayerStart.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


USmashCharacterInputData* AMatchGameMode::LoadInputDataFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if(CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputData.LoadSynchronous();
}

UInputMappingContext* AMatchGameMode::LoadInputMappingContextFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if(CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputMappingContext.LoadSynchronous();
}

void AMatchGameMode::CreateAndInitPlayers() const
{
	UGameInstance * GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) return;

	LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::InGame);
}

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

	const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();
	
	switch(inputType)
	{
		case EAutoReceiveInput::Player0:
			return ArenaSettings->SmashCharacterClassP0;
		
		case EAutoReceiveInput::Player1:
			return ArenaSettings->SmashCharacterClassP1;
		
		case EAutoReceiveInput::Player2:
			return ArenaSettings->SmashCharacterClassP2;
		
		case EAutoReceiveInput::Player3:
			return ArenaSettings->SmashCharacterClassP3;
		
		default:
			return nullptr;
	}
	
}

void AMatchGameMode::SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints)
{
	USmashCharacterInputData* InputData = LoadInputDataFromConfig();
	UInputMappingContext* InputMappingContext = LoadInputMappingContextFromConfig();

	
	for (AArenaPlayerStart* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type inputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> CharacterClass = GetSmashCharacterClassFromInputType(inputType);
		if(CharacterClass == nullptr) continue;

		ASmashCharacter* Character = GetWorld()->SpawnActorDeferred<ASmashCharacter>(CharacterClass, SpawnPoint->GetTransform());

		if(Character == nullptr) continue;
		Character->InputData = InputData;
		Character->InputMappingContext = InputMappingContext;
		Character->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		Character->SetOrientX(SpawnPoint->GetStartOrientX());
		Character->FinishSpawning(SpawnPoint->GetTransform());

		CharactersInsideArena.Add(Character);
	}
}


void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (MatchWidgetClass)
	{
		MatchWidget = CreateWidget<UMatchWidget>(GetWorld(), MatchWidgetClass);
		if (MatchWidget)
		{
			MatchWidget->AddToViewport();
		}
	}

	CreateAndInitPlayers();
	TArray<AArenaPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorsInArena(PlayerStartsPoints);
	SpawnCharacters(PlayerStartsPoints);
}


void AMatchGameMode::UpdateWidgetPercentages()
{
	if (!MatchWidget) return;

	TArray<int> Percentages;
	for (ASmashCharacter* Character : CharactersInsideArena)
	{
		if (Character)
		{
			Percentages.Add(Character->GetCurrentPercentage());
		}
	}

	MatchWidget->UpdatePercentages(Percentages);
}
