// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_AbortMoveTo.h"
#include "Enemies/EnemyAIController.h"
#include "Enemies/EnemyMaster.h"
#include "CharacterMaster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_AbortMoveTo::UBTService_AbortMoveTo()
{
	NodeName = TEXT("AbortMoveTo");
	//Interval = 0.1f;
}

void UBTService_AbortMoveTo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetBlackboardComponent())
	{
		//const float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"));
		const float Distance = Cast<AEnemyMaster>(OwnerComp.GetAIOwner()->GetPawn())->GetDistanceTo(
			Cast<ACharacterMaster>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetPlayerCharacter"))));
		const float AttackRange = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("AttackRange"));

		if (Distance <= AttackRange)
		{
			if (OwnerComp.GetAIOwner())
			{
				OwnerComp.GetAIOwner()->StopMovement();
			}
		}
	}
}
