// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmashCharacterState.h"
#include "SmashCharacterStateDoubleJump.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateDoubleJump : public USmashCharacterState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashCharacterStateDoubleJump();
	
	virtual ESmashCharacterStateID GetStateID() override;
	
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

protected:

	float ElapsedTime;
	
	UPROPERTY(EditAnywhere)
	float DoubleJumpWalkSpeed = 500.0f;

	UPROPERTY(EditAnywhere)
	float DoubleJumpMaxHeight = 600.0f;

	UPROPERTY(EditAnywhere)
	float DoubleJumpDuration = 1.5f;

	UPROPERTY(EditAnywhere)
	float DoubleJumpAirControl = 1.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> DoubleJumpAnim;
};
 