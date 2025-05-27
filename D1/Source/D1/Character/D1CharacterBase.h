// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/D1AttackInterface.h"
#include "Interface/D1ItemInterface.h"
#include  "Item/D1ItemData.h"
#include "D1CharacterBase.generated.h"

struct FD1CharacterStat;

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate , class UD1ItemData* /*InItemData*/)

UCLASS()
class D1_API AD1CharacterBase : public ACharacter , public ID1AttackInterface , public ID1ItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AD1CharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage , struct FDamageEvent const& DamageEvent , AController* EventInstigator ,
	                         AActor* DamageCauser) override;

public:
	virtual void ProcessAttack();
	virtual void ProcessComboAttack();

	virtual void ComboAttackBegin();
	virtual void ComboAttackEnd(class UAnimMontage* TargetMontage , bool IsProperlyEnded);

	void SetComboCheckTimer();
	void ComboCheck();

	// Inherited via ID1AttackInterface
	void AttackHitCheck() override;

	virtual void SetDead();

	void ApplyStat(const FD1CharacterStat& BaseStat , const FD1CharacterStat& ModifierStat);

	virtual void TakeItem(UD1ItemData* InItemData) override;
	virtual void DrinkPotion(UD1ItemData* InItemData);
	virtual void ReadScroll(UD1ItemData* InItemData);
	virtual void EquipWeapon(UD1ItemData* InItemData);

public :
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);

protected:
	TMap<EItemType , FOnTakeItemDelegate> TakeItemAction;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	TObjectPtr<class USkeletalMeshComponent>WeaponComponent;

protected:
	UPROPERTY(EditAnywhere , Category = Attack)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere , Category = Attack)
	TObjectPtr<class UAnimMontage> ComboAttackMontage;

	UPROPERTY(EditAnywhere , Category = Attack)
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere , Category = Attack)
	TObjectPtr<class UD1ComboAttackData> ComboAttackData;

	int32 CurrentCombo = 0;

	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	TObjectPtr<class UD1CharacterStatComponent> StatComponent;

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> HpBarWidgetComponent;
};
