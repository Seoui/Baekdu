// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatComponent.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent() :
	Level(1)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	UE_LOG(LogTemp, Warning, TEXT("UCharacterStatComponent::UCharacterStatComponent"));
}

// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("UCharacterStatComponent::BeginPlay"));

	InitializeStat();
}

void UCharacterStatComponent::InitializeStat()
{
	if (StatDataTable)
	{
		switch (CharacterType)
		{
		case ECharacterType::ECT_Greystone:
			CharacterStatRow = StatDataTable->FindRow<FCharacterStat>(TEXT("Greystone"), TEXT(""));
			break;
		case ECharacterType::ECT_Sparrow:
			CharacterStatRow = StatDataTable->FindRow<FCharacterStat>(TEXT("Sparrow"), TEXT(""));
			break;
		}

		if (CharacterStatRow)
		{
			MaxHP = CharacterStatRow->DefaultMaxHP;
			MaxMP = CharacterStatRow->DefaultMaxMP;
			ATK = CharacterStatRow->DefaultATK;
			DEF = CharacterStatRow->DefaultDEF;
			NextEXP = CharacterStatRow->DefaultNextEXP;

			CurrentHP = MaxHP;
			CurrentMP = MaxMP;
			CurrentEXP = 0.f;
			CriticalRate = 0.f;
			CooldownReduction = 0.f;
		}
	}
}

void UCharacterStatComponent::ChangeHP(float HPAmount)
{
	CurrentHP = FMath::Clamp<float>(CurrentHP + HPAmount, 0.0f, MaxHP);
}

void UCharacterStatComponent::ChangeMP(float MPAmount)
{
	CurrentMP = FMath::Clamp<float>(CurrentMP + MPAmount, 0.0f, MaxMP);
}

void UCharacterStatComponent::LevelUp()
{
	++Level;
	MaxHP = MaxHP * (1 + CharacterStatRow->VarianceHP);
	MaxMP = MaxMP * (1 + CharacterStatRow->VarianceMP);
	CurrentHP = MaxHP;
	CurrentMP = MaxMP;
	ATK = ATK * (1 + CharacterStatRow->VarianceATK);
	DEF = DEF * (1 + CharacterStatRow->VarianceDEF);
	NextEXP = NextEXP * (1 + CharacterStatRow->VarianceEXP);
	//CurrentEXP = 0.f;
}

void UCharacterStatComponent::AddEXP(float EXPAmount)
{
	float LocalEXP = CurrentEXP + EXPAmount;

	if (LocalEXP >= NextEXP)
	{
		while (LocalEXP >= NextEXP)
		{
			CurrentEXP = LocalEXP - NextEXP;
			LocalEXP = CurrentEXP;
			LevelUp();
		}
	}
	else
	{
		CurrentEXP = LocalEXP;
	}
}
