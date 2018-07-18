/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

WorldGeneration:  This is used to generate the blocks the player can interacte with in the world

*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.h"
#include "WorldGeneration.generated.h"

UCLASS()
class THEWORLDSEND_API AWorldGeneration : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldGeneration();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	//Gets an X location
	float generateX(float oldUsedX);		

	//Gets a Y location
	float generateY(float oldUsedY);

	//Gets a X location for the second spawning area
	float generateSmallX(float oldUsedX);

	//Gets a Y location for the second spawning area
	float generateSmallY(float oldUsedY);

	bool isUsed(TArray<float> &oldArray, float numCheck);

	//distance between each block
	float distanceBetween(float inOne, float inTwo);

	bool isOneGreater(float inOne, float inTwo);

	//unreal array to hold X values
	TArray<float> oldX;	

	//Unreal array to hold Y values
	TArray<float> oldY;	

	//unreal array to hold X values
	TArray<float> oldSmallX;	

	//Unreal array to hold Y values
	TArray<float> oldSmallY;	

	//X value for first location
	float newX;

	//Y value for first location
	float newY;

	//Z value
	float newZ;

	//X value for second location
	float newSmallX;

	//Y value for second location
	float newSmallY;

protected:

	//Lets a Blueprint in the world generation blueprint to be set to spawn an item here
	UPROPERTY(EditDefaultsOnly, Category = "Tin")	
	TSubclassOf<APickup> blockTin;

	//Lets a Blueprint in the world generation blueprint to be set to spawn an item here
	UPROPERTY(EditDefaultsOnly, Category = "Stone")	
	TSubclassOf<APickup> blockStone;

	//Lets a Blueprint in the world generation blueprint to be set to spawn an item here
	UPROPERTY(EditDefaultsOnly, Category = "Dirt")	
	TSubclassOf<APickup> blockDirt;

	//Lets a Blueprint in the world generation blueprint to be set to spawn an item here 
	UPROPERTY(EditDefaultsOnly, Category = "Grass")	
	TSubclassOf<APickup> blockGrass;
};