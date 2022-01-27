// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMaster.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStatComponent.h"
#include "CharacterPlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "CharacterSkillComponent.h"
#include "SkillType.h"
#include "Components/WidgetComponent.h"
//#include "MathUtil.h"

// Sets default values
ACharacterMaster::ACharacterMaster():
	TurnRate(100.f),
	RightMouseButtonIsPressed(false),
	IsAttacking(false),
	SaveAttack(false),
	AttackCount(0),
	CharacterState(ECharacterState::ECS_Unoccupied)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->AddRelativeRotation(FRotator(-50.f, 0.f, 0.f));
	SpringArmComponent->TargetArmLength = 900.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = true;
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 900.f, 0.f);

	StatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("CharacterStatComponent"));
	SkillComponent = CreateDefaultSubobject<UCharacterSkillComponent>(TEXT("CharacterSkillComponent"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(CameraComponent);

	UE_LOG(LogTemp, Warning, TEXT("ACharacterMaster::ACharacterMaster"));
}

// Called when the game starts or when spawned
void ACharacterMaster::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("ACharacterMaster::BeginPlay"));

	AnimInstance = GetMesh()->GetAnimInstance();

	InitCharacterAttribute();

	UpdateUI();
}

void ACharacterMaster::UpdateUI()
{
	// ----- 초기 UI 세팅 -----
	OnCurrentHPChangedDelegate.Broadcast();
	OnCurrentMPChangedDelegate.Broadcast();
	OnMaxHPChangedDelegate.Broadcast();
	OnMaxMPChangedDelegate.Broadcast();
	OnCurrentEXPChangedDelegate.Broadcast();
	OnNextEXPChangedDelegate.Broadcast();
	OnStatChangedDelegate.Broadcast();
}

void ACharacterMaster::MoveForward(float Value)
{
	if ((Controller != nullptr) &&
		(Value != 0.0f) &&
		(GetCharacterState() != ECharacterState::ECS_SkillAttacking))
	{
		const FRotator YawRotation{ 0.0f, GetControlRotation().Yaw, 0.0f };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);
	}
}

