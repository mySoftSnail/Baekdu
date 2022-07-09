// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyNormal.h"
#include "Components/WidgetComponent.h"
#include "UI/UI_EnemyHPBarMini.h"

AEnemyNormal::AEnemyNormal()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
}

void AEnemyNormal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyNormal::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitNormalEnemyAttribute();
}

void AEnemyNormal::NormalAttackTrace_BC()
{
	NormalAttackTrace();
}

void AEnemyNormal::NormalAttackTrace()
{
}

void AEnemyNormal::PlayNormalAttackMontage()
{
}

void AEnemyNormal::NormalAttackEnd()
{
	OnNormalAttackEndDelegate.Broadcast();
}

void AEnemyNormal::BeginPlay()
{
	Super::BeginPlay();

	// Enemy HP Bar¸¦ ½ºÅÈ ÄÄÆ÷³ÍÆ® µ¨¸®°ÔÀÌÆ®¿¡ ¹ÙÀÎµù
	if (WidgetComponent->GetUserWidgetObject())
	{
		Cast<UUI_EnemyHPBarMini>(WidgetComponent->GetUserWidgetObject())->BindDelegate(GetStatComponent());
	}
}

void AEnemyNormal::InitNormalEnemyAttribute()
{
	UDataTable* DataTable = GetAttributeDataTable();
	if (DataTable)
	{
		FEnemyAttribute* RowData = nullptr;

		switch (NormalEnemyType)
		{
		case ENormalEnemyType::ENET_DuskMelee:
			RowData = DataTable->FindRow<FEnemyAttribute>(TEXT("DuskMelee"), TEXT(""));
			SetEnemyAttributeRow(RowData);
			break;
		}

		if (RowData)
		{
			UBehaviorTree* BT = RowData->BehaviorTree;
			SetBehaviorTree(BT);
			NormalAttackMontage = RowData->NormalAttackMontage;
			SetDeathMontage(RowData->DeathMontage);
			SetHitReactMontage(RowData->HitReactMontage);
		}
	}
}