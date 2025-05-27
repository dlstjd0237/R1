// Fill out your copyright notice in the Description page of Project Settings.


#include "D1Stagegimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/OverlapResult.h"
#include "Character/D1Monster.h"
#include "Item/D1ItemBox.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/D1GameInterface.h"

// Sets default values
AD1Stagegimmick::AD1Stagegimmick()
{
	//Stage Section
	StageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageMesh"));
	SetRootComponent(StageMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/_Art/Environment/Stages/SM_SQUARE.SM_SQUARE'"));
	if (StageMeshRef.Succeeded())
	{
		StageMesh->SetStaticMesh(StageMeshRef.Object);
	}

	StageTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTriggerBox"));
	StageTriggerBox->SetupAttachment(GetRootComponent());
	StageTriggerBox->SetBoxExtent(FVector(775.0f , 775.0f , 300.0f));
	StageTriggerBox->SetRelativeLocation(FVector(0.0f , 0.0f , 250.0f));
	StageTriggerBox->SetCollisionProfileName(FName("D1Trigger"));
	StageTriggerBox->OnComponentBeginOverlap.AddDynamic(this , &AD1Stagegimmick::OnStageTriggerBoxBeginOverlap);


	//Gate Section
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(
		TEXT("/Script/Engine.StaticMesh'/Game/_Art/Environment/Props/SM_GATE.SM_GATE'"));

	if (GateMeshRef.Succeeded())
		UE_LOG(LogTemp , Log , TEXT("SM_GATE Not Find! :("));

	static FName GateSockets[] = {TEXT("+XGate") , TEXT("-XGate") ,TEXT("+YGate") ,TEXT("-YGate")};
	for (FName GateSocket : GateSockets)
	{
		UStaticMeshComponent* GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
		GateMesh->SetStaticMesh(GateMeshRef.Object);
		GateMesh->SetupAttachment(StageMesh , GateSocket);
		GateMesh->SetRelativeLocation(FVector(0.0f , -80.0f , 0.0f));
		GateMesh->SetRelativeRotation(FRotator(0.0f , -90.0f , 0.0f));
		GateMeshes.Add(GateSocket , GateMesh);

		FName TriggerBoxName = *GateSocket.ToString().Append(TEXT("TriggerBox"));
		UBoxComponent* GateTriggerBox = CreateDefaultSubobject<UBoxComponent>(TriggerBoxName);
		GateTriggerBox->SetupAttachment(StageMesh , GateSocket);
		GateTriggerBox->SetBoxExtent((FVector(100.0f , 100.0f , 300.0f)));
		GateTriggerBox->SetRelativeLocation((FVector(70.0f , 0.0f , 250.0f)));
		GateTriggerBox->SetCollisionProfileName(FName("D1Trigger"));
		GateTriggerBox->ComponentTags.Add(GateSocket);
		GateTriggerBox->OnComponentBeginOverlap.AddDynamic(this , &AD1Stagegimmick::OnGateTriggerBoxBeginOverlap);
		GateTriggerBoxes.Add(GateTriggerBox);
	}

	// GameState
	CurrentState = EStageState::READY;

	StateChangeActions.Add(EStageState::READY ,
	                       FOnStateChangedDelegate::CreateUObject(this , &AD1Stagegimmick::SetReady));
	StateChangeActions.Add(EStageState::FIGHT ,
	                       FOnStateChangedDelegate::CreateUObject(this , &AD1Stagegimmick::SetFight));
	StateChangeActions.Add(EStageState::REWARD ,
	                       FOnStateChangedDelegate::CreateUObject(this , &AD1Stagegimmick::SetChooseReward));
	StateChangeActions.Add(EStageState::NEXT ,
	                       FOnStateChangedDelegate::CreateUObject(this , &AD1Stagegimmick::SetChooseNext));

	// FightState
	static ConstructorHelpers::FClassFinder<AD1Monster> MonsterClassRef(
		TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_D1Monster.BP_D1Monster_C'"));
	if (MonsterClassRef.Succeeded())
	{
		MonsterClass = MonsterClassRef.Class;
	}

	// RewardState
	static FName RewardSockets[] = {TEXT("+XReward") , TEXT("-XReward") , TEXT("+YReward") ,TEXT("-YReward")};

	for (FName RewardSocket : RewardSockets)
	{
		FVector BoxLocation = StageMesh->GetSocketLocation(RewardSocket);
		RewardBoxLocations.Add(RewardSocket , BoxLocation);
	}
}

void AD1Stagegimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetState(CurrentState);
}


void AD1Stagegimmick::OnStageTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor ,
                                                    UPrimitiveComponent* OtherComp , int32 OtherBodyIndex ,
                                                    bool bFromSweep , const FHitResult& SweepResult)
{
	SetState(EStageState::FIGHT);
}

