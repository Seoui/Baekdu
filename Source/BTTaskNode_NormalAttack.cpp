// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_NormalAttack.h"
#include "EnemyMaster.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_NormalAttack::UBTTaskNode_NormalAttack() :
	IsAttacking(false)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyMaster* ControlledCharacter = Cast<AEnemyMaster>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (ControlledCharacter)
	{
		AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetPlayerCharacter")));
		IsAttacking = true;

		//ControlledCharacter->NormalAttackStart(Target);	Target이 꼭 필요한가?
		ControlledCharacter->PlayNormalAttackMontage();

		ControlledCharacter->OnNormalAttackEndDelegate.AddUFunction(this, FName("EndAttack"));
		return EBTNodeResult::InProgress;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}

void UBTTaskNode_NormalAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBTTaskNode::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (IsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTaskNode_NormalAttack::EndAttack()
{
	IsAttacking = false;
}
