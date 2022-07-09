// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyAIController.h"
#include "Enemies/EnemyMaster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterMaster.h"

AEnemyAIController::AEnemyAIController() 
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	check(BlackboardComponent);
}

void AEnemyAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
}

void AEnemyAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	AEnemyMaster* Enemy = Cast<AEnemyMaster>(GetPawn());
	if (Enemy)
	{
		if (Enemy->GetBehaviorTree())
		{
			BlackboardComponent->InitializeBlackboard(*(Enemy->GetBehaviorTree()->BlackboardAsset));
		}
	}

	/* ���� ���� 
	auto PlayerCharacter = Cast<ACharacterMaster>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	BlackboardComponent->SetValueAsObject(TEXT("TargetPlayerCharacter"), PlayerCharacter);*/
}
