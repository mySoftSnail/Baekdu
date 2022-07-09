// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Normal_DuskMelee.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Enemies/EnemyStatComponent.h"
#include "CharacterMaster.h"

ANormal_DuskMelee::ANormal_DuskMelee()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ANormal_DuskMelee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANormal_DuskMelee::NormalAttackTrace()
{
	Super::NormalAttackTrace();

	TArray<FHitResult> HitResults;
	ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	bool bResult = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		GetActorLocation() + GetActorForwardVector() * 100.f,
		GetActorLocation() + GetActorForwardVector() * 110.f,
		120.f,
		TraceTypeQuery,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
	);

	if (bResult)
	{
		for (auto& HitResult : HitResults)
		{
			FDamageEvent DamageEvent{};
			ACharacterMaster* Character = Cast<ACharacterMaster>(HitResult.Actor);
			if (Character)
			{
				// ATK 만큼 데미지 전달
				Character->TakeDamage(GetStatComponent()->GetATK(), DamageEvent, GetController(), this);
			}
		}
	}
}

void ANormal_DuskMelee::PlayNormalAttackMontage()
{
	Super::PlayNormalAttackMontage();

	UAnimMontage* Montage = GetNormalAttackMontage();

	if (Montage)
	{
		FName SectionName;
		int32 RandomNumber = FMath::RandRange(0, 3);
		switch (RandomNumber)
		{
		case 0:
			SectionName = TEXT("Attack_A");
			break;
		case 1:
			SectionName = TEXT("Attack_B");
			break;
		case 2:
			SectionName = TEXT("Attack_C");
			break;
		case 3:
			SectionName = TEXT("Attack_D");
			break;
		}

		GetAnimInstance()->Montage_Play(Montage);
		GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
	}
}

void ANormal_DuskMelee::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ANormal_DuskMelee::BeginPlay()
{
	Super::BeginPlay();

}

void ANormal_DuskMelee::PlayDeathMontage()
{
	UAnimMontage* Montage = GetDeathMontage();

	if (Montage)
	{
		FName SectionName;
		/*int32 RandomNumber = FMath::RandRange(0, 4);
		switch (RandomNumber)
		{
		case 0:
			SectionName = TEXT("DeathA");
			break;
		case 1:
			SectionName = TEXT("DeathB");
			break;
		case 2:
			SectionName = TEXT("DeathC");
			break;
		case 3:
			SectionName = TEXT("DeathD");
			break;
		case 4:
			SectionName = TEXT("DeathE");
			break;
		}*/
		SectionName = TEXT("DeathA");
		GetAnimInstance()->Montage_Play(Montage);
		GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
	}
}

void ANormal_DuskMelee::PlayHitReactMontage(const FVector CauserLocation)
{
	FName SectionName = GetHitReactSectionName(CauserLocation);

	if (GetAnimInstance() && GetHitReactMontage())
	{
		GetAnimInstance()->Montage_Play(GetHitReactMontage());
		GetAnimInstance()->Montage_JumpToSection(SectionName, GetHitReactMontage());
	}
}

FName ANormal_DuskMelee::GetHitReactSectionName(const FVector CauserLocation)
{
	FName SectionName;
	FVector CharacterToEnemy = CauserLocation - GetActorLocation();
	CharacterToEnemy.Normalize();
	float ForwardDotResult = CharacterToEnemy | GetActorForwardVector();
	FVector CrossResult = GetActorForwardVector() ^ CharacterToEnemy;
	const float cos45 = FMath::Cos(PI / 4);

	if (ForwardDotResult >= cos45 && ForwardDotResult <= 1.f)
	{
		SectionName = TEXT("HitReact_Front");
	}
	else if (ForwardDotResult <= -cos45 && ForwardDotResult >= -1.f)
	{
		SectionName = TEXT("HitReact_Back");
	}
	else
	{
		if (CrossResult.Z > 0)
		{
			SectionName = TEXT("HitReact_Right");
		}
		else if (CrossResult.Z < 0)
		{
			SectionName = TEXT("HitReact_Left");
		}
		else
		{
			SectionName = TEXT("HitReact_Front");
		}
	}

	return SectionName;
}
