// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmashCharacterState.h"
#include "Components/SphereComponent.h"
#include "SmashCharacterStateAttack.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateAttack : public USmashCharacterState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashCharacterStateAttack();

protected:
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;
	virtual void StateExit(ESmashCharacterStateID NextStateID) override;
	virtual void StateTick(float DeltaTime) override;
	virtual ESmashCharacterStateID GetStateID() override;
	void PerformDirectionalAttack();



	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> NeutralAttackAnim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> UpAttackAnim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> DownAttackAnim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> SideAttackAnim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> FNeutralAttackAnim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> FUpAttackAnim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> FDownAttackAnim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> FSideAttackAnim;
	
	float AttackDuration = 1.5f; 


private:
	float ElapsedTime;
};
