// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraWorldSubsytem.generated.h"
#include "Camera/CameraComponent.h"

/**
 * 
 */
UCLASS()
class SMASHUE_API UCameraWorldSubsytem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
#pragma region Subsystem Override
	virtual void PostInitialize() override;
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override{ return TStatId();};
#pragma endregion Subsystem Override
	
#pragma region Main Camera
	void AddFollowActor (UObject* FollowTarget);

	void RemoveFollowTarget(UObject* FollowTarget);

protected:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraMain;
#pragma endregion Main Camera
	
#pragma region FollowTargets	
	UPROPERTY()
	TArray<UObject*> FollowTargets;

	void TickUpdateCameraPosition(float DeltaTime);
#pragma endregion FollowTargets

#pragma region Misc
	FVector CalculateAveragePositionsBetweenTargets();

	UCameraComponent* FindCameraByTag(const FName& Tag) const;
#pragma endregion Misc

#pragma region Bounds
protected:
	UPROPERTY()
	FVector2D CameraBoundsMin;

	UPROPERTY()
	FVector2D CameraBoundsMax;

	UPROPERTY()
	float CameraBoundsYProjectionCenter;

	AActor* FindCameraBoundsActor();
	
	void InitCameraBounds(AActor* CameraBoundsActor);

	void ClampPositionIntoCameraBounds(FVector& Position);

	void GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax);

	FVector CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition);
	
#pragma endregion Bounds
	
};


