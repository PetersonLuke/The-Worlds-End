/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

WeaponsSwords:  This handles if the player has hit a target and deals damage to them

Lets stuff spawn when attacking
The type of damage delt
The location of the end of the weapon (such as where the arrow should spawn when the bow is fired
Base damage for a weapon
Swinging of the weapons, and server handaling of the swing
how fast a weapon can swing
starting and ending of swing on mouse button hold

*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponSwords.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;

//Contains infomration of a single hitscan weapon linetrace
USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FVector_NetQuantize TraceTo;
};

UCLASS()
class THEWORLDSEND_API AWeaponSwords : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponSwords();

protected:
	
	virtual void BeginPlay() override;

	//adds being able give a mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	//this is used to add effects when used
	//such as, allow the bow to spawn a mash when it fires
	void PlaySwingEffects(FVector TraceEnd);

	//the type of damage delt
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	//the point of the weapon where objects are going to appear
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	//the "normal" starting damaga for a created item
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;

	//the attack
	virtual void Swing();

	//makes the server do the swing funcation
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSwing();

	//the time between attacks
	FTimerHandle TimerHandle_TimeBetweenAttacks;

	//the last time the player attacked
	float LastSwingedTime;

	//how fast you can swing on mouse hold
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOffSwing;

	float TimeBetweenSwings;

	//for debug
	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	//for debug
	UFUNCTION()
	void OnRep_HitScanTrace();

	//Enables Item ID option in unreal
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

public:

	//start of swing
	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartSwing();

	//end of swing
	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopSwing();
};