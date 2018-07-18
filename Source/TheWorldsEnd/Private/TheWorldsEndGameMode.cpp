/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

TheWorldsEndGameMode:  Deals with when to spawn AI and inventory

*/


#include "TheWorldsEndGameMode.h"
#include "HealthComponent.h"
#include "TheWorldsEndGameState.h"
#include "TheWorldsEndPlayerState.h"
#include "TimerManager.h"


//Constructor
ATheWorldsEndGameMode::ATheWorldsEndGameMode()
{
	//how long it takes for a new wave to state after the old one ended
	TimeBetweenWaves = 2.0f;

	GameStateClass = ATheWorldsEndGameState::StaticClass();
	PlayerStateClass = ATheWorldsEndPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}


//Start Wave
void ATheWorldsEndGameMode::StartWave()
{
	//how many bots should be spawned  (done this way until night time is made)
	WaveCount++;
	NrOfBotsToSpawn = 10 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ATheWorldsEndGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);

	//set the wave state to wave in progress
	SetWaveState(EWaveState::WaveInProgress);
}


//End Wave
void ATheWorldsEndGameMode::EndWave()
{
	//restarts the timer
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	//sets the wave state to waiting to complete
	SetWaveState(EWaveState::WaitingToComplete);
}


//Prepare for next wave
void ATheWorldsEndGameMode::PrepareForNextWave()
{
	//sets the timers
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ATheWorldsEndGameMode::StartWave, TimeBetweenWaves, false);

	//sets the game state to waiting to start
	SetWaveState(EWaveState::WaitingToStart);

	//dead players come alive!  It is a miracle
	RestartDeadPlayers();
}


//Check to see if any bots are alive
void ATheWorldsEndGameMode::CheckWaveState()
{
	//Check to see if preparing for the next wave
	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	//Checks if bots are going to spawn or if the next wave is preparing
	if (NrOfBotsToSpawn > 0 || bIsPreparingForWave)
	{
		return;
	}

	//if no bots are alive set it to false
	bool bIsAnyBotAlive = false;

	//looking for all the pawns in the world
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}

		//gets the health of the bots
		UHealthComponent* HealthComp = Cast<UHealthComponent>(TestPawn->GetComponentByClass(UHealthComponent::StaticClass()));
		if (HealthComp && HealthComp->GetHealth() > 0.0f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	//checks if all bots are dead
	if (!bIsAnyBotAlive)
	{
		//sets the game state to wave complete
		SetWaveState(EWaveState::WaveComplete);

		PrepareForNextWave();
	}
}

//checks if any player is alive
void ATheWorldsEndGameMode::CheckAnyPlayerAlive()
{
	//looks for all the player controled pawns in the world
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		//checks if the player has health remaining
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			UHealthComponent* HealthComp = Cast<UHealthComponent>(MyPawn->GetComponentByClass(UHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				//A player is still alive
				return;
			}
		}
	}
	
	//all players are dead
	GameOver();
}


//Game over
void ATheWorldsEndGameMode::GameOver()
{
	EndWave();

	//sets the state to Game Over
	SetWaveState(EWaveState::GameOver);

	UE_LOG(LogTemp, Log, TEXT("GAME OVER!"));
}


void ATheWorldsEndGameMode::SetWaveState(EWaveState NewState)
{
	ATheWorldsEndGameState* GS = GetGameState<ATheWorldsEndGameState>();
	if (ensureAlways(GS))
	{
		GS->SetWaveState(NewState);
	}
}

//restarts the players
void ATheWorldsEndGameMode::RestartDeadPlayers()
{
	//looks for all the player controled pawns in the world
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		//checks if the player has health remaining
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
}


//Start Play
void ATheWorldsEndGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}


void ATheWorldsEndGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//cheacks the wave state every second
	CheckWaveState();
	CheckAnyPlayerAlive();
}

//gets the item data base
UDataTable * ATheWorldsEndGameMode::GetItemDB() const
{
	return ItemDB;
}


//Spawn bot Timer Elapsed
void ATheWorldsEndGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NrOfBotsToSpawn--;

	if (NrOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}