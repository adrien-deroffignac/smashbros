// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedPlayerInput.h"
#include "SmashCharacterInputData.h"
#include "Engine/DeveloperSettings.h"
#include "SmashCharacterSettings.generated.h"


class UInputMappingContext;

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta =(DisplayName = "Smash Character Settings"))
class SMASHUE_API USmashCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config, Category = "Inputs")
	TSoftObjectPtr<USmashCharacterInputData> InputData;

	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	float InputMoveXThreshold = 0.1f;

	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	float InputMoveYThreshold = 0.1f;
};
