// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "CharacterSkillMaster.h"
#include "Skill_LeapAttack.generated.h"

/**
 * 
 */
UCLASS()
class BAEKDU_API ASkill_LeapAttack : public ACharacterSkillMaster
{
	GENERATED_BODY()
	
public:
	ASkill_LeapAttack();

	virtual void Tick(float DeltaTime) override;

	// 입력 파라미터 제거함.
	virtual void RunSkill() override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
	void LeapAttackTrace();

private:
	UPROPERTY(VisibleAnywhere)
	FVector Destination;

	bool bIsLeaping;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MaxInterpSpeed;
};
