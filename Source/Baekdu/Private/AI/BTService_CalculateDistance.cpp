// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CalculateDistance.h"
#include "CharacterMaster.h"
#include "Enemies/EnemyMaster.h"
#include "Enemies/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CalculateDistance::UBTService_CalculateDistance()
{
	NodeName = TEXT("CalculateDistance");
	Interval = 0.1f;
}

void UBTService_CalculateDistance::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ControllingPawn = Cast<AEnemyMaster>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<ACharacterMaster>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetPlayerCharacter")));
	}

	const float Distance = ControllingPawn->GetDistanceTo(PlayerCharacter);
	UE_LOG(LogClass, Warning, TEXT("%s: %f"), *OwnerComp.GetAIOwner()->GetName(), Distance);
	//UE_LOG(LogClass, Warning, TEXT("%s: %d"), *OwnerComp.GetAIOwner()->GetName(), OwnerComp.GetBlackboardComponent());

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("Distance"), Distance);
}
