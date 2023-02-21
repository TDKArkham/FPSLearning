// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UFPGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSLEARNING_API IFPGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	
	/**
	 * @brief 
	 * @param InstigateActor The actor which initiates this interaction.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(AActor* InstigateActor);
};
