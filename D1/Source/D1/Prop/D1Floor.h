// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D1Floor.generated.h"

UCLASS()
class D1_API AD1Floor : public AActor
{
	GENERATED_BODY()

public:
	AD1Floor();

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> Meshes; 

	
};
