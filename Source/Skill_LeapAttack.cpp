// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_LeapAttack.h"
#include "CharacterMaster.h"
#include "CharacterSkillComponent.h"
#include "CharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StandardFunctionLibrary.h"

ASkill_LeapAttack::ASkill_LeapAttack() :
	bIsLeaping(false),
	MaxInterpSpeed(5.f)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkillType = ESkillType::EST_NeedClick;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(250.f);
	SphereCollision->bHiddenInGame = true;

	SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ASkill_LeapAttack::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASkill_LeapAttack::OnSphereOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ASkill_LeapAttack::OnSphereEndOverlap);
}

void ASkill_LeapAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter && bIsLeaping)
	{
		const float MoveMultiplier = PlayerCharacter->GetAnimInstance()->GetCurveValue(TEXT("Move"));

		if (MoveMultiplier > 0.f)
		{
			const float InterpSpeed = MaxInterpSpeed * MoveMultiplier;
			const FVector CharacterLocation = PlayerCharacter->GetActorLocation();
			const FVector NewLocation = FMath::VInterpTo(CharacterLocation, Destination, DeltaTime, InterpSpeed);
			PlayerCharacter->SetActorLocation(NewLocation);
		}
	}
}

void ASkill_LeapAttack::RunSkill(FSkillInfo* _SkillInfo)
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

			// 캐릭터를 커서방향으로 주어진 거리만큼 이동하도록 목적지 설정
			const FVector CharacterLocation = PlayerCharacter->GetActorLocation();
			const FVector DirectionToCursor = CursorLocation - CharacterLocation;
			const float DistanceToCursor = DirectionToCursor.Size();
			const FVector UnitDirection = DirectionToCursor.GetSafeNormal();

			if (DistanceToCursor <= SkillInfo->Range) // 커서까지의 거리가 최대거리보다 작다면
			{
				Destination = CursorLocation;
			}
			else
			{
				Destination = CharacterLocation + UnitDirection * SkillInfo->Range;
			}
			Destination.Z = CharacterLocation.Z; // Destination.Z를 캐릭터의 높이와 동일하게

			// LeapAttack start 이펙트
			if (SkillInfo->SkillEffect1)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					SkillInfo->SkillEffect1,
					CharacterLocation,
					PlayerCharacter->GetActorRotation(),
					true);
			}

			bIsLeaping = true;
			PlayerCharacter->GetAnimInstance()->Montage_Play(SkillInfo->SkillMontage1);
			//DrawDebugDirectionalArrow(GetWorld(), PlayerCharacter->GetActorLocation(), CursorLocation, 80.f, FColor::Blue, true, 60.f);
		}
	}
}

void ASkill_LeapAttack::OnSphereOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == PlayerCharacter)
	{
		// do nothing 자신이면 캐릭터 충돌안되도록 콜리전 설정할 예정
	}
	else
	{
		const float ATK = PlayerCharacter->GetStatComponent()->GetATK();
		const float Damage = UStandardFunctionLibrary::CalculateDamage(ATK, SkillInfo->Coefficient, SkillInfo->Damage);
		const FDamageEvent DamageEvent;
		OtherActor->TakeDamage(Damage, DamageEvent, PlayerCharacter->GetController(), PlayerCharacter);
	}
}

void ASkill_LeapAttack::OnSphereEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
}

void ASkill_LeapAttack::ActivateCollision()
{
	SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	// LeapAttack smash 이펙트
	FVector CharacterLocation = PlayerCharacter->GetActorLocation();
	if (SkillInfo->SkillEffect2)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			SkillInfo->SkillEffect2,
			FVector(CharacterLocation.X, CharacterLocation.Y, 70.f),
			PlayerCharacter->GetActorRotation(),
			true);
	}
}

void ASkill_LeapAttack::DeactivateCollision()
{
	SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	bIsLeaping = false;
}