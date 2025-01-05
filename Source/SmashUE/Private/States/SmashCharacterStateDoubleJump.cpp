// Fill out your copyright notice in the Description page of Project Settings.


#include "States/SmashCharacterStateDoubleJump.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "States/SmashCharacter.h"
#include "States/SmashCharacterStateMachine.h"


// Sets default values for this component's properties
USmashCharacterStateDoubleJump::USmashCharacterStateDoubleJump()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ElapsedTime = 0.0f;
}





ESmashCharacterStateID USmashCharacterStateDoubleJump::GetStateID()
{
	return ESmashCharacterStateID::DoubleJump;
}


void USmashCharacterStateDoubleJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	ElapsedTime = 0.0f;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DoubleJump"));
	Character->bDoubleJumped = true;

	Character->PlayAnimMontage(DoubleJumpAnim);
	
	Character->GetCharacterMovement()->StopActiveMovement();
	if (Character->GetOrientX() > 0.0f)
	{
		Character->LaunchCharacter(FVector(1.0f, 0.0f, DoubleJumpMaxHeight), false, true);
	}
	else if (Character->GetOrientX() < 0.0f)
	{
		Character->LaunchCharacter(FVector(-1.0f, 0.0f, DoubleJumpMaxHeight), false, true);
	}
	else
	{
		Character->LaunchCharacter(FVector(0.0f, 1.0f, DoubleJumpMaxHeight), false, true);
	}
}

void USmashCharacterStateDoubleJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateDoubleJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	ElapsedTime += DeltaTime;
	
	
	if (ElapsedTime >= DoubleJumpDuration || Character->GetVelocity().Z < 0.0f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
	
	Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	
}

