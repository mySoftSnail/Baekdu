// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyAIController.h"
#include "CharacterMaster.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	//Interval = 0.1f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	if (ControllingPawn)
	{
		float DetectRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("DetectRange"));
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
		bool bResult = GetWorld()->OverlapMultiByChannel(
			OverlapResults,
			ControllingPawn->GetActorLocation(),
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,
			FCollisionShape::MakeSphere(DetectRadius),
			CollisionQueryParam
		);

		if (bResult)
		{
			for (const auto& Result : OverlapResults)
			{
				ACharacterMaster* Target = Cast<ACharacterMaster>(Result.Actor);
				if (Target)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("TargetPlayerCharacter"), Target);
					break;
				}
			}
		}

		DrawDebugSphere(GetWorld(), ControllingPawn->GetActorLocation(), DetectRadius, 16, FColor::Red, false, 0.2f);
	}
}
