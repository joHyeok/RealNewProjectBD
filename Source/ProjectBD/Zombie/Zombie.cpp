// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AZombie::AZombie()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;

	SetSpeed(WalkSpeed);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AZombie::ProcessSeenPawn);
		PawnSensing->OnHearNoise.AddDynamic(this, &AZombie::ProcessHeardPawn);
	}
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0)
	{
		return 0.0f;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) //PointDamage ó��
	{
		//������
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		UE_LOG(LogClass, Warning, TEXT("Point Damage %s"), *(PointDamageEvent->HitInfo.BoneName.ToString()));

		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("head")) == 0)
		{
			CurrentHP = 0;
		}
		else
		{
			CurrentHP -= DamageAmount;
		}
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID)) //RadialDamage ó��
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)(&DamageEvent);

		UE_LOG(LogClass, Warning, TEXT("Radial Damage %f"), DamageAmount);
	}
	else //��� ������ ó��
	{
		CurrentHP -= DamageAmount;
	}

	if (CurrentHP <= 0)
	{
		CurrentState = EZombieState::Dead;
	}


	UE_LOG(LogClass, Warning, TEXT("%f"), DamageAmount);

	return 0.0f;
}

void AZombie::ProcessSeenPawn(APawn* Pawn)
{
	UE_LOG(LogClass, Warning, TEXT("See %s"), *Pawn->GetName());
	SetState(EZombieState::Chase);
}

void AZombie::ProcessHeardPawn(APawn* Pawn, const FVector& Location, float Volume)
{
	UE_LOG(LogClass, Warning, TEXT("See %s"), *Pawn->GetName());

}

void AZombie::SetState(EZombieState NewState)
{
	CurrentState = NewState;
}

void AZombie::SetSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

	//switch (CurrentState) {
	//case EZombieState::Normal:
	//{
	//	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	//}
	//break;
	//
	//case EZombieState::Battle:
	//case EZombieState::Dead:
	//{
	//	GetCharacterMovement()->MaxWalkSpeed = 0;
	//
	//}
	//break;
	//
	//case EZombieState::Chase:
	//{
	//	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	//}
	//break;
	//}
}
