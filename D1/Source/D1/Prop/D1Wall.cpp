// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/D1Wall.h"

// Sets default values
AD1Wall::AD1Wall()
{
	// ==== Mesh Settings ====
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	
	BackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackMesh"));
	BackMesh->SetupAttachment(Mesh);
	BackMesh->SetRelativeLocation(FVector(300, -25, 0));
	BackMesh->SetRelativeRotation(FRotator(0, -180, 0));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/RestaurantScene/Meshes/SM_Wall01.SM_Wall01'"));
	
	if (MeshRef.Succeeded())
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	BackMesh->SetStaticMesh(MeshRef.Object);
	}
	// =======================

}



