/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

Weaponbow:  Overrides swing from weapon swords
			Gets the location of the "muzzle"
*/


#include "WeaponBow.h"

void AWeaponBow::Swing()
{
	//checks if the player has a projecitleclass item
	AActor* MyOwner = GetOwner();
	if (MyOwner && ProjectileClass)
	{
		//gets the eye location for the pawn
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		//gets the muzzlelocation for the weapon
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		//spawns the bow
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//spawn the projectile
		GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParams);
	}
}