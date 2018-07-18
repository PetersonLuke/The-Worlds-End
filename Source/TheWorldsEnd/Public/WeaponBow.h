/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

Weaponbow:  Overrides swing from weapon swords
			allows creation of a projectile blueprint
*/


#pragma once

#include "CoreMinimal.h"
#include "WeaponSwords.h"
#include "WeaponBow.generated.h"

UCLASS()
class THEWORLDSEND_API AWeaponBow : public AWeaponSwords
{
	GENERATED_BODY()
	
protected:

	//over rides the swing
	virtual void Swing() override;
	
	//allows the create of a projectile class Blueprint for the weapon to fire
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon")
	TSubclassOf<AActor> ProjectileClass;
};