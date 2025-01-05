// Fill out your copyright notice in the Description page of Project Settings.


#include "States/SmashCharacterStateIdle.h"

#include "States/SmashCharacter.h"
#include "States/SmashCharacterStateMachine.h"


// Sets default values for this component's properties
USmashCharacterStateIdle::USmashCharacterStateIdle()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USmashCharacterStateIdle::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USmashCharacterStateIdle::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ESmashCharacterStateID USmashCharacterStateIdle::GetStateID()
{
	return ESmashCharacterStateID::Idle;
}

void USmashCharacterStateIdle::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	Character->PlayAnimMontage(IdleAnim);

	Character->InputMoveXFastEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
	Character->JumpEvent.AddDynamic(this, &USmashCharacterStateIdle::OnJump);
	Character->AttackEvent.AddDynamic(this, &USmashCharacterStateIdle::OnAttack);
}

void USmashCharacterStateIdle::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	
	Character->StopAnimMontage(IdleAnim);
	
	Character->InputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
	Character->JumpEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnJump);
	Character->AttackEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnAttack);
}

void USmashCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if(FMath::Abs(Character->GetInputMoveX()) > 0.1f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
	
	if (Character->GetVelocity().Z < 0.0f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

void USmashCharacterStateIdle::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

void USmashCharacterStateIdle::OnJump()
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}

void USmashCharacterStateIdle::OnAttack()
{
	StateMachine->ChangeState(ESmashCharacterStateID::Attack);
}

