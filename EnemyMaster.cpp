// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMaster.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEnemyMaster::AEnemyMaster() :
	bIsDead(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyMaster::BeginPlay()
{
	Super::BeginPlay();

	EnemyController = Cast<AEnemyAIController>(GetController());

	float Distance = 1000.f;
	EnemyController->GetBlackboardComponent()->SetValueAsFloat(TEXT("Distance"), Distance);

	if (EnemyController && BehaviorTree)
	{
		EnemyController->RunBehaviorTree(BehaviorTree);
	}
}

void AEnemyMaster::InitEnemyAttribute()
{
	if (AttributeDataTable)
	{
		switch (EnemyType)
		{
		case EEnemyType::EET_Dusk_Melee:
			AttributeRow = AttributeDataTable->FindRow<FEnemyAttribute>(TEXT("Dusk_Melee"), TEXT(""));
			break;
		}

		if (AttributeRow)
		{
			BehaviorTree = AttributeRow->BehaviorTree;
			NormalAttackMontage = AttributeRow->NormalAttackMontage;
		}
	}
}

// Called every frame
void AEnemyMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyMaster::PlayNormalAttackMontage()
{
	FName SectionName;
	int32 RandomNumber = FMath::RandRange(0, 3);
	switch (RandomNumber)
	{
	case 0:
		SectionName = TEXT("Attack_A");
		break;
	case 1:
		SectionName = TEXT("Attack_B");
		break;
	case 2:
		SectionName = TEXT("Attack_C");
		break;
	case 3:
		SectionName = TEXT("Attack_D");
		break;
	}
	
	GetMesh()->GetAnimInstance()->Montage_Play(NormalAttackMontage);
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, NormalAttackMontage);
}

void AEnemyMaster::DoDamage()
{
	//TArray<FHitResult> HitResults;
	//FCollisionQueryParams Params(NAME_None, false, this);
	//bool bResult = GetWorld()->SweepMultiByChannel(
	//	HitResults,
	//	GetActorLocation(),
	//	GetActorLocation() + GetActorForwardVector() * 200.f,
	//	FQuat::Identity,
	//	ECollisionChannel::ECC_GameTraceChannel1,
	//	FCollisionShape::MakeSphere(100.f),
	//	Params);
	
	TArray<FHitResult> HitResults;
	ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	bool bResult = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.f,
		100.f,
		TraceTypeQuery,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
	);

	if (bResult)
	{
		for (auto& HitResult : HitResults)
		{
			FDamageEvent DamageEvent{};
			auto Character = Cast<ACharacter>(HitResult.Actor);
			if (Character)
			{
				Character->TakeDamage(10.f, DamageEvent, GetController(), this);
			}
		}
	}
}

void AEnemyMaster::NormalAttackEnd()
{
	OnNormalAttackEndDelegate.Broadcast();
}

void AEnemyMaster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitEnemyAttribute();
}
