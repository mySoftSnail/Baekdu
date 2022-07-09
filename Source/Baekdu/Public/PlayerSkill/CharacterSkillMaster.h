// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "SkillType.h"
#include "GameFramework/Actor.h"
#include "CharacterSkillComponent.h"
#include "CharacterSkillMaster.generated.h"

class ACharacterMaster;

/**
 *
 */
UCLASS()
class BAEKDU_API ACharacterSkillMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterSkillMaster();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void RunSkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent;

	// protected·Î ¹Ù²Ü ¿¹Á¤
	UPROPERTY(VisibleAnywhere)
	ACharacterMaster* PlayerCharacter;
	// protected·Î ¹Ù²Ü ¿¹Á¤
	FSkillInfo* SkillInfo;

	UPROPERTY()
	ESkillType SkillType;

	bool bIsCooldown;

public:
	FORCEINLINE ESkillType GetSkillType() const { return SkillType; }
	FORCEINLINE void SetSkillType(ESkillType NewSkillType) { SkillType = NewSkillType; }

	FORCEINLINE bool GetIsCooldown() const { return bIsCooldown; }
	FORCEINLINE void SetIsCooldown(bool IsCooldown) { bIsCooldown = IsCooldown; }

	FORCEINLINE ACharacterMaster* GetPlayerCharacter() const { return PlayerCharacter; }

	FORCEINLINE void SetSkillInfo(FSkillInfo* NewSkillInfo) { SkillInfo = NewSkillInfo; }
	FORCEINLINE FSkillInfo* GetSkillInfo() const { return SkillInfo; }
};


