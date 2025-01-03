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
	Character->JumpEvent.AddDynamic(this, &USmashCharacterStateFall::DoubleJump);

}


void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	Character->StopAnimMontage(FallAnim);
	Character->GetCharacterMovement()->GravityScale = 1.0f;
	Character->JumpEvent.RemoveDynamic(this, &USmashCharacterStateFall::DoubleJump);

}

void USmashCharacterStateFall::DoubleJump()
{
	if (Character->hasDoubleJumped) return;
	StateMachine->ChangeState(ESmashCharacterStateID::DoubleJump);
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
		Character->hasDoubleJumped = false;
		if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold)
		{
			StateMachine->ChangeState(ESmashCharacterStateID::Run);
		}
		else
		{
			StateMachine->ChangeState(ESmashCharacterStateID::Idle);
		}
	}else {
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
}

ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}
