// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/D1Monster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/D1AIController.h"
#include "Engine/AssetManager.h"

AD1Monster::AD1Monster()
{
	AIControllerClass = AD1AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("D1Monster"));

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetHiddenInGame(true);
}

void AD1Monster::BeginPlay()
{
	Super::BeginPlay();
}

void AD1Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AD1Monster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ensure(MonsterMeshes.Num() > 0);
	int32 RandomIndex = FMath::RandRange(0 , MonsterMeshes.Num() - 1);
	MonsterMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		MonsterMeshes[RandomIndex] , FStreamableDelegate::CreateUObject(this , &AD1Monster::MonsterMeshLoadCompleted));
}

void AD1Monster::SetDead()
{
	Super::SetDead();

	AD1AIController* AIController = Cast<AD1AIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle , FTimerDelegate::CreateLambda(
		                                       [&]()
		                                       {
			                                       Destroy();
		                                       }
	                                       ) , DeadEventDelayTime , false);
}

float AD1Monster::GetAttackRange()
{
	return 150.0f;
}

void AD1Monster::AttackByAI()
{
	ProcessComboAttack();
}

void AD1Monster::SetAIAttackFinishedDelegate(const FAIAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AD1Monster::ComboAttackEnd(class UAnimMontage* TargetMontage , bool IsProperlyEnded)
{
	Super::ComboAttackEnd(TargetMontage , IsProperlyEnded);

	//AttackTask한테 공격 끝남 알림
	OnAttackFinished.ExecuteIfBound();
}

void AD1Monster::MonsterMeshLoadCompleted()
{
	if (MonsterMeshHandle.IsValid())
	{
		USkeletalMesh* MonsterMesh = Cast<USkeletalMesh>(MonsterMeshHandle->GetLoadedAsset());
		if (MonsterMesh)
		{
			GetMesh()->SetSkeletalMesh(MonsterMesh);
			GetMesh() -> SetHiddenInGame(false);
		}
	}
	MonsterMeshHandle -> ReleaseHandle();
}
