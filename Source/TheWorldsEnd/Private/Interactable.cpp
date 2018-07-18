/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

Interactable:  Gets a name to add to items and an action to preform
Displays the text of what the player is looking at and what they should do
*/

#include "Interactable.h"


// Sets default values
AInteractable::AInteractable()
{
	Name = "Please Give me a Name";	//defult name
	Action = "Interact";	//defult interaction
}


//displays text of the objects name that the player is looking at and F to preform the correct action
FString AInteractable::GetUseText() const
{
	return FString::Printf(TEXT("%s: Press F to %s"), *Name, *Action);
}