// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/D1CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "D1ComboAttackData.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/D1CharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/D1HpBarWidget.h"
#include "Item/D1PotionItemData.h"
#include "Item/D1ScrollItemData.h"
#include "Item/D1WeaponItemData.h"

// Sets default values
AD1CharacterBase::AD1CharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(34.0f , 88.0f);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f , 720.0f , 0.0f);

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f , 0.0f , -88.0f) , FRotator(0.0f , -90.0f , 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FindMeshRef(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/_Art/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Standard.SK_CharM_Standard'"));
	if (FindMeshRef.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(FindMeshRef.Object);
	}

	// Animation
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(
		TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/ABP_Player.ABP_Player_C'"));
	if (AnimInstanceRef.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	// Weapon Component
	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponComponent->SetupAttachment(GetMesh() ,TEXT("hand_rSocket"));

	//Stat Component
	StatComponent = CreateDefaultSubobject<UD1CharacterStatComponent>(TEXT("Stat"));

	// HpBar Widget Component
	HpBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBarWidgetComponent->SetupAttachment(GetRootComponent());
	HpBarWidgetComponent->SetRelativeLocation(FVector(0.0f , 0.0f , 90.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_HpBar.WBP_HpBar_c'"));
	if (HpBarWidgetRef.Succeeded())
	{
		HpBarWidgetComponent->SetWidgetClass(HpBarWidgetRef.Class);
		HpBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		HpBarWidgetComponent->SetDrawSize(FVector2D(150.0f , 15.0f));
		HpBarWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


	// Attack Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(
		TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_Attack.AM_Attack'"));
	if (AttackMontageRef.Succeeded())
	{
		AttackMontage = AttackMontageRef.Object;
	}

	// ComboAttack Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboAttackMontageRef(
		TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboAttackMontageRef.Succeeded())
	{
		ComboAttackMontage = ComboAttackMontageRef.Object;
	}

	// Dead Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(
		TEXT("/Script/Engine.AnimMontage'/Game/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Succeeded())
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// ComboAttack Data
	static ConstructorHelpers::FObjectFinder<UD1ComboAttackData> ComboAttackDataRef(
		TEXT("/Script/D1.D1ComboAttackData'/Game/CharacterAction/DA_ComboAttackData.DA_ComboAttackData'"));
	if (ComboAttackDataRef.Succeeded())
	{
		ComboAttackData = ComboAttackDataRef.Object;
	}

#pragma region ItemAction
	TakeItemAction.Add(EItemType::Potion , FOnTakeItemDelegate::CreateUObject(this , &AD1CharacterBase::DrinkPotion));
	TakeItemAction.Add(EItemType::Scroll , FOnTakeItemDelegate::CreateUObject(this , &AD1CharacterBase::ReadScroll));
	TakeItemAction.Add(EItemType::Weapon , FOnTakeItemDelegate::CreateUObject(this , &AD1CharacterBase::EquipWeapon));
#pragma endregion
}

// Called when the game starts or when spawned
void AD1CharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AD1CharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StatComponent->OnHpZero.AddUObject(this , &AD1CharacterBase::SetDead);
	StatComponent->OnStatChanged.AddUObject(this , &AD1CharacterBase::ApplyStat);

	if (HpBarWidgetComponent == nullptr)
		return;

	HpBarWidgetComponent->InitWidget();
	UD1HpBarWidget* HpBarWidget = Cast<UD1HpBarWidget>(HpBarWidgetComponent->GetUserWidgetObject());
	if (HpBarWidget)
	{
		HpBarWidget->UpdateStat(StatComponent->GetBaseStat() , StatComponent->GetModifierStat());
		HpBarWidget->UpdateHp(StatComponent->GetCurrentHp());

		StatComponent->OnHpChanged.AddUObject(HpBarWidget , &UD1HpBarWidget::UpdateHp);
		StatComponent->OnStatChanged.AddUObject(HpBarWidget , &UD1HpBarWidget::UpdateStat);
	}
}

// Called every frame
void AD1CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AD1CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AD1CharacterBase::TakeDamage(float Damage , FDamageEvent const& DamageEvent , AController* EventInstigator ,
                                   AActor* DamageCauser)
{
	Super::TakeDamage(Damage , DamageEvent , EventInstigator , DamageCauser);

	StatComponent->ApplyDamage(Damage);

	return Damage;
}

void AD1CharacterBase::ProcessAttack()
{
	if (GetCurrentMontage() == AttackMontage)
		return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		int32 Index = FMath::RandRange(1 , 4);
		FString SectionName = FString::Printf(TEXT("Attack%d") , Index);
		AnimInstance->Montage_Play(AttackMontage , 1.0f);
		AnimInstance->Montage_JumpToSection(FName(*SectionName));
	}
}

void AD1CharacterBase::ProcessComboAttack()
{
	if (CurrentCombo == 0)
	{
		ComboAttackBegin();
		return;
	}

	if (ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = true;
	}
	else
	{
		HasNextComboCommand = false;
	}
}

void AD1CharacterBase::ComboAttackBegin()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	CurrentCombo = 1;

	const float AttackSpeedRate = StatComponent->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboAttackMontage , AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this , &AD1CharacterBase::ComboAttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate , ComboAttackMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AD1CharacterBase::ComboAttackEnd(UAnimMontage* TargetMontage , bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	CurrentCombo = 0;
}

void AD1CharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboAttackData->EffectiveFrameCount[ComboIndex] / ComboAttackData->FrameRate) /
		AttackSpeedRate;

	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle , this , &AD1CharacterBase::ComboCheck ,
		                                       ComboEffectiveTime , false);
	}
}

