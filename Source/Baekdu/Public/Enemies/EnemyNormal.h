// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "Enemies/EnemyMaster.h"
#include "EnemyType.h"
#include "EnemyNormal.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNormalAttackEndDelegate);

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class BAEKDU_API AEnemyNormal : public AEnemyMaster
{
	GENERATED_BODY()

public:
	AEnemyNormal();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	// EnemyMaster�� �ø� ����
	virtual void NormalAttackTrace();
	// EnemyMaster�� �ø� ����
	virtual void PlayNormalAttackMontage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void InitNormalEnemyAttribute();

	// EnemyMaster�� �ø� ����
	UFUNCTION(BlueprintCallable)
	void NormalAttackEnd();
	// EnemyMaster�� �ø� ����
	UFUNCTION(BlueprintCallable)
	void NormalAttackTrace_BC();

public:
	FOnNormalAttackEndDelegate OnNormalAttackEndDelegate;

private:
	// EnemyMaster�� �ø� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyNormal|Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* NormalAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyNormal", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyNormal", meta = (AllowPrivateAccess = "true"))
	ENormalEnemyType NormalEnemyType;

public:
	FORCEINLINE UAnimMontage* GetNormalAttackMontage() const { return NormalAttackMontage; }
	FORCEINLINE void SetNormalAttackMontage(UAnimMontage* Montage) { NormalAttackMontage = Montage; }
};
