// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StandardFunctionLibrary.generated.h"

UCLASS()
class BAEKDU_API UStandardFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static float CalculateDamage(const float Stat, const float Coefficient, const float SkillDamage);
};
