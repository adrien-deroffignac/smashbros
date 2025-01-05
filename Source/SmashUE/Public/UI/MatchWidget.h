// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchWidget.generated.h"

/**
 * 
 */
UCLASS()
class SMASHUE_API UMatchWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdatePercentages(const TArray<int>& Percentages);
};
