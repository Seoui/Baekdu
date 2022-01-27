// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileMaster.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileMaster::AProjectileMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void AProjectileMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

