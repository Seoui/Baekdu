// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayerController.h"

ACharacterPlayerController::ACharacterPlayerController()
{
}

void ACharacterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ACharacterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ACharacterPlayerController::BeginPlay"));

	bShowMouseCursor = true;
}
