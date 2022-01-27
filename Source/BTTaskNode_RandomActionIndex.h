// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_RandomActionIndex.generated.h"

/**
 * 
 */
UCLASS()
class BAEKDU_API UBTTaskNode_RandomActionIndex : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_RandomActionIndex();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
