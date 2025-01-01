#pragma once

#include "CoreMinimal.h"
#include "CameraSettings.generated.h"

/**
 *
 */
UCLASS(Config = Game, DefaultConfig, meta =(DisplayName = "Camera Settings"))
class SMASHUE_API UCameraSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Camera Settings")
	FName CameraMainTag = TEXT("CameraMain");

	UPROPERTY(Config, EditAnywhere, Category = "Camera Settings")
	FName CameraDistanceMinTag = TEXT("CameraDistanceMin");

	UPROPERTY(Config, EditAnywhere, Category = "Camera Settings")
	FName CameraDistanceMaxTag = TEXT("CameraDistanceMax");

	UPROPERTY(Config, EditAnywhere, Category = "Camera Settings")
	FName CameraBoundsTag = TEXT("CameraBounds");

	UPROPERTY(Config, EditAnywhere, Category = "Camera Settings")
	float DistanceBetweenTargetsMin = 300.0f;

	UPROPERTY(Config, EditAnywhere, Category = "Camera Settings")
	float DistanceBetweenTargetsMax = 1500.0f;

	UPROPERTY(Config, EditAnywhere, Category = "Camera Settings")
	float SizeDampingFactor = 5.0f;

	UPROPERTY(Config, EditAnywhere, Category = "Camera Settings")
	float PositionDampingFactor = 5.0f;

};
