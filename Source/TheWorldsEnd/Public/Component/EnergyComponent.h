/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

EnergyComponent:  This give energy to the player

*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnergyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEWORLDSEND_API UEnergyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnergyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//value for energy
	float Energy;

	//the default amount of energy can be changed in the blueprints
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnergyComponent")
	float DefaultEnergy;

public:	

	//teh amount of energy
	float GetEnergy() const;

	UFUNCTION(BlueprintCallable, Category = "EnergyComponent")
	void UseEnergy(float EnergyUse);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};