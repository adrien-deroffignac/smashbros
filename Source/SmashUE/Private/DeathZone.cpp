#include "DeathZone.h"
#include "Components/BoxComponent.h"
#include "States/SmashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Arena/ArenaPlayerRespawn.h"
#include "Arena/ArenaPlayerRespawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "States/SmashCharacter.h"

ADeathZone::ADeathZone()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ADeathZone::OnOverlapBegin);
}

void ADeathZone::BeginPlay()
{
    Super::BeginPlay();
}

void ADeathZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
                                bool bFromSweep, const FHitResult& SweepResult)
{
    ASmashCharacter* SmashCharacter = Cast<ASmashCharacter>(OtherActor);
    if (SmashCharacter)
    {
        SmashCharacter->AddPercentage(-SmashCharacter->GetCurrentPercentage());

        TArray<AActor*> RespawnPoints;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerRespawn::StaticClass(), RespawnPoints);

        if (RespawnPoints.Num() > 0)
        {
            AActor* RespawnPoint = RespawnPoints[FMath::RandRange(0, RespawnPoints.Num() - 1)];
            FVector RespawnLocation = RespawnPoint->GetActorLocation();
            FRotator RespawnRotation = RespawnPoint->GetActorRotation();
            
            SmashCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
            
            SmashCharacter->SetActorLocation(RespawnLocation);
            SmashCharacter->SetActorRotation(RespawnRotation);
        }
    }
}
