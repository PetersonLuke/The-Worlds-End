/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

GamePlaycontroller:  This is used to get crafting information,
if the player is looking at in an interatable object,
and adding items to the players inventory

*/

#include "GameplayController.h"
#include "Interactable.h"
#include "TheWorldsEndGameMode.h"
#include "MCharacter.h"
#include "TheWorldsEndGameState.h"

void AGameplayController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Use", IE_Pressed, this, &AGameplayController::Interact);
}


//deals with being able to craft items.  It takes and Itam ID A Item ID B and outputs item ID C
void AGameplayController::CraftItem(FInventoryItem ItemA, FInventoryItem ItemB, AGameplayController * Controller)
{
	for (auto Craft : ItemB.CraftCombinations)
	{
		//checks if the item can be crafted
		if (Craft.ComponentID == ItemA.ItemID)
		{
			//checks if Item A should be deleted
			if (Craft.bDestoryItemA)
			{
				//if destory is true the item is removed from the inventory
				Inventory.RemoveSingle(ItemA);
			}

			//checks if Item B should be deleted
			if (Craft.bDestoryItemB)
			{
				//if destory is true the item is removed from the inventory
				Inventory.RemoveSingle(ItemB);
			}

			//if so return the products ID
			AddItemToInventoryByID(Craft.ProductID);

			//reloads the inventory after a crafting action has taken place
			ReloadInventory();
		}
	}
}


//looking for a current interactable.  information recived from Interatables class
void AGameplayController::Interact()
{
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(this);
	}
}


//Adds an item to the inventory based of their ID
void AGameplayController::AddItemToInventoryByID(FName ID)
{
	ATheWorldsEndGameMode* GameMode = Cast<ATheWorldsEndGameMode>(GetWorld()->GetAuthGameMode());	//gets the gamemode for the items stored
	UDataTable* ItemTable = GameMode->GetItemDB();													//gets the item by ID

	//adds the item into an available row
	FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");

	//looks to see if it can add the item
	if (ItemToAdd)
	{
		//If item is valid it will add it
		Inventory.Add(*ItemToAdd);
	}
}