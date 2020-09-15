// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Zombie.h"
#include "BehaviorTree/BehaviorTree.h"

AZombieAIController::AZombieAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
}

void AZombieAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	//AIController�� ZOmbie�� ������ �׶� BT������ �ʱ�ȭ�ؼ� ���
	AZombie* Zombie = Cast<AZombie>(InPawn);
	if (Zombie) {
		if (Zombie->ZombieBT && Zombie->ZombieBT->BlackboardAsset) {
			//���� Ÿ���� �ϳ��� ���۷����� �ϳ��� �����Ͷ� ������ �ִ´�
			BBComponent->InitializeBlackboard(*(Zombie->ZombieBT->BlackboardAsset));
			BTComponent->StartTree(*(Zombie->ZombieBT));
		}
	}
}

void AZombieAIController::OnUnPossess()
{
	Super::OnUnPossess();
}