#include "States/SmashCharacterStateFall.h"
#include "States/SmashCharacter.h"
#include "States/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

USmashCharacterStateFall::USmashCharacterStateFall()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->PlayAnimMontage(FallAnim);
	
	Character->GetCharacterMovement()->GravityScale = FallGravityScale;
	Character->GetCharacterMovement()->AirControl = FallAirControl;
	
	Character->JumpEvent.AddDynamic(this, &USmashCharacterStateFall::DoubleJump);
	Character->AttackEvent.AddDynamic(this, &USmashCharacterStateFall::OnAttack);

}


void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	
	Character->GetCharacterMovement()->GravityScale = 1.0f;
	Character->JumpEvent.RemoveDynamic(this, &USmashCharacterStateFall::DoubleJump);
	Character->AttackEvent.RemoveDynamic(this, &USmashCharacterStateFall::OnAttack);
	Character->GetCharacterMovement()->AirControl = 1.0f;
	Character->SetOrientX(Character->GetInputMoveX());
}

void USmashCharacterStateFall::DoubleJump()
{
	if (Character->bDoubleJumped) return;
	StateMachine->ChangeState(ESmashCharacterStateID::DoubleJump);
}

void USmashCharacterStateFall::OnAttack()
{
	Character->bInAir = true;
	StateMachine->ChangeState(ESmashCharacterStateID::Attack);
}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	CheckLanding();
}

void USmashCharacterStateFall::CheckLanding()
{
	if (!Character->GetCharacterMovement()->IsFalling())
	{
		Character->bDoubleJumped = false;
		Character->bInAir = false;
		if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold)
		{
			StateMachine->ChangeState(ESmashCharacterStateID::Run);
		}
		else
		{
			StateMachine->ChangeState(ESmashCharacterStateID::Idle);
		}
	}
	
	Character->SetOrientX(Character->GetInputMoveX());
	
}

ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}
