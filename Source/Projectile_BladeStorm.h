// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "ProjectileMaster.h"
#include "Projectile_BladeStorm.generated.h"

class USceneComponent;
class UParticleSystemComponent;
class UCapsuleComponent;
class ASkill_BladeStorm;

/**
 * 
 */
UCLASS()
class BAEKDU_API AProjectile_BladeStorm : public AProjectileMaster
{
	GENERATED_BODY()
public:
	AProjectile_BladeStorm();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCapsuleOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void DestroyProjectile();
	
private:
	UPROPERTY()
	ASkill_BladeStorm* Skill_BladeStorm;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleSystem;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleCollision;

public:
	void SetLifeSpan(float LifeTime);
};
