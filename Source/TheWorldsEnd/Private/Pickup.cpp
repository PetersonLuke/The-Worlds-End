/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

Pickup:		creates a mesh of the item in the inventory
			Allows and id to be given
*/


#include "Pickup.h"

APickup::APickup()
{
	//lets a mesh be given
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");

	//id for the item
	ItemID = FName("Please enter an ID");
}