void ACharacterMaster::MoveRight(float Value)
{
	if ((Controller != nullptr) && 
		(Value != 0.0f) &&
		(GetCharacterState() != ECharacterState::ECS_SkillAttacking))
	{
		const FRotator YawRotation{ 0.0f, GetControlRotation().Yaw, 0.0f };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void ACharacterMaster::Turn(float Value)
{
	if (RightMouseButtonIsPressed)
	{
		AddControllerYawInput(Value * TurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void ACharacterMaster::NormalAttack()
{
	if (GetCharacterState() != ECharacterState::ECS_SkillAttacking &&
		GetCharacterState() != ECharacterState::ECS_Jumping)
	{
		if (IsAttacking)
		{
			SaveAttack = true;
		}
		else // 실제 노멀 어택이 시작됨
		{
			SetCharacterState(ECharacterState::ECS_NormalAttacking);
			GetCharacterMovement()->bOrientRotationToMovement = false;
			IsAttacking = true;
			RotateToCursor();
			PlayNormalAttackMontage();
		}
	}
}

void ACharacterMaster::SkillOnePressed()
{
	SkillKeyPressed(0, ESkillSelection::ESS_SlotOne);
}

void ACharacterMaster::SkillTwoPressed()
{
	SkillKeyPressed(1, ESkillSelection::ESS_SlotTwo);
}

void ACharacterMaster::SkillThreePressed()
{
	SkillKeyPressed(2, ESkillSelection::ESS_SlotThree);
}

void ACharacterMaster::SkillFourPressed()
{
	SkillKeyPressed(3, ESkillSelection::ESS_SlotFour);
}

void ACharacterMaster::SkillFivePressed()
{
	SkillKeyPressed(4, ESkillSelection::ESS_SlotFive);
}

void ACharacterMaster::LeftMouseButtonPressed()
{
	if (GetCharacterState() == ECharacterState::ECS_SkillAiming)
	{
		SkillComponent->RunSkill((uint8)SelectedSkill);
	}
	else
	{
		NormalAttack();
	}
}

void ACharacterMaster::RightMouseButtonPressed()
{
	if (GetCharacterState() == ECharacterState::ECS_SkillAiming)
	{
		SetCharacterState(ECharacterState::ECS_Unoccupied);
	}
	else
	{
		RightMouseButtonIsPressed = true;
	}
}

void ACharacterMaster::RightMouseButtonReleased()
{
	RightMouseButtonIsPressed = false;
}

void ACharacterMaster::PlayHitReactMontage(const FVector CauserLocation)
{
	FName SectionName = GetHitReactSectionName(CauserLocation);

	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

FName ACharacterMaster::GetHitReactSectionName(const FVector CauserLocation)
{
	FName SectionName;
	FVector CharacterToEnemy = CauserLocation - GetActorLocation();
	CharacterToEnemy.Normalize();
	float ForwardDotResult = CharacterToEnemy | GetActorForwardVector();
	FVector CrossResult = GetActorForwardVector() ^ CharacterToEnemy;
	const float cos45 = FMath::Cos(PI / 4);

	if (ForwardDotResult >= cos45 && ForwardDotResult <= 1.f)
	{
		SectionName = TEXT("HitReact_Front");
	}
	else if (ForwardDotResult <= -cos45 && ForwardDotResult >= -1.f)
	{
		SectionName = TEXT("HitReact_Back");
	}
	else
	{
		if (CrossResult.Z > 0)
		{
			SectionName = TEXT("HitReact_Right");
		}
		else if (CrossResult.Z < 0)
		{
			SectionName = TEXT("HitReact_Left");
		}
		else
		{
			SectionName = TEXT("HitReact_Front");
		}
	}

	return SectionName;
}

void ACharacterMaster::InitCharacterAttribute()
{
	if (AttributeDataTable)
	{
		switch (CharacterType)
		{
		case ECharacterType::ECT_Greystone:
			AttributeRow = AttributeDataTable->FindRow<FCharacterAttribute>(TEXT("Greystone"), TEXT(""));
			break;
		case ECharacterType::ECT_Sparrow:
			AttributeRow = AttributeDataTable->FindRow<FCharacterAttribute>(TEXT("Sparrow"), TEXT(""));
			break;
		}

		if (AttributeRow)
		{
			NormalAttackMontage = AttributeRow->NormalAttackMontage;
			HitReactMontage = AttributeRow->HitReactMontage;
		}
	}
}

void ACharacterMaster::SkillKeyPressed(const uint8 Index, const ESkillSelection Selection)
{
	if (SkillComponent->GetSkillCooldownFromIndex(Index) == false &&
		SkillComponent->GetSkillResourceFromIndex(Index) <= StatComponent->GetCurrentMP())
	{
		SelectedSkill = Selection;
		if (SkillComponent->GetSkillTypeFromIndex(Index) == ESkillType::EST_DirectUse)
		{
			SkillComponent->RunSkill((uint8)SelectedSkill);
		}
		else
		{
			SetCharacterState(ECharacterState::ECS_SkillAiming);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("스킬이 쿨타임 중이거나, MP가 부족함"));
	}
}

void ACharacterMaster::LevelUp()
{
	StatComponent->LevelUp();
	UpdateUI();
}

void ACharacterMaster::AddEXP(float EXPAmount)
{
	StatComponent->AddEXP(EXPAmount);
	UpdateUI();
}

float ACharacterMaster::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	// DamageAmount를 캐릭터의 스탯에 따라 계산할 필요가 있음

	// 피격 방향에 따른 Hit React Montage 재생
	PlayHitReactMontage(DamageCauser->GetActorLocation());

	// HP 업데이트
	float NewCurrentHP = FMath::Clamp<float>(StatComponent->GetCurrentHP() - DamageAmount, 0.0f, StatComponent->GetMaxHP());
	StatComponent->SetCurrentHP(NewCurrentHP);

	// UI에 브로드캐스트
	OnCurrentHPChangedDelegate.Broadcast();

	return DamageAmount;
}

void ACharacterMaster::ComboAttackSave()
{
	// SaveAttack == true이면 다음 콤보 공격으로 넘어감.
	if (SaveAttack)
	{
		SaveAttack = false;
		RotateToCursor();
		PlayNormalAttackMontage();
	}
}

void ACharacterMaster::ResetCombo()
{
	AttackCount = 0;
	SaveAttack = false;
	IsAttacking = false;
}

void ACharacterMaster::PlayNormalAttackMontage()
{
	FName SectionName;

	switch (AttackCount)
	{
	case 0:
		AttackCount = 1;
		SectionName = FName(TEXT("NormalAttack_A"));
		break;
	case 1:
		AttackCount = 2;
		SectionName = FName(TEXT("NormalAttack_B"));
		break;
	case 2:
		AttackCount = 0;
		SectionName = FName(TEXT("NormalAttack_C"));
		break;
	}

	if (AnimInstance && NormalAttackMontage)
	{
		AnimInstance->Montage_Play(NormalAttackMontage);
		AnimInstance->Montage_JumpToSection(SectionName, NormalAttackMontage);
	}
}

// Called every frame
void ACharacterMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanRotate)
	{
		FRotator CurrentRotator = GetActorRotation();
		float Yaw = FMath::FInterpTo(CurrentRotator.Yaw, OrientationRotator.Yaw, DeltaTime, 20.f);
		GetCapsuleComponent()->SetWorldRotation(FRotator(0.f, Yaw, 0.f));
	}
}

// Called to bind functionality to input
void ACharacterMaster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ACharacterMaster::LeftMouseButtonPressed);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterMaster::Jump);
	PlayerInputComponent->BindAction("SkillOne", IE_Pressed, this, &ACharacterMaster::SkillOnePressed);
	PlayerInputComponent->BindAction("SkillTwo", IE_Pressed, this, &ACharacterMaster::SkillTwoPressed);
	PlayerInputComponent->BindAction("SkillThree", IE_Pressed, this, &ACharacterMaster::SkillThreePressed);
	PlayerInputComponent->BindAction("SkillFour", IE_Pressed, this, &ACharacterMaster::SkillFourPressed);
	PlayerInputComponent->BindAction("SkillFive", IE_Pressed, this, &ACharacterMaster::SkillFivePressed);
	
	PlayerInputComponent->BindAction("RightMouseButton", IE_Pressed, this, &ACharacterMaster::RightMouseButtonPressed);
	PlayerInputComponent->BindAction("RightMouseButton", IE_Released, this, &ACharacterMaster::RightMouseButtonReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterMaster::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterMaster::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACharacterMaster::Turn);
}

