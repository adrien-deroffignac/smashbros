// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"
#include "Kismet/GameplayStatics.h"

void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{	
	ULocalMultiplayerSettings* LocalMultiplayerSettings = GetMutableDefault<ULocalMultiplayerSettings>();

	for (int i = 0; i < LocalMultiplayerSettings->GetNbKeyboardProfiles(); i++)
	{
		int AssignedPlayerIndex = AssignNewPlayerToKeyboardProfile(i);
		AssignKeyboardMapping(AssignedPlayerIndex, i, MappingType);
	}

	for  (int i = 0; i < LocalMultiplayerSettings->NbMaxGamePads; i++)
	{
		int AssignedPlayerIndex = AssignNewPlayerToGamepadDeviceID(i);
		AssignGamepadInputMapping(AssignedPlayerIndex, MappingType);
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
	if(PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex))
	{
		return PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	int Index = GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
	if(Index != -1)
	{
		return -1;
	}
	
	LastAssignedPlayerIndex++;
	PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, LastAssignedPlayerIndex);
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	ULocalMultiplayerSettings* LocalMultiplayerSettings = GetMutableDefault<ULocalMultiplayerSettings>();

	FLocalMultiplayerProfileData KeyboardProfile = LocalMultiplayerSettings->KeyboardProfilesData[KeyboardProfileIndex];
	UInputMappingContext* IMC = KeyboardProfile.GetIMCFromType(MappingType);
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if(PlayerController != nullptr)
	{
		PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddMappingContext(IMC, 0);
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	if(PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))
	{
		return PlayerIndexFromGamepadProfileIndex[DeviceID];
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	int Index = GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID);
	if(Index == -1)
	{
		return -1;
	}
	
	LastAssignedPlayerIndex++;
	PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	ULocalMultiplayerSettings* LocalMultiplayerSettings = GetMutableDefault<ULocalMultiplayerSettings>();

	FLocalMultiplayerProfileData GamepadProfile = LocalMultiplayerSettings->GamepadProfileData;
	UInputMappingContext* IMC = GamepadProfile.GetIMCFromType(MappingType);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);

	if(PlayerController != nullptr)
	{
		PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddMappingContext(IMC, 0);
	}
}
