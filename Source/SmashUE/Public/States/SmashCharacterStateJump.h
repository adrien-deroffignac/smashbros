// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmashCharacterState.h"
#include "SmashCharacterStateJump.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateJump : public USmashCharacterState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashCharacterStateJump();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual ESmashCharacterStateID GetStateID() override;

	UFUNCTION()
	void OnDoubleJump();

	UFUNCTION()
	void OnAttack();
	
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

protected:

	float ElapsedTime;
	
	UPROPERTY(EditAnywhere)
	float JumpWalkSpeed = 500.0f;

	UPROPERTY(EditAnywhere)
	float JumpMaxHeight = 600.0f;

	UPROPERTY(EditAnywhere)
	float JumpDuration = 1.5f;

	UPROPERTY(EditAnywhere)
	float JumpAirControl = 1.f;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> JumpAnim;
	
};
 