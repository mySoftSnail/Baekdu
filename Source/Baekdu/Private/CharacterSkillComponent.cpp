// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillComponent.h"
#include "CharacterMaster.h"
#include "PlayerSkill/CharacterSkillMaster.h"
#include "CharacterStatComponent.h"
#include "CharacterPlayerController.h"

// Sets default values for this component's properties
UCharacterSkillComponent::UCharacterSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	UE_LOG(LogTemp, Warning, TEXT("ACharacterSkillComponent::ACharacterSkillComponent"));
}

// Called when the game starts
void UCharacterSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ACharacterSkillComponent::BeginPlay"));

	PlayerCharacter = Cast<ACharacterMaster>(GetOwner());

	InitSkillSet();
}

void UCharacterSkillComponent::ResetCooldown(const uint8 Index)
{
	SkillSet[Index]->SetIsCooldown(false);
}

void UCharacterSkillComponent::InitSkillSet()
{
	if (SkillDataTable)
	{
		TArray<FSkillInfo*> AllRows;
		SkillDataTable->GetAllRows<FSkillInfo>(TEXT(""), AllRows);

		for (auto& Row : AllRows)
		{
			if (Row->CharacterType == PlayerCharacter->GetCharacterType())
			{
				if (::IsValid(Row->SkillClass))
				{
					ACharacterSkillMaster* Skill = GetWorld()->SpawnActor<ACharacterSkillMaster>(Row->SkillClass, PlayerCharacter->GetActorLocation(), FRotator(0.f));
					Skill->SetSkillInfo(Row);
					Skill->SetSkillType(Row->SkillType);

					if (Skill)
					{
						SkillInfos.Add(*Row);
						SkillSet.Add(Skill);
					}
				}
			}
		}
	}
}

/*
* 여기서 보면 SkillInfos를 통해 Skill 정보를 얻고 있는데, 이렇게 하지말고, 위에서 
* SKill을 Spawn할 때, 내부에 SkillInfo를 주입해준다. 그리고 그거를 이용하는 것이지.
* SkillInfos 배열은 굳이 안써도 될 듯. 다음 줄의 코드는 예시
* SkillSet[KeyIndex]->GetSkillInfo()->Resource;
*/
void UCharacterSkillComponent::UpdateMP(const uint8 KeyIndex)
{
	// 캐릭터 마나 감소
	float NewCurrentMP = FMath::Clamp<float>(
		PlayerCharacter->GetStatComponent()->GetCurrentMP() - SkillInfos[KeyIndex].Resource,
		0.0f,
		PlayerCharacter->GetStatComponent()->GetMaxMP());

	PlayerCharacter->GetStatComponent()->SetCurrentMP(NewCurrentMP);

	// 캐릭터 MP Bar에 브로드캐스트
	PlayerCharacter->OnCurrentMPChangedDelegate.Broadcast();

	// 위의 코드 개선 예정
}

void UCharacterSkillComponent::RunCooldown(const uint8 KeyIndex)
{
	ACharacterPlayerController* OwnerController = Cast<ACharacterPlayerController>(PlayerCharacter->GetController());

	OwnerController->ShowCooldown(KeyIndex);
	SkillSet[KeyIndex]->SetIsCooldown(true);

	FTimerHandle ResetTimerHandle;
	FTimerDelegate ResetCooldownDelegate;
	ResetCooldownDelegate.BindUFunction(this, FName("ResetCooldown"), KeyIndex);
	GetWorld()->GetTimerManager().SetTimer(
		ResetTimerHandle,
		ResetCooldownDelegate,
		SkillInfos[KeyIndex].Cooldown,
		false);
}

void UCharacterSkillComponent::RunSkill(const uint8 KeyIndex)
{
	if (SkillSet.Num() > 0)
	{
		if (SkillSet[KeyIndex] && SkillInfos.IsValidIndex((KeyIndex)))
		{
			UpdateMP(KeyIndex);
			RunCooldown(KeyIndex);
			SkillSet[KeyIndex]->RunSkill();
		}
	}
}

ESkillType UCharacterSkillComponent::GetSkillTypeFromIndex(const uint8 Index) const 
{
	return SkillSet[Index]->GetSkillType();
}

bool UCharacterSkillComponent::GetSkillCooldownFromIndex(const uint8 Index) const
{
	return SkillSet[Index]->GetIsCooldown();
}

float UCharacterSkillComponent::GetSkillResourceFromIndex(const uint8 Index) const
{
	return SkillInfos[Index].Resource;
}
