// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "CharacterSkillMaster.h"
#include "Skill_BladeStorm.generated.h"

/**
 * 
 */
UCLASS()
class BAEKDU_API ASkill_BladeStorm : public ACharacterSkillMaster
{
	GENERATED_BODY()
public:
	ASkill_BladeStorm();

	virtual void RunSkill() override;

protected:
	virtual void BeginPlay() override;

	void SpawnBladeStormProjectile();

public:
	void DoDamage(AActor* Actor);
};
