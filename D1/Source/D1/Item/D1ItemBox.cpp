// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/D1ItemBox.h"

#include "Chaos/PBDRigidClustering.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Interface/D1ItemInterface.h"
#include "Engine/AssetManager.h"
#include "Item/D1ItemData.h"

// Sets default values
AD1ItemBox::AD1ItemBox()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetCollisionProfileName(FName("D1Trigger"));
	TriggerBox->SetBoxExtent(FVector(40.0f , 42.0f , 30.0f));
	


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetCollisionProfileName(FName("NoCollision"));
	Mesh->SetRelativeLocation(FVector(0.0f , 0.0f , -30.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT(
		"/Script/Engine.StaticMesh'/Game/_Art/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (MeshRef.Succeeded())
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	Effect->SetupAttachment(GetRootComponent());
	Effect->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT(
		"/Script/Engine.ParticleSystem'/Game/_Art/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectRef.Succeeded())
	{
		Effect->SetTemplate(EffectRef.Object);
	}
}

void AD1ItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this , &AD1ItemBox::OnBeginOverlap);

	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("D1ItemData") , Assets);
	ensure(Assets.Num() > 0);

	int32 RandomIndex = FMath::RandRange(0 , Assets.Num() - 1);
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}

	ItemData = Cast<UD1ItemData>(AssetPtr.Get());
	ensure(ItemData);
}

void AD1ItemBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor ,
                                UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep ,
                                const FHitResult& SweepResult)
{
	//충돌 확인
	if (nullptr == ItemData)
	{
		Destroy();
		return;
	}

	ID1ItemInterface* OverlappingCharacter = Cast<ID1ItemInterface>(OtherActor);
	if (OverlappingCharacter)
	{
		OverlappingCharacter->TakeItem(ItemData);
	}


	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	Effect->Activate();
	Effect->OnSystemFinished.AddDynamic(this , &AD1ItemBox::OnBoxEffectFinished);
}

void AD1ItemBox::OnBoxEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}
