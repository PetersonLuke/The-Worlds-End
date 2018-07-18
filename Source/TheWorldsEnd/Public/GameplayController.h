/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

GamePlaycontroller:  This is used to get crafting information,
if the player is looking at in an interatable object,
and adding items to the players inventory
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MCharacter.h"
#include "Interactable.h"
#include "GameplayController.generated.h"


UCLASS()
class THEWORLDSEND_API AGameplayController : public APlayerController
{
	GENERATED_BODY()
	
public:

	//used to craft items
	UFUNCTION(BlueprintCallable, Category = "Utils")
	void CraftItem(FInventoryItem ItemA, FInventoryItem ItemB, AGameplayController* Controller);
	
	//used to reload the inventory
	UFUNCTION(BlueprintImplementableEvent)
	void ReloadInventory();

	//gets the current interabtable from the interabtable class
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AInteractable* CurrentInteractable;

	//used to add an item to inventory by ID
	UFUNCTION(BlueprintCallable, Category = "Utils")
	void AddItemToInventoryByID(FName ID);

	//an array for teh inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FInventoryItem> Inventory;

protected:

	void Interact();

	virtual void SetupInputComponent() override;
};