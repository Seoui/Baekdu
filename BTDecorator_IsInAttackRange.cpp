// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsInAttackRange.h"
#include "EnemyMaster.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{

}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	if (OwnerComp.GetBlackboardComponent())
	{
		float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"));

		if (Distance <= 500.f)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bIsInAttackRange"), true);
			return true;
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bIsInAttackRange"), false);
			return false;
		}
	}

	return false;
}
