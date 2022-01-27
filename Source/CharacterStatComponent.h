// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "CharacterMaster.h"
#include "CharacterStatComponent.generated.h"

class UDataTable;

USTRUCT(BlueprintType)
struct FCharacterStat : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultMaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultMaxMP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultDEF;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultNextEXP;

	// Variance : ¡ı∞®¿≤
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VarianceHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VarianceMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VarianceATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VarianceDEF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VarianceEXP;
};

/**
 *
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAEKDU_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitializeStat();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterStatComponent", meta = (AllowPrivateAccess = "true"))
	UDataTable* StatDataTable;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterStatComponent", meta = (AllowPrivateAccess = "true"))
	ECharacterType CharacterType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatComponent", meta = (AllowPrivateAccess = "true"))
	int32 Level;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatComponent", meta = (AllowPrivateAccess = "true"))
	float CurrentHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStatComponent", meta = (AllowPrivateAccess = "true"))
	float MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatComponent", meta = (AllowPrivateAccess = "true"))
	float CurrentMP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStatComponent", meta = (AllowPrivateAccess = "true"))
	float MaxMP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStatComponent", meta = (AllowPrivateAccess = "true"))
	float ATK;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStatComponent", meta = (AllowPrivateAccess = "true"))
	float DEF;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStatComponent", meta = (AllowPrivateAccess = "true"))
	float NextEXP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStatComponent", meta = (AllowPrivateAccess = "true"))
	float CurrentEXP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStatComponent", meta = (AllowPrivateAccess = "true"))
	float CriticalRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStatComponent", meta = (AllowPrivateAccess = "true"))
	float CooldownReduction;

	FCharacterStat* CharacterStatRow;
	
public:
	FORCEINLINE float GetATK() const { return ATK; }
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }
	FORCEINLINE float GetCurrentMP() const { return CurrentMP; }
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetMaxMP() const { return MaxMP; }
	FORCEINLINE float GetDEF() const { return DEF; }
	FORCEINLINE int32 GetLevel() const { return Level; }
	FORCEINLINE float GetCriticalRate() const { return CriticalRate; }
	FORCEINLINE float GetCurrentEXP() const { return CurrentEXP; }
	FORCEINLINE float GetNextEXP() const { return NextEXP; }
	FORCEINLINE float GetCooldownReduction() const { return CooldownReduction; }

	FORCEINLINE void SetCurrentHP(float NewCurrentHP) { CurrentHP = NewCurrentHP; }
	FORCEINLINE void SetCurrentMP(float NewCurrentMP) { CurrentMP = NewCurrentMP; }
	FORCEINLINE void SetMaxHP(float NewMaxHP) { MaxHP = NewMaxHP; }
	FORCEINLINE void SetMaxMP(float NewMaxMP) { MaxMP = NewMaxMP; }

	void ChangeHP(float HPAmount);
	void ChangeMP(float MPAmount);

	void LevelUp();
	void AddEXP(float EXPAmount);
};
