/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

HungerCompionent:  Gives a hunger bar to the playerPawn

*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HungerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEWORLDSEND_API UHungerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHungerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//value of hunger
	float Hunger;

	//the default hunger amount.  can be changed in blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HungerComponent")
	float DefaultHunger;

public:	

	//gets hunger
	float GetHunger() const;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};