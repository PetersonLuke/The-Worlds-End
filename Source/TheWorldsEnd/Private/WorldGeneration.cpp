/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

WorldGeneration:  This is used to generate the blocks the player can interacte with in the world

*/

#include "WorldGeneration.h"
#include "Pickup.h" 
#include "TheWorldsEnd.h"

//example found at         https://www.youtube.com/watch?v=sLtFL-69Tck

// Sets default values
AWorldGeneration::AWorldGeneration()
{
	//sets the X, Y Z values to 0
	newX = 0.0f;
	newY = 0.0f;
	newZ = -15050.0f;

	newSmallX = 0.0f;
	newSmallY = 0.0f;

	oldX.SetNum(400);
	oldY.SetNum(400);

	oldSmallX.SetNum(400);
	oldSmallY.SetNum(400);
}

// Called when the game starts or when spawned
void AWorldGeneration::BeginPlay()
{
	Super::BeginPlay();

	//For lewer levels of ore
	//runs until newZ is less -10050
	while (newZ < -10050 )
	{
		//placing 100 spawnable items
		for (int32 i = 0; i < 400; i++)
		{
			newX = generateX(newX);
			newY = generateY(newY);

			oldX[i] = newX;
			oldX[i] = newY;

			//check to see if there real is a world
			const UWorld* world = GetWorld();

			if (world)
			{
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;
				spawnParams.Instigator = Instigator;

				//gets the location the object should be spawned
				FVector newVec;
				newVec.Set(newX, newY, newZ);

				//gets a number from 0 to 3
				int32 num = FMath::RandRange(0, 3);	
				//spawing if the number is 0
				if (num == 0)
				{
					AActor* NewActor = GetWorld()->SpawnActor<APickup>(blockTin, newVec, FRotator::ZeroRotator, spawnParams);
				}
				//spawning if the number is 1
				else if (num == 1)
				{
					AActor* NewActor = GetWorld()->SpawnActor<APickup>(blockStone, newVec, FRotator::ZeroRotator, spawnParams);
				}
				//spawning if the number is 2
				else if (num == 2)
				{
					AActor* NewActor = GetWorld()->SpawnActor<APickup>(blockDirt, newVec, FRotator::ZeroRotator, spawnParams);
				}
				//spawning if the number is 3
				else if (num == 3)
				{
					AActor* NewActor = GetWorld()->SpawnActor<APickup>(blockGrass, newVec, FRotator::ZeroRotator, spawnParams);
				}
			}
		}	//end for (int32 i = 0; i < 100; i++)	

		//increases the newZ by 100
		newZ = newZ + 100;

	}	//end while (newZ < -10050 )
}

//generats a X value
float AWorldGeneration::generateX(float oldUsedX)
{
	float passBack = 0.0f;

	//number from -5000 to 5000
	passBack = FMath::RandRange(-10000, 10000);
	passBack = passBack + oldUsedX;

	if (passBack < -10000.0f || passBack > 10000.0f)
	{
		passBack = generateX(oldUsedX);
	}

	if (!isUsed(oldX, passBack))
	{
		passBack = generateX(oldUsedX);
	}

	return passBack;
}


//generates Y value
float AWorldGeneration::generateY(float oldUsedY)
{
	float passBack = 0.0f;

	passBack = FMath::RandRange(-10000, 10000);
	passBack = passBack + oldUsedY;

	if (passBack < -10000.0f || passBack > 10000.0f)
	{
		passBack = generateY(oldUsedY);
	}

	if (!isUsed(oldY, passBack))
	{
		passBack = generateY(oldUsedY);
	}

	return passBack;
}


bool AWorldGeneration::isUsed(TArray<float>& oldArray, float numCheck)
{
	bool flag = true;

	for (int32 i = 0; i < oldArray.Num(); i++)
	{
		if (distanceBetween(oldArray[i], numCheck) < 100.0f)
		{
			flag = false;
		}
		
	}

	return flag;
}

float AWorldGeneration::distanceBetween(float inOne, float inTwo)
{
	float passBack = 0.0f;

	bool flag = isOneGreater(inOne, inTwo);

	if (flag == true)
	{
		passBack = inOne - inTwo;
	}
	else
	{
		passBack = inTwo - inOne;
	}

	return passBack;
}

bool AWorldGeneration::isOneGreater(float inOne, float inTwo)
{
	bool flag = true;

	if (inOne <= inTwo)
	{
		flag = false;
	}

	return flag;
}