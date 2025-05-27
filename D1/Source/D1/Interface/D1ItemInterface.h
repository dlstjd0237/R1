// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "D1ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UD1ItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class D1_API ID1ItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	
public : 
	virtual void TakeItem(class UD1ItemData* InItemData) = 0;


};
