// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkill/Skill_FireAura.h"
#include "CharacterMaster.h"
#include "CharacterSkillComponent.h"
#include "CharacterStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "StandardFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"

ASkill_FireAura::ASkill_FireAura() :
	SkillTickDuration(1.f)
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASkill_FireAura::BeginPlay()
{
	Super::BeginPlay();
}

void ASkill_FireAura::RunSkill()
{
	ACharacterMaster* Character = GetPlayerCharacter();
	FSkillInfo* SI = GetSkillInfo();

	if (Character)
	{
		if (Character->GetCharacterState() != ECharacterState::ECS_Jumping)
		{
			// ĳ���� ���¸� ���� (ECS_SkillAiming -> ECS_SkillAttacking)
			Character->SetCharacterState(ECharacterState::ECS_SkillAttacking);

			// FireAura ����Ʈ
			FireAuraFX = UGameplayStatics::SpawnEmitterAttached(SI->SkillEffect1, Character->GetMesh(), FName(),
				Character->GetActorLocation(), FRotator(0.f), EAttachLocation::KeepWorldPosition);

			// FireAura �ִϸ��̼�
			Character->GetAnimInstance()->Montage_Play(SI->SkillMontage1);

			SetSkillTimer();
		}
	}
}

// FireAuraTrace�� �Լ� �̸� ���� ����
void ASkill_FireAura::DoDamage()
{
	ACharacterMaster* Character = GetPlayerCharacter();
	FSkillInfo* SI = GetSkillInfo();

	const FVector CharacterLocation = Character->GetActorLocation();
	ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	TArray<FHitResult> OutHits;

	bool bResult = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		CharacterLocation,
		CharacterLocation,
		250.f,
		TraceTypeQuery,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHits,
		true
	);

	if (bResult)
	{
		for (auto& HitResult : OutHits)
		{
			ACharacter* Enemy = Cast<ACharacter>(HitResult.Actor);
			if (Enemy)
			{
				const float ATK = Character->GetStatComponent()->GetATK();
				const float Damage = UStandardFunctionLibrary::CalculateDamage(ATK, SI->Coefficient, SI->Damage);
				const FDamageEvent DamageEvent;
				Enemy->TakeDamage(Damage, DamageEvent, Character->GetController(), Character);
			}
		}
	}
}

void ASkill_FireAura::ClearSkillTimer(FTimerHandle DoDamageTimerHandle)
{
	GetWorld()->GetTimerManager().ClearTimer(DoDamageTimerHandle);
	if (FireAuraFX)
	{
		FireAuraFX->Deactivate();
	}
}

void ASkill_FireAura::SetSkillTimer()
{
	FSkillInfo* SI = GetSkillInfo();

	// Ÿ�̸� ���� (��ų ������ ƽ)
	FTimerHandle DoDamageTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DoDamageTimerHandle, this, &ASkill_FireAura::DoDamage, SkillTickDuration, true);

	// Ÿ�̸� ���� (��ų ��ü)
	FTimerHandle FireAuraTimerHandle;
	FTimerDelegate FireAuraTimerDelegate;
	FireAuraTimerDelegate.BindUFunction(this, FName("ClearSkillTimer"), DoDamageTimerHandle, FireAuraFX);
	GetWorld()->GetTimerManager().SetTimer(FireAuraTimerHandle, FireAuraTimerDelegate, SI->Duration, false);
}
