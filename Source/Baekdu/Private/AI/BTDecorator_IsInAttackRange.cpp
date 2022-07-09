// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTDecorator_IsInAttackRange.h"
#include "Enemies/EnemyMaster.h"
#include "Enemies/EnemyAIController.h"
#include "CharacterMaster.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{

}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	if (OwnerComp.GetBlackboardComponent())
	{
		AEnemyMaster* ControllingPawn = Cast<AEnemyMaster>(OwnerComp.GetAIOwner()->GetPawn());
		ACharacterMaster* PlayerPawn = Cast<ACharacterMaster>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetPlayerCharacter")));

		const float Distance = ControllingPawn->GetDistanceTo(PlayerPawn);
		//const float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"));
		const float AttackRange = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("AttackRange"));

		if (Distance <= AttackRange)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}
