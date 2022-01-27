// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsDead.h"
#include "EnemyAIController.h"
#include "EnemyMaster.h"

UBTDecorator_IsDead::UBTDecorator_IsDead()
{
	NodeName = TEXT("IsDead");
}

bool UBTDecorator_IsDead::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AEnemyMaster* ControllingPawn = Cast<AEnemyMaster>(OwnerComp.GetAIOwner()->GetPawn());

	if (ControllingPawn)
	{
		if (ControllingPawn->GetIsDead())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}