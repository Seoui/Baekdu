// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsTargetDead.h"
#include "EnemyAIController.h"
#include "CharacterMaster.h"
#include "Kismet/GameplayStatics.h"

UBTDecorator_IsTargetDead::UBTDecorator_IsTargetDead()
{
	NodeName = TEXT("IsTargetDead");
}

bool UBTDecorator_IsTargetDead::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ACharacterMaster* Target = Cast<ACharacterMaster>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Target)
	{
		if (Target->GetCharacterState() == ECharacterState::ECS_Dead)
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