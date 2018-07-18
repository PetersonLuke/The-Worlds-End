/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

TheWorldsEndGameMode:  Deals with when to spawn AI and inventory

*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TheWorldsEndGameMode.generated.h"

enum class EWaveState : uint8;

//Victim actor, killer actor, killer actor controller
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

UCLASS()
class THEWORLDSEND_API ATheWorldsEndGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_NextWaveStart;

	//the amount of bots to spawn in the current wave
	int32 NrOfBotsToSpawn;

	int32 WaveCount;

	//how long before the next waves come
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves;

protected:
	
	//allows BP to spawn a single bot
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	//start spawning the bots
	void StartWave();

	//Stop spawning the bots
	void EndWave();

	//Set timer for the next set of wave
	void PrepareForNextWave();

	void CheckWaveState();

	//checks if any players are alive
	void CheckAnyPlayerAlive();

	//game over if all players are dead
	void GameOver();

	void SetWaveState(EWaveState NewState);

	//restart dead players
	void RestartDeadPlayers();

	//item data base for inventory
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ItemDB;

public:

	ATheWorldsEndGameMode();

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnActorKilled OnActorKilled;

	class UDataTable* GetItemDB() const;
};