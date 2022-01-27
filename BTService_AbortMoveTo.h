// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AbortMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class BAEKDU_API UBTService_AbortMoveTo : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_AbortMoveTo();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
