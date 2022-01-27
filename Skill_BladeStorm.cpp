// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_BladeStorm.h"
#include "CharacterMaster.h"
#include "CharacterSkillComponent.h"
#include "CharacterStatComponent.h"
#include "Projectile_BladeStorm.h"
#include "StandardFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

ASkill_BladeStorm::ASkill_BladeStorm()
{
	SkillType = ESkillType::EST_NeedClick;
}

void ASkill_BladeStorm::RunSkill(FSkillInfo * _SkillInfo)
{
	if (PlayerCharacter)
	{
		if (PlayerCharacter->GetCharacterState() != ECharacterState::ECS_Jumping)
		{
			SkillInfo = _SkillInfo;

			// 캐릭터 상태를 설정 (ECS_SkillAiming -> ECS_SkillAttacking)
			PlayerCharacter->SetCharacterState(ECharacterState::ECS_SkillAttacking);

			// 캐릭터를 커서방향으로 회전
			const FVector CursorLocation = PlayerCharacter->RotateToCursor();

			// 몽타주 재생
			PlayerCharacter->GetAnimInstance()->Montage_Play(SkillInfo->SkillMontage1);

			SpawnBladeStormProjectile(SkillInfo);
		}
	}
}

void ASkill_BladeStorm::BeginPlay()
{
	Super::BeginPlay();
}

void ASkill_BladeStorm::SpawnBladeStormProjectile(FSkillInfo* _SkillInfo)
{
	FString ProjectilePath(TEXT("Blueprint'/Game/_Baekdu/02_Skill/Projectile/Projectile_BladeStormBP.Projectile_BladeStormBP_C'"));
	auto ProjectileClass = StaticLoadClass(AProjectile_BladeStorm::StaticClass(), nullptr, *ProjectilePath);

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Owner = this;
	auto Projectile = GetWorld()->SpawnActor<AProjectile_BladeStorm>(
		ProjectileClass,
		PlayerCharacter->GetActorLocation(),
		PlayerCharacter->GetActorRotation(),
		ActorSpawnParameters);
	const float SkillRange = SkillInfo->Range;
	const float SkillSpeed = Projectile->GetVelocity().Size();
	Projectile->SetLifeSpan(SkillRange / SkillSpeed);
}

void ASkill_BladeStorm::DoDamage(AActor* Actor)
{
	const float ATK = PlayerCharacter->GetStatComponent()->GetATK();
	const float Damage = UStandardFunctionLibrary::CalculateDamage(ATK, SkillInfo->Coefficient, SkillInfo->Damage);
	const FDamageEvent DamageEvent;
	Actor->TakeDamage(Damage, DamageEvent, PlayerCharacter->GetController(), PlayerCharacter);
}
