// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmashCharacterState.h"
#include "SmashCharacterStateFall.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateFall : public USmashCharacterState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashCharacterStateFall();
	
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	
	virtual void StateExit(ESmashCharacterStateID NextStateID) override;
	
	virtual void StateTick(float DeltaTime) override;
	
	virtual ESmashCharacterStateID GetStateID() override;
	
	UFUNCTION()
	void DoubleJump();

	UFUNCTION()
	void OnAttack();
protected:
	UPROPERTY(EditAnywhere)
	float FallHorizontalMoveSpeed = 400.0f;

	UPROPERTY(EditAnywhere)
	float FallAirControl = 1.0f;

	UPROPERTY(EditAnywhere)
	float FallGravityScale = 3.0f;

	UPROPERTY(EditAnywhere)
	float FallFastGravityScale = 10.0f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> FallAnim;
	
	void CheckLanding();
};
