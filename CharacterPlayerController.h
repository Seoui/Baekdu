// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "GameFramework/PlayerController.h"
#include "CharacterPlayerController.generated.h"

/**
 *
 */
UCLASS()
class BAEKDU_API ACharacterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACharacterPlayerController();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowCooldown(const uint8 Index);

protected:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
};
