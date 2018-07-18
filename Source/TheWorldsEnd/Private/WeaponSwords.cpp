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
handaling of muliplayer attackings is also delt with here

*/


#include "WeaponSwords.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TheWorldsEnd.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

//used to enable or disapble DebugWeaponDrawing
static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("COOP.DebugWeapons"), 
	DebugWeaponDrawing, 
	TEXT("Draw Debug Lines for Weapons"), 
	ECVF_Cheat);


// Sets default values
AWeaponSwords::AWeaponSwords()
{
	
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	//gives name to socket to be attached at the spawning point of projectials
	MuzzleSocketName = "MuzzleSocket";

	//starting damage for a weapon
	BaseDamage = 15.0f;

	//how fast you can swing
	RateOffSwing = 60;

	//This lets the weapons be created for multiplayer
	SetReplicates(true);

	//how much it will update
	NetUpdateFrequency = 66.0f;
	//the minimum update fraguency to prevent lag
	MinNetUpdateFrequency = 33.0f;

	//Id for the item
	ItemID = FName("Please enter an ID");
}


void AWeaponSwords::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenSwings = 60 / RateOffSwing;
}


void AWeaponSwords::Swing()
{
	//allows multiplayer magical stuff
	if (Role < ROLE_Authority)
	{
		ServerSwing();
	}

	//gets the owner
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		//gets the location the player is looking
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		//the direction you hit
		FVector ShotDirection = EyeRotation.Vector();

		//the range you can hit the enemy
		FVector TraceEnd = EyeLocation + (ShotDirection * 500);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		//Partivle "Target" Parameter
		FVector TracerEndPoint = TraceEnd;

		//checks for a hit
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
		{
			//blocking hit process damage

			AActor* HitActor = Hit.GetActor();

			UGameplayStatics::ApplyPointDamage(HitActor, BaseDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), MyOwner, DamageType);

			TracerEndPoint = Hit.ImpactPoint;
		}

		//checks to see if the debug drawing appears
		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Blue, false, 1.0f, 0, 1.0f);
		}	

		PlaySwingEffects(TracerEndPoint);

		if (Role == ROLE_Authority)
		{
			HitScanTrace.TraceTo = TracerEndPoint;
		}

		//gets the last time the pawns weapon has swong
		LastSwingedTime = GetWorld()->TimeSeconds;
	}
}


//used for debug
void AWeaponSwords::OnRep_HitScanTrace()
{
	PlaySwingEffects(HitScanTrace.TraceTo);

}


//starting of a swing on button pressed
void AWeaponSwords::StartSwing()
{
	//picks the greatest value
	float FirstDelay = FMath::Max(LastSwingedTime + TimeBetweenSwings - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenAttacks, this, &AWeaponSwords::Swing, TimeBetweenSwings, true, FirstDelay);
}


//stoping of a swing on button lift
void AWeaponSwords::StopSwing()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenAttacks);
}


//multiplayer swing stuff
void AWeaponSwords::ServerSwing_Implementation()
{
	Swing();
}


//multiplayer swing stuff
bool AWeaponSwords::ServerSwing_Validate()
{
	return true;
}


//lets objects spawn at the muzzlesocket location
void AWeaponSwords::PlaySwingEffects(FVector TraceEnd)
{
	FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
}


//magiacal multiplayer stuff
void AWeaponSwords::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AWeaponSwords, HitScanTrace, COND_SkipOwner);
}