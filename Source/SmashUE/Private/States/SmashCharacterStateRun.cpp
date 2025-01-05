// Fill out your copyright notice in the Description page of Project Settings.


#include "States/SmashCharacterStateRun.h"

#include "States/SmashCharacter.h"
#include "States/SmashCharacterStateMachine.h"


// Sets default values for this component's properties
USmashCharacterStateRun::USmashCharacterStateRun()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USmashCharacterStateRun::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USmashCharacterStateRun::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->PlayAnimMontage(RunAnim);
	Character->JumpEvent.AddDynamic(this, &USmashCharacterStateRun::OnJump);
	Character->AttackEvent.AddDynamic(this, &USmashCharacterStateRun::OnAttack);


}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->AddMovementInput(FVector::ForwardVector, 0.0f);
	Character->JumpEvent.RemoveDynamic(this, &USmashCharacterStateRun::OnJump);
	Character->AttackEvent.RemoveDynamic(this, &USmashCharacterStateRun::OnAttack);


}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (FMath::Abs(Character->GetInputMoveX()) < CharacterSettings->InputMoveXThreshold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetInputMoveX() * RunSpeed);
	}

	if (Character->GetVelocity().Z < 0.0f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

void USmashCharacterStateRun::OnJump()
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}

void USmashCharacterStateRun::OnAttack()
{
	StateMachine->ChangeState(ESmashCharacterStateID::Attack);
}


