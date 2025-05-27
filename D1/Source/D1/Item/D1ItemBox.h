// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D1ItemBox.generated.h"

UCLASS()

class D1_API AD1ItemBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AD1ItemBox();

protected:
	virtual void PostInitializeComponents() override;

public :
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp ,
	                    int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEffectFinished(class UParticleSystemComponent* PSystem);

public :
	TObjectPtr<class UBoxComponent> GetTriggerBox() {return TriggerBox;}
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> TriggerBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UParticleSystemComponent> Effect;

	UPROPERTY(EditAnywhere , Category = Item)
	TObjectPtr<class UD1ItemData> ItemData;
};
