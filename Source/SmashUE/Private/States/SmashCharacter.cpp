// Fill out your copyright notice in the Description page of Project Settings.


#include "States/SmashCharacter.h"

#include "CameraWorldSubsytem.h"
#include "States/SmashCharacterStateMachine.h"
#include "EnhancedInputSubsystems.h"
#include "SmashCharacterSettings.h"
#include "EnhancedInputComponent.h"
#include "MatchGameMode.h"
#include "Components/SphereComponent.h"

// Sets default values
ASmashCharacter::ASmashCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bInAir = false;
	AttackHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("AttackHitbox"));
	AttackHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackHitbox->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();
	InitStateMachine();

	AttackHitbox->OnComponentBeginOverlap.AddDynamic(this, &ASmashCharacter::OnHitboxOverlap);

	
	GetWorld()->GetSubsystem<UCameraWorldSubsytem>()->AddFollowActor(this);
}

// Called every frame
void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();
}

// Called to bind functionality to input
void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(EnhancedInputComponent == nullptr) return;
	
	BindInputMoveXAxisAndActions(EnhancedInputComponent);
	
}

float ASmashCharacter::GetOrientX() const
{
	return OrientX;
}

void ASmashCharacter::SetOrientX(float Value)
{
	OrientX = Value;
}

void ASmashCharacter::CreateStateMachine()
{
	StateMachine = NewObject<USmashCharacterStateMachine>(this);
}

void ASmashCharacter::InitStateMachine()
{
	if(StateMachine == nullptr) return;
	StateMachine->Init(this);
}

void ASmashCharacter::TickStateMachine(float DeltaTime) const
{
	if(StateMachine == nullptr) return;
	StateMachine->Tick(DeltaTime);
}

void ASmashCharacter::SetupMappingContextIntoController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(PlayerController == nullptr) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if(Player == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(InputSystem == nullptr) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
	
}

float ASmashCharacter::GetInputMoveX() const
{
	return InputMoveX;
}


void ASmashCharacter::BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if(InputData == nullptr) return;

	if(InputData->InputActionMoveX)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputMoveX);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveX);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputMoveX);
	}

	if (InputData->InputActionMoveXFast)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveXFast,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveXFast);
	}
	
	if (InputData->InputActionJump)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionJump,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnJump);
	}

	if (InputData->InputActionAttack)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionAttack,
				ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnAttackInput);
	}

	if (InputData->InputActionMoveY)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveY,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputMoveY);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveY,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveY);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveY,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputMoveY);
	}
	
}

void ASmashCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
}

void ASmashCharacter::OnInputMoveXFast(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
	InputMoveXFastEvent.Broadcast(InputMoveX);
}

float ASmashCharacter::GetInputMoveY() const
{
	return InputMoveY;
}

void ASmashCharacter::OnInputMoveY(const FInputActionValue& InputActionValue)
{
	InputMoveY = InputActionValue.Get<float>();
}


void ASmashCharacter::OnAttackInput()
{
	AttackEvent.Broadcast();
}

float ASmashCharacter::GetCurrentPercentage()
{
	return CurrentPercentage;
}

void ASmashCharacter::OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASmashCharacter* TargetCharacter = Cast<ASmashCharacter>(OtherActor);
	if(TargetCharacter == nullptr) return;
	TargetCharacter->AddPercentage(AttackDamage);
	
}

void ASmashCharacter::OnJump()
{
	JumpEvent.Broadcast();
}


bool ASmashCharacter::IsFollowable()
{
	return true;
}

FVector ASmashCharacter::GetFollowPosition()
{
	return GetActorLocation();
}


void ASmashCharacter::RotateMeshUsingOrientX() const
{
	
	FRotator Rotation = GetMesh()->GetComponentRotation();
	Rotation.Yaw = -90.f * OrientX;
	GetMesh()->SetRelativeRotation(Rotation);
}


void ASmashCharacter::AddPercentage(int Percentage)
{
	CurrentPercentage += Percentage;
	AMatchGameMode* MatchGameMode = Cast<AMatchGameMode>(GetWorld()->GetAuthGameMode());
	if (MatchGameMode)
	{
		MatchGameMode->UpdateWidgetPercentages();
	}
}

