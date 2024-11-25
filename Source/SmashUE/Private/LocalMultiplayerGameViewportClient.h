// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "LocalMultiplayerGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class SMASHUE_API ULocalMultiplayerGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	virtual void PostInitProperties() override;

	virtual bool InputKey(const FInputKeyEventArgs& EventArgs) override;

};
