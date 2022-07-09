// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkill/Skill_BladeStorm.h"
#include "CharacterMaster.h"
#include "CharacterSkillComponent.h"
#include "CharacterStatComponent.h"
#include "Projectile_BladeStorm.h"
#include "StandardFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ASkill_BladeStorm::ASkill_BladeStorm()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASkill_BladeStorm::RunSkill()
{
	ACharacterMaster* Character = GetPlayerCharacter();
	FSkillInfo* SI = GetSkillInfo();

	if (Character)
	{
		if (Character->GetCharacterState() != ECharacterState::ECS_Jumping)
		{
			// 캐릭터 상태를 설정 (ECS_SkillAiming -> ECS_SkillAttacking)
			Character->SetCharacterState(ECharacterState::ECS_SkillAttacking);

			// 캐릭터를 커서방향으로 회전
			const FVector CursorLocation = Character->RotateToCursor();

			// 몽타주 재생
			Character->GetAnimInstance()->Montage_Play(SI->SkillMontage1);

			SpawnBladeStormProjectile();
		}
	}
}

void ASkill_BladeStorm::BeginPlay()
{
	Super::BeginPlay();
}

void ASkill_BladeStorm::SpawnBladeStormProjectile()
{
	ACharacterMaster* Character = GetPlayerCharacter();
	FSkillInfo* SI = GetSkillInfo();

	FString ProjectilePath(TEXT("Blueprint'/Game/_Baekdu/02_Skill/Projectile/Projectile_BladeStormBP.Projectile_BladeStormBP_C'"));
	auto ProjectileClass = StaticLoadClass(AProjectile_BladeStorm::StaticClass(), nullptr, *ProjectilePath);

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Owner = this;
	auto Projectile = GetWorld()->SpawnActor<AProjectile_BladeStorm>(
		ProjectileClass,
		Character->GetActorLocation(),
		Character->GetActorRotation(),
		ActorSpawnParameters);
	const float SkillRange = SI->Range;
	const float SkillSpeed = Projectile->GetVelocity().Size();
	Projectile->SetLifeSpan(SkillRange / SkillSpeed);
}

void ASkill_BladeStorm::DoDamage(AActor* Actor)
{
	ACharacterMaster* Character = GetPlayerCharacter();
	FSkillInfo* SI = GetSkillInfo();

	const float ATK = Character->GetStatComponent()->GetATK();
	const float Damage = UStandardFunctionLibrary::CalculateDamage(ATK, SI->Coefficient, SI->Damage);
	const FDamageEvent DamageEvent;
	Actor->TakeDamage(Damage, DamageEvent, Character->GetController(), Character);
}
