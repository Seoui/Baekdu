// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_FireAura.h"
#include "CharacterMaster.h"
#include "CharacterSkillComponent.h"
#include "CharacterStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "StandardFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"

ASkill_FireAura::ASkill_FireAura() :
	SkillTickDuration(1.f)
{
	SkillType = ESkillType::EST_DirectUse;
}

void ASkill_FireAura::BeginPlay()
{
	Super::BeginPlay();
}

void ASkill_FireAura::RunSkill(FSkillInfo * _SkillInfo)
{
	if (PlayerCharacter)
	{
		if (PlayerCharacter->GetCharacterState() != ECharacterState::ECS_Jumping)
		{
			SkillInfo = _SkillInfo;

			// 캐릭터 상태를 설정 (ECS_SkillAiming -> ECS_SkillAttacking)
			PlayerCharacter->SetCharacterState(ECharacterState::ECS_SkillAttacking);

			// FireAura 이펙트
			FireAuraFX = UGameplayStatics::SpawnEmitterAttached(SkillInfo->SkillEffect1, PlayerCharacter->GetMesh(), FName(),
				PlayerCharacter->GetActorLocation(), FRotator(0.f), EAttachLocation::KeepWorldPosition);

			// FireAura 애니메이션
			PlayerCharacter->GetAnimInstance()->Montage_Play(SkillInfo->SkillMontage1);

			SetSkillTimer();
		}
	}
}

void ASkill_FireAura::DoDamage()
{
	const FVector CharacterLocation = PlayerCharacter->GetActorLocation();
	ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PlayerCharacter);
	TArray<FHitResult> OutHits;

	bool bResult = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		CharacterLocation,
		CharacterLocation,
		250.f,
		TraceTypeQuery,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHits,
		true
	);

	if (bResult)
	{
		for (auto& HitResult : OutHits)
		{
			ACharacterMaster* Enemy = Cast<ACharacterMaster>(HitResult.Actor);
			if (Enemy)
			{
				const float ATK = PlayerCharacter->GetStatComponent()->GetATK();
				const float Damage = UStandardFunctionLibrary::CalculateDamage(ATK, SkillInfo->Coefficient, SkillInfo->Damage);
				const FDamageEvent DamageEvent;
				Enemy->TakeDamage(Damage, DamageEvent, PlayerCharacter->GetController(), PlayerCharacter);
				
			}
		}
	}
}

void ASkill_FireAura::ClearSkillTimer(FTimerHandle DoDamageTimerHandle)
{
	GetWorld()->GetTimerManager().ClearTimer(DoDamageTimerHandle);
	if (FireAuraFX)
	{
		FireAuraFX->Deactivate();
	}
}

void ASkill_FireAura::SetSkillTimer()
{
	// 타이머 설정 (스킬 데미지 틱)
	FTimerHandle DoDamageTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DoDamageTimerHandle, this, &ASkill_FireAura::DoDamage, SkillTickDuration, true);

	// 타이머 설정 (스킬 전체)
	FTimerHandle FireAuraTimerHandle;
	FTimerDelegate FireAuraTimerDelegate;
	FireAuraTimerDelegate.BindUFunction(this, FName("ClearSkillTimer"), DoDamageTimerHandle, FireAuraFX);
	GetWorld()->GetTimerManager().SetTimer(FireAuraTimerHandle, FireAuraTimerDelegate, SkillInfo->Duration, false);
}
