// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "CharacterSkillMaster.h"
#include "Skill_FireAura.generated.h"

class USphereComponent;
class UParticleSystemComponent;

/**
 * 
 */
UCLASS()
class BAEKDU_API ASkill_FireAura : public ACharacterSkillMaster
{
	GENERATED_BODY()
public:
	ASkill_FireAura();

	virtual void RunSkill(struct FSkillInfo* _SkillInfo) override;

protected:
	virtual void BeginPlay() override;

	void DoDamage();

	UFUNCTION()
	void ClearSkillTimer(FTimerHandle DoDamageTimerHandle);

	void SetSkillTimer();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float SkillTickDuration;

	UPROPERTY()
	UParticleSystemComponent* FireAuraFX;
};
