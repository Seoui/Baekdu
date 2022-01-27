// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "EnemyMaster.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AEnemyAIController;

DECLARE_MULTICAST_DELEGATE(FOnNormalAttackEndDelegate);

USTRUCT(BlueprintType)
struct FEnemyAttribute : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* NormalAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HitReactMontage;
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	EET_Dusk_Melee UMETA(DisplayName = "Dusk_Melee"),
	
};

/**
 *
 */
UCLASS()
class BAEKDU_API AEnemyMaster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyMaster();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlayNormalAttackMontage();

	UFUNCTION(BlueprintCallable)
	void DoDamage();

	UFUNCTION(BlueprintCallable)
	void NormalAttackEnd();

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitEnemyAttribute();

public:
	//UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "EnemyMaster")
	FOnNormalAttackEndDelegate OnNormalAttackEndDelegate;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyMaster|Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* NormalAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyMaster", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyMaster", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyMaster", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyMaster", meta = (AllowPrivateAccess = "true"))
	AEnemyAIController* EnemyController;

	FEnemyAttribute* AttributeRow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyMaster", meta = (AllowPrivateAccess = "true"))
	UDataTable* AttributeDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyMaster", meta = (AllowPrivateAccess = "true"))
	EEnemyType EnemyType;

public:
	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
};
