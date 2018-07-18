/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

Interactable:  Gets a name to add to items and an action to preform
Displays the text of what the player is looking at and what they should do
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class THEWORLDSEND_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:

	AInteractable();
	
	UFUNCTION(BlueprintImplementableEvent)
	void Interact(APlayerController* Controller);

	//takes a name
	UPROPERTY(EditDefaultsOnly)
	FString Name;

	//takes an action
	UPROPERTY(EditDefaultsOnly)
	FString Action;

	//gets text if the player is looking at an interactable object
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	FString GetUseText() const;
};