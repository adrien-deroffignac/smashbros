// Fill out your copyright notice in the Description page of Project Settings.


#include "SmashCharacterStateWalk.h"

#include "SmashCharacter.h"
#include "SmashCharacterStateMachine.h"


// Sets default values for this component's properties
USmashCharacterStateWalk::USmashCharacterStateWalk()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USmashCharacterStateWalk::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USmashCharacterStateWalk::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ESmashCharacterStateID USmashCharacterStateWalk::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->PlayAnimMontage(WalkAnim);
}

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->StopAnimMontage(WalkAnim);
	
}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if(FMath::Abs(Character->GetInputMoveX()) < 0.1f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else
	{
		
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
}

