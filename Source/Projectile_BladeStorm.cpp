// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_BladeStorm.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Skill_BladeStorm.h"
#include "CharacterMaster.h"

AProjectile_BladeStorm::AProjectile_BladeStorm()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(RootComponent);

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->SetupAttachment(RootComponent);
	CapsuleCollision->bHiddenInGame = false;

	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AProjectile_BladeStorm::BeginPlay()
{
	Super::BeginPlay();

	Skill_BladeStorm = Cast<ASkill_BladeStorm>(GetOwner());
	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_BladeStorm::OnCapsuleOverlap);
}

void AProjectile_BladeStorm::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->GetSuperClass() != ACharacterMaster::StaticClass())
	{
		Skill_BladeStorm->DoDamage(OtherActor);
	}
}

void AProjectile_BladeStorm::DestroyProjectile()
{
	Destroy();
}

void AProjectile_BladeStorm::SetLifeSpan(float LifeTime)
{
	// 타이머 설정
	FTimerHandle LifeTimer;
	GetWorldTimerManager().SetTimer(
		LifeTimer,
		this,
		&AProjectile_BladeStorm::DestroyProjectile,
		LifeTime,
		false);
}
