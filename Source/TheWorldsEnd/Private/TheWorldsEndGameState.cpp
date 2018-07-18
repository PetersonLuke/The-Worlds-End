/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

TheWordsEndGameStat:  Handles the waves of bots to get spawned

*/


#include "TheWorldsEndGameState.h"
#include "Net/UnrealNetwork.h"


void ATheWorldsEndGameState::OnRep_WaveState(EWaveState OldState)
{
	WaveStateChanged(WaveState, OldState);
}


void ATheWorldsEndGameState::SetWaveState(EWaveState NewState)
{
	if (Role == ROLE_Authority)
	{
		EWaveState OldState = WaveState;

		WaveState = NewState;
		OnRep_WaveState(OldState);
	}
}


//Magical multiplayer additional stuff
void ATheWorldsEndGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATheWorldsEndGameState, WaveState);
}