#pragma once

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	EST_DirectUse UMETA(DisplayName = "DirectUse"),
	EST_NeedClick UMETA(DisplayName = "NeedClick"),

	EST_MAX UMETA(DisplayName = "DefaultMAX")
};