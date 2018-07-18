/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018


*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

class USphereComponent;
class UDecalComponent;
class APowerupActor;

UCLASS()
class THEWORLDSEND_API APickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComp;

	//using EditInstanceonly vs EditDefaultsonly allows to place the manualy powerup class into the world
	//when the powerup class is in the world
	//You can then change what it is spawning with
	//else you would make multiple BP and have them be differant
	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
	TSubclassOf<APowerupActor> PowerUpClass;

	APowerupActor* PowerUpInstance;

	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
	float CooldownDuration;

	FTimerHandle TimerHandle_RespawnTimer;

	void Respawn();

public:

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};