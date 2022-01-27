// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CalculateDistance.h"
#include "CharacterMaster.h"
#include "EnemyMaster.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CalculateDistance::UBTService_CalculateDistance()
{
	NodeName = TEXT("CalculateDistance");
	Interval = 0.1f;
}

void UBTService_CalculateDistance::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (ControllingPawn == nullptr)
	{
		ControllingPawn = Cast<AEnemyMaster>(OwnerComp.GetAIOwner()->GetPawn());
	}
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<ACharacterMaster>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetPlayerCharacter")));
	}

	float Distance = ControllingPawn->GetDistanceTo(PlayerCharacter);

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("Distance"), Distance);
}
