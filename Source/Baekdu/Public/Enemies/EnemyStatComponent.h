// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "EnemyType.h"
#include "EnemyStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyCurrentHPChanged);

USTRUCT(BlueprintType)
struct FEnemyStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultMaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultDEF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultDropEXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectRange;

	// Variance : ¡ı∞®¿≤
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VarianceHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VarianceATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VarianceDEF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VarianceDropEXP;
};

/**
 *
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAEKDU_API UEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "EnemyStatComponent")
	FOnEnemyCurrentHPChanged OnEnemyCurrentHPChanged;

public:	
	// Sets default values for this component's properties
	UEnemyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitializeStat();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyStatComponent", meta = (AllowPrivateAccess = "true"))
	UDataTable* StatDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyStatComponent", meta = (AllowPrivateAccess = "true"))
	ENormalEnemyType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStatComponent", meta = (AllowPrivateAccess = "true"))
	int32 Level;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyStatComponent", meta = (AllowPrivateAccess = "true"))
	float MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyStatComponent", meta = (AllowPrivateAccess = "true"))
	float CurrentHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyStatComponent", meta = (AllowPrivateAccess = "true"))
	float ATK;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyStatComponent", meta = (AllowPrivateAccess = "true"))
	float DEF;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyStatComponent", meta = (AllowPrivateAccess = "true"))
	float DropEXP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyStatComponent", meta = (AllowPrivateAccess = "true"))
	float AttackRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyStatComponent", meta = (AllowPrivateAccess = "true"))
	float DetectRange;

	FEnemyStat* EnemyStatRow;

public:	
	FORCEINLINE float GetATK() const { return ATK; }
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetDEF() const { return DEF; }
	FORCEINLINE int32 GetLevel() const { return Level; }
	FORCEINLINE float GetDropEXP() const { return DropEXP; }
	FORCEINLINE float GetAttackRange() const { return AttackRange; }
	FORCEINLINE float GetDetectRange() const { return DetectRange; }

	FORCEINLINE void SetCurrentHP(float NewCurrentHP) { CurrentHP = NewCurrentHP; }
	FORCEINLINE void SetMaxHP(float NewMaxHP) { MaxHP = NewMaxHP; }

	void ChangeHP(float HPAmount);
};
