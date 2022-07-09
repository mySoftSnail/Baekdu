// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "EnemyMaster.generated.h"

class UBehaviorTree;
class AEnemyAIController;
class UEnemyStatComponent;

USTRUCT(BlueprintType)
struct FEnemyAttribute : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* NormalAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DeathMontage;
};

/**
 *
 */
UCLASS()
class BAEKDU_API AEnemyMaster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyMaster();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Dead(UAnimMontage* Montage);

	virtual void PlayDeathMontage(); 

	UFUNCTION(BlueprintCallable)
	void FinishDeath();

	virtual void PlayHitReactMontage(const FVector CauserLocation);

	virtual FName GetHitReactSectionName(const FVector CauserLocation);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyMaster", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyMaster", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyMaster", meta = (AllowPrivateAccess = "true"))
	AEnemyAIController* EnemyController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyMaster", meta = (AllowPrivateAccess = "true"))
	UDataTable* AttributeDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyMaster|Component", meta = (AllowPrivateAccess = "true"))
	UEnemyStatComponent* EnemyStatComponent;

	FEnemyAttribute* AttributeRow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyMaster|Animation", meta = (AllowPrivateAccess = "true"))
	UAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyMaster|Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyMaster|Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactMontage;	

	UPROPERTY()
	bool bTestVariable;

public:
	FORCEINLINE bool GetIsDead() const { return bIsDead; }

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	FORCEINLINE void SetBehaviorTree(UBehaviorTree* BT) { BehaviorTree = BT; }

	FORCEINLINE UEnemyStatComponent* GetStatComponent() const { return EnemyStatComponent; }
	FORCEINLINE UDataTable* GetAttributeDataTable() const { return AttributeDataTable; }

	FORCEINLINE FEnemyAttribute* GetEnemyAttributeRow() const { return AttributeRow; }
	FORCEINLINE void SetEnemyAttributeRow(FEnemyAttribute* RowData) { AttributeRow = RowData; }

	FORCEINLINE UAnimInstance* GetAnimInstance() const { return GetMesh()->GetAnimInstance(); }

	FORCEINLINE UAnimMontage* GetDeathMontage() const { return DeathMontage; }

	FORCEINLINE void SetDeathMontage(UAnimMontage* Montage) { DeathMontage = Montage; }

	FORCEINLINE UAnimMontage* GetHitReactMontage() const { return HitReactMontage; }

	FORCEINLINE void SetHitReactMontage(UAnimMontage* Montage) { HitReactMontage = Montage; }
};
