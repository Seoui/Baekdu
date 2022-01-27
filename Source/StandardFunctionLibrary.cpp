// Fill out your copyright notice in the Description page of Project Settings.


#include "StandardFunctionLibrary.h"

float UStandardFunctionLibrary::CalculateDamage(const float Stat, const float Coefficient, const float SkillDamage)
{
	return Stat * Coefficient + SkillDamage;
}