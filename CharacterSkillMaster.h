// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "SkillType.h"
#include "GameFramework/Actor.h"
#include "CharacterSkillMaster.generated.h"

class ACharacterMaster;

/**
 *
 */
UCLASS()
class BAEKDU_API ACharacterSkillMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterSkillMaster();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void RunSkill(struct FSkillInfo* _SkillInfo);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	ACharacterMaster* PlayerCharacter;
	
	FSkillInfo* SkillInfo;

	UPROPERTY()
	ESkillType SkillType;

	bool bIsCooldown;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent;

public:
	FORCEINLINE ESkillType GetSkillType() const { return SkillType; }
	FORCEINLINE bool GetIsCooldown() const { return bIsCooldown; }
	FORCEINLINE void SetIsCooldown(bool Cooldown) { bIsCooldown = Cooldown; }
};


