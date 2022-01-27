// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "CharacterMaster.h"
#include "SkillType.h"
#include "CharacterSkillComponent.generated.h"

class ACharacterMaster;
class ACharacterSkillMaster;
class UAnimMontage;
class USoundCue;

USTRUCT(BlueprintType)
struct FSkillInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterType CharacterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACharacterSkillMaster> SkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Coefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Resource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VarianceCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VarianceDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VarianceResource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SkillIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* SkillSound1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* SkillSound2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* SkillSound3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* SkillEffect1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* SkillEffect2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* SkillEffect3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* SkillMontage1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* SkillMontage2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
};

/**
 *
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAEKDU_API UCharacterSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterSkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void ResetCooldown(const uint8 Index);

	void InitSkillSet();

	void UpdateMP(const uint8 KeyIndex);

	void RunCooldown(const uint8 KeyIndex);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ACharacterSkillMaster*> SkillSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FSkillInfo> SkillInfos;

	UPROPERTY()
	ACharacterMaster* PlayerCharacter;

public:
	ESkillType GetSkillTypeFromIndex(const uint8 Index) const;

	bool GetSkillCooldownFromIndex(const uint8 Index) const;

	float GetSkillResourceFromIndex(const uint8 Index) const;

	void RunSkill(const uint8 KeyIndex);
};
