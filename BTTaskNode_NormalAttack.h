// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_NormalAttack.generated.h"

/**
 * 
 */
UCLASS()
class BAEKDU_API UBTTaskNode_NormalAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_NormalAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UFUNCTION()
	void EndAttack();

private:
	bool IsAttacking;
};
