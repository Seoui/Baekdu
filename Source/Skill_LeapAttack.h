// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "CharacterSkillMaster.h"
#include "Skill_LeapAttack.generated.h"

class USphereComponent;
class USceneComponent;

/**
 * 
 */
UCLASS()
class BAEKDU_API ASkill_LeapAttack : public ACharacterSkillMaster
{
	GENERATED_BODY()
	
public:
	ASkill_LeapAttack();

	virtual void Tick(float DeltaTime) override;

	virtual void RunSkill(struct FSkillInfo* _SkillInfo) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);
	
private:
	UPROPERTY(VisibleAnywhere)
	FVector Destination;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereCollision;

	bool bIsLeaping;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MaxInterpSpeed;

public:
	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

};