void AD1Stagegimmick::OnGateTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor ,
                                                   UPrimitiveComponent* OtherComp , int32 OtherBodyIndex ,
                                                   bool bFromSweep , const FHitResult& SweepResult)
{
	check(OverlappedComponent -> ComponentTags.Num() == 1)
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	check(StageMesh->DoesSocketExist(SocketName));

	FVector NewLocation = StageMesh->GetSocketLocation(SocketName);
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams ColiisionQueryParam;
	ColiisionQueryParam.AddIgnoredActor(this);


	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults ,
		NewLocation ,
		FQuat::Identity ,
		FCollisionObjectQueryParams::InitType::AllObjects ,
		FCollisionShape::MakeSphere(775.0f) ,
		ColiisionQueryParam
	);

	if (!bResult)
	{
		FTransform NewTransform(NewLocation);
		AD1Stagegimmick* NewGimmick = GetWorld()->SpawnActorDeferred<AD1Stagegimmick>(
			AD1Stagegimmick::StaticClass() , NewTransform);

		if (NewGimmick)
		{
			NewGimmick->SetStageLevel(CurrentStageLevel + 1);
			NewGimmick->FinishSpawning(NewTransform);
		}
	}
}

void AD1Stagegimmick::OpenAllGates()
{
	for (const auto GateMesh : GateMeshes)
	{
		(GateMesh.Value)->SetRelativeRotation(FRotator(0.0f , -90.0f , 0.0f));
	}
}

void AD1Stagegimmick::CloseAllGates()
{
	for (const auto GateMesh : GateMeshes)
	{
		(GateMesh.Value)->SetRelativeRotation(FRotator(0.0f , 0.0f , 0.0f));
	}
}

void AD1Stagegimmick::SetState(EStageState InNameState)
{
	CurrentState = InNameState;

	if (StateChangeActions.Contains(CurrentState))
	{
		StateChangeActions[CurrentState].ExecuteIfBound();
	}
}

void AD1Stagegimmick::SetReady()
{
	StageTriggerBox->SetCollisionProfileName(FName("D1Trigger"));
	for (auto GateTriggerBox : GateTriggerBoxes)
	{
		GateTriggerBox->SetCollisionProfileName(FName("NoCollision"));
	}

	OpenAllGates();
}

void AD1Stagegimmick::SetFight()
{
	StageTriggerBox->SetCollisionProfileName(FName("D1Trigger"));
	for (auto GateTriggerBox : GateTriggerBoxes)
	{
		GateTriggerBox->SetCollisionProfileName(FName("NoCollision"));
	}

	CloseAllGates();

	// 몬스터 스폰
	GetWorld()->GetTimerManager().SetTimer(MonsterSpawnTimerHandle , this , &AD1Stagegimmick::OnMonsterSpawn ,
	                                       MonsterSpawnTime , false);
}

void AD1Stagegimmick::SetChooseReward()
{
	StageTriggerBox->SetCollisionProfileName(FName("D1Trigger"));
	for (auto GateTriggerBox : GateTriggerBoxes)
	{
		GateTriggerBox->SetCollisionProfileName(FName("NoCollision"));
	}

	CloseAllGates();

	SpawnRewardBoxes();
}

void AD1Stagegimmick::SetChooseNext()
{
	StageTriggerBox->SetCollisionProfileName(FName("NoCollision"));
	for (auto gateTriggerBox : GateTriggerBoxes)
	{
		gateTriggerBox->SetCollisionProfileName(FName("D1Trigger"));
	}

	OpenAllGates();
}

void AD1Stagegimmick::OnMonsterSpawn()
{
	const FTransform SpawnTransform(GetActorLocation() + FVector::UpVector * 88.0f);
	AD1Monster* NewMonster = GetWorld()->SpawnActorDeferred<AD1Monster>(MonsterClass , SpawnTransform);
	if (NewMonster)
	{
		NewMonster->OnDestroyed.AddDynamic(this , &AD1Stagegimmick::OnMonsterDestroyed);
		NewMonster->SetLevel(CurrentStageLevel);
		NewMonster->FinishSpawning(SpawnTransform);
	}
}

void AD1Stagegimmick::OnMonsterDestroyed(AActor* DestoryedActor)
{
	ID1GameInterface* D1GameMode = Cast<ID1GameInterface>(GetWorld()->GetAuthGameMode());
	if (D1GameMode)
	{
		D1GameMode->AddPlayerScore(1);
		if (D1GameMode->IsGameCleared())
		{
			return;
		}
	}

	SetState(EStageState::REWARD);
}

void AD1Stagegimmick::SpawnRewardBoxes()
{
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		FTransform SpawnTransform(GetActorLocation() + RewardBoxLocation.Value + FVector(0.0f , 0.0f , 30.0f));
		AD1ItemBox* RewardBoxActor = GetWorld()->SpawnActorDeferred<AD1ItemBox>(
			AD1ItemBox::StaticClass() , SpawnTransform);

		if (RewardBoxActor)
		{
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key);
			RewardBoxActor->GetTriggerBox()->OnComponentBeginOverlap.AddDynamic(
				this , &AD1Stagegimmick::OnRewardBoxBegionOverlap);
			RewardBoxes.Add(RewardBoxActor);
		}
	}

	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			RewardBox.Get()->FinishSpawning(RewardBox.Get()->GetActorTransform());
		}
	}
}

void AD1Stagegimmick::OnRewardBoxBegionOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor ,
                                               UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep ,
                                               const FHitResult& SweepResult)
{
	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			AD1ItemBox* ValidItemBox = RewardBox.Get();
			AActor* OverlapBox = OverlappedComponent->GetOwner();
			if (OverlapBox != ValidItemBox)
			{
				ValidItemBox->Destroy();
			}
		}
	}

	SetState(EStageState::NEXT);
}
