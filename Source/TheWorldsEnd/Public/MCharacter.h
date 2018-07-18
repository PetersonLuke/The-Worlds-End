/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

MCharacter:  This is what creates the pawn that the player will controlle
It handles Crafting inforamtion getting components ids, the product ID and if the components should be destroyed
inventory information is also used.  It gets the items ID name action used, thumbnail used in inventory
descrption for the item, crafting combinations that can be done,and if it can be used

Movments:	Forwards and backwards movments are created
			Left and right movments
			crouching

Sets the energy, health, hunger components to the Blueprint playerPawn
Gets the current weapon being used by the playerPawn
Sets what is the starting weapon for the playerPawn
"attaches" the weapon to the socket on the playerPawn
deals with health changes
Checks if dead
Checks if the player is looking at interactables
Gets player input
Gets the playPawns view location
Checks when the player starts swinging
Checks when the player stops swinging

*/


#pragma once

#include "Engine.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "MCharacter.generated.h"

class UCameraComponent;
class AWeaponSwords;
class UHealthComponent;
class UEnergyComponent;
class UHungerComponent;

//Handles the crafting information for the player
USTRUCT(BlueprintType)
struct FCraftingInfo : public FTableRowBase
{
	GENERATED_BODY()

public:

	//item being used to create and item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ComponentID;

	//item being made
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ProductID;

	//If the item A being creafted with is to be destroyed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDestoryItemA;

	//If the item B being creafted should be destroyed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDestoryItemB;
};


//deals with inventory information
USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_BODY()

public:

	FInventoryItem()
	{
		Name = FText::FromString("Name of Item");	//Default Item name
		Action = FText::FromString("Use");	//Defualt action
		Description = FText::FromString("Item needs a discription");	//Descritption for the item
	}

	//Allows setting item ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	//allows to make the item pickupable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class APickup> ItemPickup;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWeaponSwords> ItemSwords;

	//allows setting the item name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	//Allows setting what action to preform
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Action;

	//Allows creating a thumbail to store the inventory iteam as
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	//Gives a description of the item, well it is in your inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	//Holds all the crafting combinations
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCraftingInfo> CraftCombinations;

	//if the item can be used or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeUsed;

	//Operator overload used to compair inventory items
	bool operator==(const FInventoryItem& Item) const
	{
		if (ItemID == Item.ItemID)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

UCLASS()
class THEWORLDSEND_API AMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//added from tutorial
	//forwards and backwards keyboard movements
	void ForwardBackward(float Value);	
	
	//Left and right keyboard movements
	void LeftRight(float Value);		

	//crouching for chracter
	void BeginCrouch();
	void EndCrouch();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Components")
	UHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Components")
	UEnergyComponent* EnergyComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Components")
	UHungerComponent* HungerComp;

	//This UPROPERTY will let the weapon functions to be used in multiplayer
	UPROPERTY(Replicated)
	AWeaponSwords* CurrentWeapon;

	//The weapon/tool you start off with
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<AWeaponSwords> StarterWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bDied;

	//Checks for items that can be interacted with
	void CheckForInteractables();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	//starting swing on left mouse button click or hold
	UFUNCTION(Blueprintcallable, Category = "Player")
	void StartSwing();

	//stop swing on lift of left mouse button
	UFUNCTION(Blueprintcallable, Category = "Player")
	void StopSwing();
};