// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_AbortMoveTo.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_AbortMoveTo::UBTService_AbortMoveTo()
{

}

void UBTService_AbortMoveTo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetBlackboardComponent())
	{
		float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"));

		if (Distance <= 500.f)
		{
			if (OwnerComp.GetAIOwner())
			{
				OwnerComp.GetAIOwner()->StopMovement();
			}
		}
	}
}