void AD1CharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();

	if (HasNextComboCommand)
	{
		UAnimInstance* AimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1 , 1 , ComboAttackData->MaxComboCount);

		FName NextSection = *FString::Printf(TEXT("%s%d") , *ComboAttackData->MontageSectionNamePrefix , CurrentCombo);

		AimInstance->Montage_JumpToSection(NextSection , ComboAttackMontage);

		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void AD1CharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack) , false , this);

	const float AttackRange = StatComponent->GetTotalStat().AttackRange;
	const float AttackRadius = StatComponent->GetTotalStat().AttackRadius;
	const float AttackDamage = StatComponent->GetTotalStat().Attack;

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->
		GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult , Start , End , FQuat::Identity ,
	                                                    ECollisionChannel::ECC_GameTraceChannel2 ,
	                                                    FCollisionShape::MakeSphere(AttackRadius) , Params);

	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage , DamageEvent , GetController() , this);
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld() , CapsuleOrigin , CapsuleHalfHeight , AttackRadius ,
	                 FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat() , DrawColor , false , 5.0f);
#endif
}

void AD1CharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(false);
		AnimInstance->Montage_Play(DeadMontage , 1.0f);
	}

	SetActorEnableCollision(false);

	HpBarWidgetComponent->SetHiddenInGame(true);
}

void AD1CharacterBase::ApplyStat(const FD1CharacterStat& BaseStat , const FD1CharacterStat& ModifierStat)
{
	float MovementSpeed = (BaseStat + ModifierStat).MovementSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void AD1CharacterBase::TakeItem(UD1ItemData* InItemData)
{
	if (InItemData)
	{
		TakeItemAction[InItemData->Type].ExecuteIfBound(InItemData);
	}
}


void AD1CharacterBase::DrinkPotion(UD1ItemData* InItemData)
{
	UD1PotionItemData* PotionItemData = Cast<UD1PotionItemData>(InItemData);
	if (PotionItemData)
	{
		if (StatComponent)
		{
			StatComponent->SetHp(StatComponent->GetCurrentHp() + PotionItemData->HealAmount);
		}
	}
}

void AD1CharacterBase::ReadScroll(UD1ItemData* InItemData)
{
	UE_LOG(LogTemp , Log , TEXT("Read"));
	UD1ScrollItemData* ScrollItemData = Cast<UD1ScrollItemData>(InItemData);
	if (ScrollItemData)
	{
		if (StatComponent)
		{
			StatComponent->AddBaseStat(ScrollItemData->BaseStat);
		}
	}
}

void AD1CharacterBase::EquipWeapon(UD1ItemData* InItemData)
{
	UE_LOG(LogTemp , Log , TEXT("Equip"));

	UD1WeaponItemData* WeaponItemData = Cast<UD1WeaponItemData>(InItemData);
	if (WeaponItemData)
	{
		if (WeaponItemData->WeaponMesh.IsPending())
		{
			WeaponItemData -> WeaponMesh.LoadSynchronous();
		}

		if (WeaponComponent)
		{
			WeaponComponent->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
		}

		if (StatComponent)
		{
			StatComponent->SetModifierStat(WeaponItemData->ModifierStat);
		}
	}
}

int32 AD1CharacterBase::GetLevel()
{
	return  StatComponent->GetCurrentLevel();
}

void AD1CharacterBase::SetLevel(int32 InNewLevel)
{
	StatComponent->SetLevel(InNewLevel);
}
