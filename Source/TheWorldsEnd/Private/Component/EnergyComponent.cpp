/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

EnergyComponent:  This give energy to the player

*/

#include "EnergyComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UEnergyComponent::UEnergyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DefaultEnergy = 10;

	SetIsReplicated(true);
}


// Called when the game starts
void UEnergyComponent::BeginPlay()
{
	Super::BeginPlay();

	Energy = DefaultEnergy;
}


float UEnergyComponent::GetEnergy() const
{
	return Energy;
}

void UEnergyComponent::UseEnergy(float EnergyUse)
{
	if (Energy < 1)
	{
		return;
	}

	//Energy = FMath::Clamp(Energy + UseEnergy, 0.0f, DefaultEnergy);
}

// Called every frame
void UEnergyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	Energy = Energy + .01f;
}

//needed to allow muiltiplayer
//void UEnergyComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	DOREPLIFETIME(UEnergyComponent, Energy);
//}