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
	CameraMain = FindCameraByTag(CameraSettings->CameraMainTag);

	AActor* CameraBoundsActor = FindCameraBoundsActor();
	if (CameraBoundsActor != nullptr)
	{
		InitCameraBounds(CameraBoundsActor);
	}

	InitCameraZoomParameters();
}

void UCameraWorldSubsytem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraZoom(DeltaTime); 
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
	if (CameraMain == nullptr || FollowTargets.Num() == 0) return;

	FVector TargetPosition = CalculateAveragePositionsBetweenTargets();
	FVector CurrentPosition = CameraMain->GetComponentLocation();

	float PositionDamping = CameraSettings->PositionDampingFactor; 
	FVector SmoothedPosition = FMath::Lerp(CurrentPosition, TargetPosition, FMath::Clamp(DeltaTime * PositionDamping, 0.0f, 1.0f));

	CameraMain->SetWorldLocation(SmoothedPosition);
}


void UCameraWorldSubsytem::TickUpdateCameraZoom(float DeltaTime)
{
	if (CameraMain == nullptr || FollowTargets.Num() < 2) return;

	float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();
	float ZoomPercent = FMath::GetMappedRangeValueClamped(
		FVector2D(CameraSettings->DistanceBetweenTargetsMin, CameraSettings->DistanceBetweenTargetsMax),
		FVector2D(0.0f, 1.0f),
		GreatestDistanceBetweenTargets
	);

	float TargetCameraY = FMath::Lerp(CameraZoomYMin, CameraZoomYMax, ZoomPercent);
	FVector CurrentLocation = CameraMain->GetComponentLocation();

	float SizeDamping = CameraSettings->SizeDampingFactor;
	float SmoothedY = FMath::Lerp(CurrentLocation.Y, TargetCameraY, FMath::Clamp(DeltaTime * SizeDamping, 0.0f, 1.0f));

	CurrentLocation.Y = SmoothedY;
	CameraMain->SetWorldLocation(CurrentLocation);
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

float UCameraWorldSubsytem::CalculateGreatestDistanceBetweenTargets()
{
	float GreatestDistance = 0.f;
	for(int i = 0; i < FollowTargets.Num(); i++)
	{
		if(ICameraFollowTarget* CameraFollowTarget = Cast<ICameraFollowTarget>(FollowTargets[i]))
		{
			for(int j = i + 1; j < FollowTargets.Num(); j++)
			{
				if(ICameraFollowTarget* CameraFollowTarget2 = Cast<ICameraFollowTarget>(FollowTargets[j]))
				{
					float Distance = FVector::Dist(CameraFollowTarget->GetFollowPosition(), CameraFollowTarget2->GetFollowPosition());
					if(Distance > GreatestDistance)
					{
						GreatestDistance = Distance;
					}
				}
			}
		}
	}
	return GreatestDistance;
}

UCameraComponent* UCameraWorldSubsytem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (UCameraComponent* CameraComponent = Actor->FindComponentByClass<UCameraComponent>())
		{
			return CameraComponent;
		}
	}
	return nullptr;
}

AActor* UCameraWorldSubsytem::FindCameraBoundsActor()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), CameraSettings->CameraBoundsTag, FoundActors);

	if(FoundActors.Num() > 0)
	{
		return FoundActors[0];
	}
	return nullptr;
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

void UCameraWorldSubsytem::InitCameraZoomParameters()
{
	
	CameraZoomYMin = 0.f;
	CameraZoomYMax = 0.f;

	UCameraComponent* MinActor = FindCameraByTag("CameraDistanceMin");
	if (MinActor)
	{
		CameraZoomYMin = MinActor->GetComponentLocation().Y;
	}

	UCameraComponent* MaxActor = FindCameraByTag("CameraDistanceMax");
	if (MaxActor)
	{
		CameraZoomYMax = MaxActor->GetComponentLocation().Y;
	}
	
}

void UCameraWorldSubsytem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CameraSettings = NewObject<UCameraSettings>(this, UCameraSettings::StaticClass());
}


