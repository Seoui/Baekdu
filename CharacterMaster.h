// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Baekdu.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "CharacterMaster.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCharacterStatComponent;
class UCharacterSkillComponent;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCurrentHPChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCurrentMPChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCurrentEXPChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMaxHPChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMaxMPChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextEXPChangedDelegate);

USTRUCT(BlueprintType)
struct FCharacterAttribute: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* NormalAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HitReactMontage;
};

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	ECT_Greystone UMETA(DisplayName = "Greystone"),
	ECT_Sparrow UMETA(DisplayName = "Sparrow"),
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_NormalAttacking UMETA(DisplayName = "NormalAttack"),
	ECS_SkillAiming UMETA(DisplayName = "SkillAiming"),
	ECS_SkillAttacking UMETA(DisplayName = "SkillAttacking"),
	ECS_Jumping UMETA(DisplayName = "Jumping"),
	ECS_Dead UMETA(DisplayName = "Dead"),
};

UENUM(BlueprintType)
enum class ESkillSelection : uint8
{
	ESS_SlotOne UMETA(DisplayName = "SlotOne"),
	ESS_SlotTwo UMETA(DisplayName = "SlotTwo"),
	ESS_SlotThree UMETA(DisplayName = "SlotThree"),
	ESS_SlotFour UMETA(DisplayName = "SlotFour"),
	ESS_SlotFive UMETA(DisplayName = "SlotFive"),
};

/**
 *
 */
UCLASS()
class BAEKDU_API ACharacterMaster : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Character)
	FOnCurrentHPChangedDelegate OnCurrentHPChangedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Character)
	FOnCurrentMPChangedDelegate OnCurrentMPChangedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Character)
	FOnCurrentEXPChangedDelegate OnCurrentEXPChangedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Character)
	FOnStatChangedDelegate OnStatChangedDelegate;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Character)
	FOnMaxHPChangedDelegate OnMaxHPChangedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Character)
	FOnMaxMPChangedDelegate OnMaxMPChangedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Character)
	FOnNextEXPChangedDelegate OnNextEXPChangedDelegate;

public:
	// Sets default values for this character's properties
	ACharacterMaster();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump() override;

	FVector RotateToCursor();

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UFUNCTION(BlueprintCallable)
	void SkillKeyPressed(const uint8 Index, const ESkillSelection Selection);

	UFUNCTION(BlueprintCallable)
	void LevelUp();

	UFUNCTION(BlueprintCallable)
	void AddEXP(float EXPAmount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateUI();

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Turn(float Value);

	UFUNCTION(BlueprintCallable)
	void ComboAttackSave();

	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	void PlayNormalAttackMontage();

	void NormalAttack();

	void SkillOnePressed();

	void SkillTwoPressed();

	void SkillThreePressed();

	void SkillFourPressed();

	void SkillFivePressed();

	void LeftMouseButtonPressed();

	void RightMouseButtonPressed();

	void RightMouseButtonReleased();
	
	void PlayHitReactMontage(const FVector CauserLocation);

	FName GetHitReactSectionName(const FVector CauserLocation);

	void InitCharacterAttribute();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterMaster|Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterMaster|Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterMaster|Animation", meta = (AllowPrivateAccess = "true"))
	UAnimInstance* AnimInstance;

	float TurnRate;

	bool RightMouseButtonIsPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterMaster|Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* NormalAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterMaster|Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactMontage;

	bool IsAttacking;

	bool SaveAttack;

	int32 AttackCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterMaster|Animation", meta = (AllowPrivateAccess = "true"))
	bool bCanRotate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterMaster|Component", meta = (AllowPrivateAccess = "true"))
	UCharacterStatComponent* StatComponent;

	FRotator OrientationRotator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterMaster", meta = (AllowPrivateAccess = "true"))
	ECharacterType CharacterType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterMaster|Component", meta = (AllowPrivateAccess = "true"))
	UCharacterSkillComponent* SkillComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterMaster", meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterMaster", meta = (AllowPrivateAccess = "true"))
	ESkillSelection SelectedSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterMaster", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WidgetComponent;

	FCharacterAttribute* AttributeRow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* AttributeDataTable;

public:
	FORCEINLINE ECharacterType GetCharacterType() const { return CharacterType; }
	FORCEINLINE UAnimInstance* GetAnimInstance() const { return GetMesh()->GetAnimInstance(); }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE UCharacterStatComponent* GetStatComponent() const { return StatComponent; }

	UFUNCTION(BlueprintCallable)
	void SetCharacterState(ECharacterState NewCharacterState);
};
