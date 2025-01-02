// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaSettings.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"

class ASmashCharacter;
class AArenaPlayerStart;
class UInputMappingContext;
class USmashCharacterInputData;
/**
 * 
 */
UCLASS()
class SMASHUE_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	

protected:
	UPROPERTY(EditAnywhere)
	TArray<ASmashCharacter*> CharactersInsideArena;


private:
	USmashCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig();

	void CreateAndInitPlayers() const;

	
	void FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*> &ResultsActors);

	TSubclassOf<ASmashCharacter> GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type inputType) const;

	void SpawnCharacters(const TArray<AArenaPlayerStart*> &SpawnPoints);
};