void ACharacterMaster::Jump()
{
	if (GetCharacterState() == ECharacterState::ECS_Unoccupied)
	{
		SetCharacterState(ECharacterState::ECS_Jumping);
		ACharacter::Jump();
	}
}

FVector ACharacterMaster::RotateToCursor()
{
	ACharacterPlayerController* PlayerController = Cast<ACharacterPlayerController>(GetController());

	FHitResult HitResult;
	ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
	bool TraceResult = PlayerController->GetHitResultUnderCursorByChannel(TraceTypeQuery, true, HitResult);
	if (TraceResult)
	{
		FVector CharacterLocation = GetActorLocation();
		FVector CursorLocation = HitResult.Location;
		CharacterLocation.Z = CursorLocation.Z;
		FVector CharacterToCursor = CursorLocation - CharacterLocation;
		OrientationRotator = CharacterToCursor.ToOrientationRotator();
		GetCapsuleComponent()->SetWorldRotation(OrientationRotator);

		DrawDebugDirectionalArrow(GetWorld(), CharacterLocation, CursorLocation, 80.f, FColor::Red, true, 60.f, 0, 10.f);
		return CursorLocation;
	}

	return FVector{};
}

void ACharacterMaster::SetCharacterState(ECharacterState NewCharacterState)
{
	CharacterState = NewCharacterState;
}
