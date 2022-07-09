// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "Enemies/EnemyNormal.h"
#include "Normal_DuskMelee.generated.h"

/**
 * 
 */
UCLASS()
class BAEKDU_API ANormal_DuskMelee : public AEnemyNormal
{
	GENERATED_BODY()
	
public:
	ANormal_DuskMelee();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	// EnemyNormal
	virtual void NormalAttackTrace() override;

	virtual void PlayNormalAttackMontage() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PlayDeathMontage();

	virtual void PlayHitReactMontage(const FVector CauserLocation) override;

	virtual FName GetHitReactSectionName(const FVector CauserLocation) override;

};
