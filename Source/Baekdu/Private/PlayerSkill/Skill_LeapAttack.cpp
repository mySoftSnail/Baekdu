// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkill/Skill_LeapAttack.h"
#include "CharacterMaster.h"
#include "CharacterSkillComponent.h"
#include "CharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StandardFunctionLibrary.h"
#include "Enemies/EnemyMaster.h"
#include "Kismet/KismetSystemLibrary.h"

ASkill_LeapAttack::ASkill_LeapAttack() :
	bIsLeaping(false),
	MaxInterpSpeed(5.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASkill_LeapAttack::BeginPlay()
{
	Super::BeginPlay();

}

void ASkill_LeapAttack::LeapAttackTrace()
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
			AEnemyMaster* Enemy = Cast<AEnemyMaster>(HitResult.Actor);
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

void ASkill_LeapAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACharacterMaster* Character = GetPlayerCharacter();

	if (Character && bIsLeaping)
	{
		const float MoveMultiplier = Character->GetAnimInstance()->GetCurveValue(TEXT("Move"));

		if (MoveMultiplier > 0.f)
		{
			const float InterpSpeed = MaxInterpSpeed * MoveMultiplier;
			const FVector CharacterLocation = Character->GetActorLocation();
			const FVector NewLocation = FMath::VInterpTo(CharacterLocation, Destination, DeltaTime, InterpSpeed);
			Character->SetActorLocation(NewLocation);
		}
	}
}

void ASkill_LeapAttack::RunSkill()
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

			// 캐릭터를 커서방향으로 주어진 거리만큼 이동하도록 목적지 설정
			const FVector CharacterLocation = Character->GetActorLocation();
			const FVector DirectionToCursor = CursorLocation - CharacterLocation;
			const float DistanceToCursor = DirectionToCursor.Size();
			const FVector UnitDirection = DirectionToCursor.GetSafeNormal();

			if (DistanceToCursor <= SI->Range) // 커서까지의 거리가 최대거리보다 작다면
			{
				Destination = CursorLocation;
			}
			else
			{
				Destination = CharacterLocation + UnitDirection * SI->Range;
			}
			Destination.Z = CharacterLocation.Z; // Destination.Z를 캐릭터의 높이와 동일하게

			// LeapAttack start 이펙트
			if (SI->SkillEffect1)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					SI->SkillEffect1,
					CharacterLocation,
					Character->GetActorRotation(),
					true);
			}

			bIsLeaping = true;
			Character->GetAnimInstance()->Montage_Play(SI->SkillMontage1);
			//DrawDebugDirectionalArrow(GetWorld(), PlayerCharacter->GetActorLocation(), CursorLocation, 80.f, FColor::Blue, true, 60.f);
		}
	}
}