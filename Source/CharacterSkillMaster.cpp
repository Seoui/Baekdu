// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillMaster.h"
#include "CharacterMaster.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACharacterSkillMaster::ACharacterSkillMaster() :
	SkillType(ESkillType::EST_MAX),
	bIsCooldown(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
}

// Called when the game starts or when spawned
void ACharacterSkillMaster::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ACharacterMaster>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	const FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, true);
	AttachToComponent(PlayerCharacter->GetMesh(), Rules);
}

// Called every frame
void ACharacterSkillMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterSkillMaster::RunSkill(FSkillInfo * _SkillInfo)
{
}

