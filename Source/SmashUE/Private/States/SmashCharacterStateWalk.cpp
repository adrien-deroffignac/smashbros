// Fill out your copyright notice in the Description page of Project Settings.


#include "States/SmashCharacterStateWalk.h"

#include "States/SmashCharacter.h"
#include "States/SmashCharacterStateMachine.h"


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
	Character->JumpEvent.AddDynamic(this, &USmashCharacterStateWalk::OnJump);
	Character->AttackEvent.AddDynamic(this, &USmashCharacterStateWalk::OnAttack);

}

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->JumpEvent.RemoveDynamic(this, &USmashCharacterStateWalk::OnJump);
	Character->AttackEvent.RemoveDynamic(this, &USmashCharacterStateWalk::OnAttack);
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

	if (Character->GetVelocity().Z < 0.0f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

void USmashCharacterStateWalk::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

void USmashCharacterStateWalk::OnJump()
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}

void USmashCharacterStateWalk::OnAttack()
{
}

