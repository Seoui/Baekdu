// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_RandomActionIndex.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_RandomActionIndex::UBTTaskNode_RandomActionIndex()
{

}

EBTNodeResult::Type UBTTaskNode_RandomActionIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetBlackboardComponent())
	{
		int32 RandIndex = FMath::RandRange(0, 2);
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("ActionIndex"), 0);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
