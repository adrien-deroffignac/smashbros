// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraSettings.h"
#include "Camera/CameraComponent.h"
#include "CameraWorldSubsytem.generated.h"

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

protected:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraMain;

	void TickUpdateCameraPosition(float DeltaTime);

	void TickUpdateCameraZoom(float DeltaTime);

	
#pragma endregion Main Camera
	
#pragma region FollowTargets	
public:
	void AddFollowActor (UObject* FollowTarget);

	void RemoveFollowTarget(UObject* FollowTarget);
	
protected:
	UPROPERTY()
	TArray<UObject*> FollowTargets;
	
	FVector CalculateAveragePositionsBetweenTargets();

	float CalculateGreatestDistanceBetweenTargets();
	
#pragma endregion FollowTargets

#pragma region Misc

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

#pragma region Zoom
protected:
	UPROPERTY()
	float CameraZoomYMin = 0.f;

	UPROPERTY()
	float CameraZoomYMax = 0.f;

	UPROPERTY()
	float CameraZoomDistanceBetweenTargetsMin = 300.f;

	UPROPERTY()
	float CameraZoomDistanceBetweenTargetsMax = 1500.f;

	UFUNCTION()
	void InitCameraZoomParameters();

#pragma endregion Zoom
private:
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraSettings* CameraSettings;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};


