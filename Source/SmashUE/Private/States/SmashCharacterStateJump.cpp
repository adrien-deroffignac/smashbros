#include "States/SmashCharacterStateJump.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "States/SmashCharacter.h"
#include "States/SmashCharacterStateMachine.h"

USmashCharacterStateJump::USmashCharacterStateJump()
{
	PrimaryComponentTick.bCanEverTick = true;
	ElapsedTime = 0.0f;
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	ElapsedTime = 0.0f;

	Character->PlayAnimMontage(JumpAnim);
	if (Character->GetOrientX() > 0.0f)
	{
		Character->LaunchCharacter(FVector(1.0f, 0.0f, JumpMaxHeight), false, true);
	}
	else if (Character->GetOrientX() < 0.0f)
	{
		Character->LaunchCharacter(FVector(-1.0f, 0.0f, JumpMaxHeight), false, true);
	}
	else
	{
		Character->LaunchCharacter(FVector(0.0f, 1.0f, JumpMaxHeight), false, true);
	}
	
	Character->GetCharacterMovement()->AirControl = JumpAirControl;
	Character->bInAir = true;
	Character->JumpEvent.AddDynamic(this, &USmashCharacterStateJump::OnDoubleJump);
	Character->AttackEvent.AddDynamic(this, &USmashCharacterStateJump::OnAttack);
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	Character->JumpEvent.RemoveDynamic(this, &USmashCharacterStateJump::OnDoubleJump);
	Character->AttackEvent.RemoveDynamic(this, &USmashCharacterStateJump::OnAttack);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	ElapsedTime += DeltaTime;
	
	
	if (ElapsedTime >= JumpDuration || Character->GetVelocity().Z <= 0.0f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}else
	{
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
}

void USmashCharacterStateJump::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::OnDoubleJump()
{
	if (Character->bDoubleJumped) return;
	StateMachine->ChangeState(ESmashCharacterStateID::DoubleJump);
}

void USmashCharacterStateJump::OnAttack()
{
	StateMachine->ChangeState(ESmashCharacterStateID::Attack);
}
