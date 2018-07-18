/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

HungerCompionent:  Gives a hunger bar to the playerPawn

*/

#include "HungerComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UHungerComponent::UHungerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//normal amount
	DefaultHunger = 100;

	SetIsReplicated(true);
}


// Called when the game starts
void UHungerComponent::BeginPlay()
{
	Super::BeginPlay();

	Hunger = DefaultHunger;
}


float UHungerComponent::GetHunger() const
{
	return Hunger;
}

// Called every frame
void UHungerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}