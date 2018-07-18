/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018


*/


#include "PickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "PowerupActor.h"
#include "TimerManager.h"


// Sets default values
APickupActor::APickupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90, 0.0f, 0.0f));
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetupAttachment(RootComponent);

	CooldownDuration = 10.0f;

	//makes it so that it will start off being able to be multiplayer
	SetReplicates(true);
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();	

	if (Role == ROLE_Authority)
	{
		Respawn();
	}
}

void APickupActor::Respawn()
{
	if (PowerUpClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerUpClass is nullptr in %s.  Please update your Blueprint"), *GetName());
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PowerUpInstance = GetWorld()->SpawnActor<APowerupActor>(PowerUpClass, GetTransform(), SpawnParams);
}

void APickupActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Role == ROLE_Authority && PowerUpInstance)
	{
		PowerUpInstance->ActivatePowerup(OtherActor);
		PowerUpInstance = nullptr;

		//set timer to respawn
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &APickupActor::Respawn, CooldownDuration);
	}
}