// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"

class ASmashCharacter;
class AArenaPlayerStart;
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
	TSubclassOf<ASmashCharacter> CharacterClassP0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> CharacterClassP1;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> CharacterClassP2;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> CharacterClassP3;

private:
	void FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*> &ResultsActors);

	TSubclassOf<ASmashCharacter> GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type inputType) const;
};
