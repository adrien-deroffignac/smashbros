// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LocalMultiplayerSettings.h"

#include "LocalMultiplayerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SMASHUE_API ULocalMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType);

	int GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex);

	int AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex);

	void AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const;

	int GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID);

	int AssignNewPlayerToGamepadDeviceID(int DeviceID);

	void AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const;

public:
	ELocalMultiplayerInputMappingType GetCurrentInputMappingType() const;
	void SetCurrentInputMappingType(const ELocalMultiplayerInputMappingType NewType);

	int8 GetLastAssignedPlayerIndex() const;
protected:
	UPROPERTY()
	ELocalMultiplayerInputMappingType CurrentInputMappingType;
	
	UPROPERTY()
	int8 LastAssignedPlayerIndex = -1;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromKeyboardProfileIndex;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromGamepadProfileIndex;
};
