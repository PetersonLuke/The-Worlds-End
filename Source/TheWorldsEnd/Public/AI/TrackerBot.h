/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

TrackerBot:  This is a bot that looks for the player and explodes when it is near them

It is given health
A sphere that incompasses it
can be delt damage
gets a path point to the player
has a force of movement for how fast it should move
gets the distance to the target
can kill itself to start an explosion that deals damage to the player
checks to see if it is overlaping a player

*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TrackerBot.generated.h"

class UHealthComponent;
class USphereComponent;

UCLASS()
class THEWORLDSEND_API ATrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//lets a mesh be added
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	UStaticMeshComponent* MeshComp;

	//adds health to the AI
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	UHealthComponent* HealthComp;

	//adds a sphere to the AI
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	USphereComponent* SphereComp;

	//Handles the damage taken by the AI
	UFUNCTION()
	void HandleTakeDamage(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//gets the next path
	FVector GetNextPathPoint();

	//next point in the nav path
	FVector NextPathPoint;

	//the force of movment
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bUseVelocityChange;

	//the distance to the target
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDistanceToTarget;

	//Dynamic material to pulse on damage
	UMaterialInstanceDynamic* MatInst;

	//kills itself
	void SelfDestruct();

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	UParticleSystem* ExplosionEffect;

	//if it has exploded or not
	bool bExploded;

	//if it has started to explode
	bool bStartedSelfDestruction;

	//how big the explosion radius is
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionRadius;

	//how much damage should be delt
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionDamage;

	//how fast it can damage itself to explode
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float SelfDamageInterval;

	//time for damaging itseld
	FTimerHandle TimerHandle_SelfDamage;

	void DamageSelf();

	//timer for how long it should wait before it shoudl refreshpath if it gets stuck
	FTimerHandle TimerHandle_RefreshPath;

	//refreshes the path if it gets stuck
	void RefreshPath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//checks if it is overlaping another actor
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};