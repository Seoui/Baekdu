// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CalculateDistance.generated.h"

class AEnemyMaster;
class ACharacterMaster;

/**
 * 
 */
UCLASS()
class BAEKDU_API UBTService_CalculateDistance : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CalculateDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY()
	AEnemyMaster* ControllingPawn;

	UPROPERTY()
	ACharacterMaster* PlayerCharacter;
};
