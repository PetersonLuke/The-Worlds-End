/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

TheWordsEndGameStat:  Handles the waves of bots to get spawned

*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TheWorldsEndGameState.generated.h"


//used for debug
UENUM(BlueprintType)
enum class EWaveState : uint8
{
	//waiting to start
	WaitingToStart,

	//waiting for bots to be killed
	WaveInProgress,

	//no longer spawning new bots, waiting for players to kill remaning bots
	WaitingToComplete,

	//all bots killed
	WaveComplete,

	//all players killed
	GameOver,
};


UCLASS()
class THEWORLDSEND_API ATheWorldsEndGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
			
	UFUNCTION()
	void OnRep_WaveState(EWaveState OldState);
			
	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
	void WaveStateChanged(EWaveState NewState, EWaveState OldState);
			
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
	EWaveState WaveState;
			
public:
			
	void SetWaveState(EWaveState NewState);
	
};