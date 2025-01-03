#include "States/SmashCharacterStateJump.h"
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
	Character->LaunchCharacter(FVector(0.0f, 0.0f, JumpMaxHeight), false, true);
	Character->JumpEvent.AddDynamic(this, &USmashCharacterStateJump::OnDoubleJump);
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	Character->StopAnimMontage(JumpAnim);
	Character->JumpEvent.RemoveDynamic(this, &USmashCharacterStateJump::OnDoubleJump);

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
	if (Character->hasDoubleJumped) return;
	StateMachine->ChangeState(ESmashCharacterStateID::DoubleJump);
}
