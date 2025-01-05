#include "States/SmashCharacterStateAttack.h"
#include "States/SmashCharacter.h"
#include "States/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

USmashCharacterStateAttack::USmashCharacterStateAttack()
{
    PrimaryComponentTick.bCanEverTick = true;
    ElapsedTime = 0.0f;
   
}

void USmashCharacterStateAttack::StateEnter(ESmashCharacterStateID PreviousStateID)
{
    Super::StateEnter(PreviousStateID);
    
    
    ElapsedTime = 0.0f;
    
    PerformDirectionalAttack();
    Character->AttackHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    
}

void USmashCharacterStateAttack::StateExit(ESmashCharacterStateID NextStateID)
{
    Super::StateExit(NextStateID);
    Character->AttackHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void USmashCharacterStateAttack::StateTick(float DeltaTime)
{
    Super::StateTick(DeltaTime);

    ElapsedTime += DeltaTime;

    if (ElapsedTime >= AttackDuration + 0.1f)
    {
        StateMachine->ChangeState(ESmashCharacterStateID::Idle);
    }
}

ESmashCharacterStateID USmashCharacterStateAttack::GetStateID()
{
    return ESmashCharacterStateID::Attack;
}

void USmashCharacterStateAttack::PerformDirectionalAttack()
{
    switch (Character->bInAir)
    {
        case true:
            if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold)
            {
                AttackDuration = FSideAttackAnim->GetPlayLength();
                Character->PlayAnimMontage(FSideAttackAnim);
                Character->AttackDamage = 5.0f;
            }
            else if (Character->GetInputMoveY() > CharacterSettings->InputMoveYThreshold)
            {
                AttackDuration = FUpAttackAnim->GetPlayLength();
                Character->PlayAnimMontage(FUpAttackAnim);
                Character->AttackDamage = 7.0f;
            }else if (Character->GetInputMoveY() < -CharacterSettings->InputMoveYThreshold)
            {
                AttackDuration = FDownAttackAnim->GetPlayLength();
                Character->PlayAnimMontage(FDownAttackAnim);
                Character->AttackDamage = 6.0f;
            }else
            {
                AttackDuration = FNeutralAttackAnim->GetPlayLength();
                Character->PlayAnimMontage(FNeutralAttackAnim);
                Character->AttackDamage = 4.0f;
            }
            break;
        
        case false:
        if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold)
        {
            AttackDuration = SideAttackAnim->GetPlayLength();
            Character->PlayAnimMontage(SideAttackAnim);
            Character->AttackDamage = 4.0f;
        }
        else if (Character->GetInputMoveY() > CharacterSettings->InputMoveYThreshold)
        {
            AttackDuration = UpAttackAnim->GetPlayLength();
            Character->PlayAnimMontage(UpAttackAnim);
            Character->AttackDamage = 6.0f;
        }
        else if (Character->GetInputMoveY() < -CharacterSettings->InputMoveYThreshold)
        {
            AttackDuration = DownAttackAnim->GetPlayLength();
            Character->PlayAnimMontage(DownAttackAnim);
             Character->AttackDamage = 5.0f;
        }else
        {
            AttackDuration = NeutralAttackAnim->GetPlayLength();
            Character->PlayAnimMontage(NeutralAttackAnim);
            Character-> AttackDamage = 3.0f;
        }
        break;
    }
}




