// Fill out your copyright notice in the Description page of Project Settings.


#include "ULocalMultiplayerSettings.h"


bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	UInputMappingContext* IMC = GetIMCFromType(MappingType);
	for (auto Mapping : IMC->GetMappings())
	{
		if (Mapping.Key == Key)
		{
			return true;
		}
	}
	return false;
}

UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
	switch (MappingType)
	{
	case ELocalMultiplayerInputMappingType::InGame:
		return IMCInGame.Get();
	case ELocalMultiplayerInputMappingType::Menu:
		return IMCMenu.Get();
	}
	return nullptr;
}

int UULocalMultiplayerSettings::GetNbKeyboardProfiles() const
{
	return KeyboardProfilesData.Num();
}

int UULocalMultiplayerSettings::FindKeyboardProfileIndexFromKey(const FKey& Key,
	ELocalMultiplayerInputMappingType MappingType) const
{
	for (int i = 0; i < KeyboardProfilesData.Num(); i++)
	{
		if (KeyboardProfilesData[i].ContainsKey(Key, MappingType))
		{
			return i;
		}
	}
	return -1;
}
