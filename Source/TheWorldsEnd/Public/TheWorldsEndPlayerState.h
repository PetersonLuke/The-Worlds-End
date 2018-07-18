/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

TheWorldsEndPlayerState:  Simple adds "XP" to the playe when the get a kill
*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TheWorldsEndPlayerState.generated.h"

UCLASS()
class THEWORLDSEND_API ATheWorldsEndPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	//adds score to the player
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void AddScore(float ScoreDelta);
	
};