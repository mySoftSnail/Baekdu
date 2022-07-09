// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyMaster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/EnemyAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Enemies/EnemyStatComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "CharacterMaster.h"

// Sets default values
AEnemyMaster::AEnemyMaster() :
	bIsDead(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyStatComponent = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("EnemyStatComponent"));
}

// Called when the game starts or when spawned
void AEnemyMaster::BeginPlay()
{
	Super::BeginPlay();

	EnemyController = Cast<AEnemyAIController>(GetController());
	
	EnemyController->GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), EnemyStatComponent->GetAttackRange());
	EnemyController->GetBlackboardComponent()->SetValueAsFloat(TEXT("DetectRange"), EnemyStatComponent->GetDetectRange());

	// 비헤이비어 트리 실행
	if (EnemyController && BehaviorTree)
	{
		EnemyController->RunBehaviorTree(BehaviorTree);
	}
}

void AEnemyMaster::Dead(UAnimMontage* Montage)
{
	GetMesh()->GetAnimInstance()->Montage_Play(Montage);
	// 적 오브젝트 파괴 처리
}

void AEnemyMaster::PlayDeathMontage()
{
}

void AEnemyMaster::FinishDeath()
{
	Destroy();
}

void AEnemyMaster::PlayHitReactMontage(const FVector CauserLocation)
{
}

FName AEnemyMaster::GetHitReactSectionName(const FVector CauserLocation)
{
	return FName();
}

// Called every frame
void AEnemyMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AEnemyMaster::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 피격 방향에 따른 Hit React Montage 재생
	// HitReactMontage를 재생하지 않을 조건을 밑에 추가할것
	//if (CharacterState == ECharacterState::ECS_Jumping ||
	//	CharacterState == ECharacterState::ECS_NormalAttacking ||
	//	CharacterState == ECharacterState::ECS_SkillAttacking)
	{
	}
	//else
	{
		PlayHitReactMontage(DamageCauser->GetActorLocation());
	}

	// DEF 값만큼 데미지 감쇠
	float CalculatedDamage = FMath::Clamp<float>(DamageAmount - EnemyStatComponent->GetDEF(), 0.f, DamageAmount);

	// HP 차감
	float NewCurrentHP = FMath::Clamp<float>(EnemyStatComponent->GetCurrentHP() - CalculatedDamage, 0.f, EnemyStatComponent->GetMaxHP());
	EnemyStatComponent->SetCurrentHP(NewCurrentHP);

	// HP 변경을 UI에 브로드캐스트
	EnemyStatComponent->OnEnemyCurrentHPChanged.Broadcast();

	if (NewCurrentHP <= 0.f && !bIsDead)
	{
		// 죽음
		//Dead();

		// Death 몽타주 재생 전의 처리들
		bIsDead = true;
		SetActorEnableCollision(false);
		SetCanBeDamaged(false);
		EnemyController->StopAI();

		// 플레이어에게 EXP 제공
		float DropEXP = EnemyStatComponent->GetDropEXP();
		ACharacterMaster* Player = Cast<ACharacterMaster>(DamageCauser);
		Player->AddEXP(DropEXP);

		PlayDeathMontage();
	}

	return CalculatedDamage;
}

void AEnemyMaster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}
