// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraFollowTarget.h"
#include "SmashCharacterSettings.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "Components/SphereComponent.h"
#include "SmashCharacter.generated.h"

class USmashCharacterStateMachine;
class USmashCharacterInputData;
class UEnhancedInputComponent;

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter,
									public ICameraFollowTarget
{
	GENERATED_BODY()
#pragma region Unreal Default
public:
	// Sets default values for this character's properties
	ASmashCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion Unreal Default

#pragma region Orient
	float GetOrientX() const;

	void SetOrientX(float Value);
	void AddPercentage(int Percentage);

protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 1.f;

	void RotateMeshUsingOrientX() const;
	
	
	
#pragma endregion Orient

#pragma region State Machine

public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;


protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;

	
#pragma endregion State Machine


#pragma region Input Data / Mapping Context
public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

protected:
	void SetupMappingContextIntoController() const;


	
#pragma endregion Input Data / Mapping Context

#pragma region Input Move X

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);

	
public:
	float GetInputMoveX() const;

	UPROPERTY()
	FInputMoveXEvent InputMoveXFastEvent;

protected:
	UPROPERTY()
	float InputMoveX = 0.f;

private:
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputMoveX(const FInputActionValue& InputActionValue);
	
	void OnInputMoveXFast(const FInputActionValue& InputActionValue);


#pragma endregion Input Move X
	
#pragma region Input Move Y
public:
	float GetInputMoveY() const;
private:
	void OnInputMoveY(const FInputActionValue& InputActionValue);
		
protected:
	UPROPERTY()
	float InputMoveY = 0.f;
	
#pragma endregion Input Move Y
	
#pragma region Jump
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJump);

	UFUNCTION()
	void OnJump();
public:	
	FOnJump JumpEvent;
	
	bool bDoubleJumped = false;
	
#pragma endregion Jump

#pragma region Attack
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack);

	UFUNCTION()
	void OnAttackInput();
	


public:
	FOnAttack AttackEvent;

	float GetCurrentPercentage();
	float AttackDamage = 0.0f;
	bool bInAir;

	
	UFUNCTION()
	void OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
    									  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
    									  bool bFromSweep, const FHitResult& SweepResult);
    	
	UPROPERTY(EditAnywhere)
	USphereComponent* AttackHitbox;


private:
	float CurrentPercentage = 0.0f;
#pragma endregion Attack

#pragma region Knockback
public:
	void ApplyKnockback(const FVector& Direction, float BaseForce);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> HitAnim;
protected:
	UPROPERTY(EditAnywhere)
	float KnockbackMultiplier = 0.5f;


#pragma endregion Knockback

#pragma region Camera Target

	virtual bool IsFollowable() override;
	
	virtual FVector GetFollowPosition() override;
#pragma endregion
};

