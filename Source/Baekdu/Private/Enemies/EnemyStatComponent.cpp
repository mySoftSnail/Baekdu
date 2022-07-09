// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyStatComponent.h"

// Sets default values for this component's properties
UEnemyStatComponent::UEnemyStatComponent() :
	Level(1)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeStat();
}

void UEnemyStatComponent::InitializeStat()
{
	if (StatDataTable)
	{
		switch (EnemyType)
		{
		case ENormalEnemyType::ENET_DuskMelee:
			EnemyStatRow = StatDataTable->FindRow<FEnemyStat>(TEXT("DuskMelee"), TEXT(""));
			break;
		}

		if (EnemyStatRow)
		{
			MaxHP = EnemyStatRow->DefaultMaxHP * FMath::Pow(1 + EnemyStatRow->VarianceHP, Level - 1);
			ATK = EnemyStatRow->DefaultATK * FMath::Pow(1 + EnemyStatRow->VarianceATK, Level - 1);
			DEF = EnemyStatRow->DefaultDEF * FMath::Pow(1 + EnemyStatRow->VarianceDEF, Level - 1);
			DropEXP = EnemyStatRow->DefaultDropEXP * FMath::Pow(1 + EnemyStatRow->VarianceDropEXP, Level - 1);
			AttackRange = EnemyStatRow->AttackRange;
			DetectRange = EnemyStatRow->DetectRange;

			CurrentHP = MaxHP;
		}
	}
}

void UEnemyStatComponent::ChangeHP(float HPAmount)
{
	CurrentHP = FMath::Clamp<float>(CurrentHP + HPAmount, 0.0f, MaxHP);
}