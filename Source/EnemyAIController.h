// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UBlackboardComponent;

/**
 *
 */
UCLASS()
class BAEKDU_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;

protected:
	virtual void BeginPlay() override;

private:
	/** Blackboard component for this enemy */
	UPROPERTY(BlueprintReadOnly, Category = "EnemyAIController|AI", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent;
};
