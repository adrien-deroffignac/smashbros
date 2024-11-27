// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraWorldSubsytem.h"

#include "CameraFollowTarget.h"
#include "Kismet/GameplayStatics.h"

void UCameraWorldSubsytem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsytem::OnWorldBeginPlay(UWorld& InWorld)
{
	CameraMain = FindCameraByTag(TEXT("CameraMain"));

	AActor* CameraBoundsActor = FindCameraBoundsActor();
	if (CameraBoundsActor != nullptr)
	{
		InitCameraBounds(CameraBoundsActor);
	}
}

void UCameraWorldSubsytem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

void UCameraWorldSubsytem::AddFollowActor(UObject* FollowTarget)
{
	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsytem::RemoveFollowTarget(UObject* FollowTarget)
{
    FollowTargets.Remove(FollowTarget);
}


void UCameraWorldSubsytem::TickUpdateCameraPosition(float DeltaTime)
{
    if(CameraMain == nullptr) return;
    if(FollowTargets.Num() == 0) return;

	FVector AveragePosition = CalculateAveragePositionsBetweenTargets();
	CameraMain->SetWorldLocation(AveragePosition);
	
}

FVector UCameraWorldSubsytem::CalculateAveragePositionsBetweenTargets()
{
    FVector AveragePosition = FVector::ZeroVector;
    for(UObject* FollowTarget : FollowTargets)
    {
    	if(ICameraFollowTarget* CameraFollowTarget = Cast<ICameraFollowTarget>(FollowTarget))
		{
			AveragePosition += CameraFollowTarget->GetFollowPosition();
		}
    }
    AveragePosition /= FollowTargets.Num();
    return AveragePosition;
}

UCameraComponent* UCameraWorldSubsytem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);

	for(AActor* Actor : FoundActors)
	{
		if(UCameraComponent* CameraComponent = Actor->FindComponentByClass<UCameraComponent>())
		{
			return CameraComponent;
		}
	}
	return nullptr;
}

AActor* UCameraWorldSubsytem::FindCameraBoundsActor()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("CameraBounds"), FoundActors);
	return FoundActors.Num() > 0 ? FoundActors[0] : nullptr;
}

void UCameraWorldSubsytem::InitCameraBounds(AActor* CameraBoundsActor)
{
	FVector BoundsCenter;
	FVector BoundsExtents;
	CameraBoundsActor->GetActorBounds(false, BoundsCenter, BoundsExtents);

	CameraBoundsMin = FVector2D(BoundsCenter.X - BoundsExtents.X, BoundsCenter.Y - BoundsExtents.Y);
	CameraBoundsMax = FVector2D(BoundsCenter.X + BoundsExtents.X, BoundsCenter.Y + BoundsExtents.Y);

	CameraBoundsYProjectionCenter = BoundsCenter.Y;
}

void UCameraWorldSubsytem::ClampPositionIntoCameraBounds(FVector& Position)
{
	FVector2D ViewportBoundsMin, ViewportBoundsMax;
	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);

	FVector WorldBoundsMin = CalculateWorldPositionFromViewportPosition(ViewportBoundsMin);
	FVector WorldBoundsMax = CalculateWorldPositionFromViewportPosition(ViewportBoundsMax);

	Position.X = FMath::Clamp(Position.X, WorldBoundsMin.X, WorldBoundsMax.X);
	Position.Y = FMath::Clamp(Position.Y, WorldBoundsMin.Y, WorldBoundsMax.Y);
}

void UCameraWorldSubsytem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if(ViewportClient == nullptr) return;

	FViewport* Viewport = ViewportClient->Viewport;
	if(Viewport == nullptr) return;

	FIntRect ViewRect(
		Viewport->GetInitialPositionXY(),
		Viewport->GetInitialPositionXY() + Viewport->GetSizeXY()
		);

	FIntRect ViewportRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio, ViewRect);

	OutViewportBoundsMin.X = ViewportRect.Min.X;
	OutViewportBoundsMin.Y = ViewportRect.Min.Y;

	OutViewportBoundsMax.X = ViewportRect.Max.X;
	OutViewportBoundsMax.Y = ViewportRect.Max.Y;
	
}

FVector UCameraWorldSubsytem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if (CameraMain == nullptr) return FVector::Zero();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PlayerController == nullptr) return FVector::Zero();

	float YDistanceToCenter = CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundsYProjectionCenter;

	FVector CameraWorldProjectDir;
	FVector WorldPosition;
	UGameplayStatics::DeprojectScreenToWorld(
		PlayerController,
		ViewportPosition,
		WorldPosition,
		CameraWorldProjectDir
		);

	WorldPosition += CameraWorldProjectDir * YDistanceToCenter;

	return WorldPosition;
}


