/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

TheWorldsEndPlayerState:  Simple adds "XP" to the playe when the get a kill
*/


#include "TheWorldsEndPlayerState.h"

//adds score
void ATheWorldsEndPlayerState::AddScore(float ScoreDelta)
{
	//increases the score
	Score += ScoreDelta;
}