// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_EnemyHPBarMini.h"
#include "Enemies/EnemyStatComponent.h"

void UUI_EnemyHPBarMini::BindDelegate(UEnemyStatComponent* Component)
{
	if (Component)
	{
		StatComponent = Component;
		StatComponent->OnEnemyCurrentHPChanged.AddDynamic(this, &UUI_EnemyHPBarMini::UpdateHPBar);
		StatComponent->OnEnemyCurrentHPChanged.Broadcast();
	}
}
