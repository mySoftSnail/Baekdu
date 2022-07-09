// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "Blueprint/UserWidget.h"
#include "UI_EnemyHPBarMini.generated.h"

class UEnemyStatComponent;
/**
 * 
 */
UCLASS()
class BAEKDU_API UUI_EnemyHPBarMini : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindDelegate(UEnemyStatComponent* Component);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHPBar();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UEnemyStatComponent> StatComponent;
};
