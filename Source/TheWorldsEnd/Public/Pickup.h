/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

Pickup:		lets a mesh be given
			Allows and id to be given
*/


#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Pickup.generated.h"

UCLASS()
class THEWORLDSEND_API APickup : public AInteractable
{
	GENERATED_BODY()

public:

	APickup();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Pickup")
	void OnUsed();

protected:

	//lets a mesh be given
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PickupMesh;
	
	//Enables Item ID option in unreal
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
};