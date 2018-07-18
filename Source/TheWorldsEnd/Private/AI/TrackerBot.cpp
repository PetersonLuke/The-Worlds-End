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

#include "TrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Navigation/NavigationSystem.h"
#include "GameFramework/Character.h"
#include "AI/Navigation/NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "HealthComponent.h"
#include "MCharacter.h"
#include "Components/SphereComponent.h"


//used to enable or disapble DebugWeaponDrawing
static int32 DebugTrackerBotDrawing = 0;
FAutoConsoleVariableRef CVARDebugTrackerBotDrawing(
	TEXT("COOP.DebugTrackerBot"),
	DebugTrackerBotDrawing,
	TEXT("Draw Debug Lines for Tracker Bot"),
	ECVF_Cheat);


// Sets default values
ATrackerBot::ATrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	//adds health to the AI
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	//Handles the damage taken by the AI
	HealthComp->OnHealthChanged.AddDynamic(this, &ATrackerBot::HandleTakeDamage);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(200);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(RootComponent);

	bUseVelocityChange = false;
	MovementForce = 1000;
	RequiredDistanceToTarget = 100;

	//amount of damage dealt
	ExplosionDamage = 40;
	//range of the explosion
	ExplosionRadius = 200;
	//how fast it can deal damage to itself
	SelfDamageInterval = 0.25f;
}


// Called when the game starts or when spawned
void ATrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
	//lets multiplayer work so that it can find other players that are not the server
	if (Role == ROLE_Authority)
	{
		//find the initial move to
		NextPathPoint = GetNextPathPoint();
	}
}


//Handling of damage taken to the AI
void ATrackerBot::HandleTakeDamage(UHealthComponent * OwningHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	
	if (MatInst == nullptr)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}

	if (MatInst)
	{
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}
	
	//Explode when hit points equles 0
	if (Health <= 0.0f)
	{
		SelfDestruct();
	}
}


//looks for the next point in the world the bot should move to
FVector ATrackerBot::GetNextPathPoint()
{
	//look for best target to attack
	AActor* BestTarget = nullptr;	
	float NearestTargetDistance = FLT_MAX;

	//use of unreal engine source code
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		
		//checking if the target is on the same team
		if (TestPawn == nullptr || UHealthComponent::IsFriendly(TestPawn, this))
		{
			continue;
		}

		//checks if the target has health
		UHealthComponent* TestPawnHealthComp = Cast<UHealthComponent>(TestPawn->GetComponentByClass(UHealthComponent::StaticClass()));
		if (TestPawnHealthComp && TestPawnHealthComp->GetHealth() > 0.0f)
		{
			//get actor locations
			float Distance = (TestPawn->GetActorLocation() - GetActorLocation()).Size();

			//looks for the closest target
			if (Distance < NearestTargetDistance)
			{
				BestTarget = TestPawn;
				NearestTargetDistance = Distance;
			}
		}
	}

	if (BestTarget)
	{
		//get path to the target
		UNavigationPath* NavPath = UNavigationSystem::FindPathToActorSynchronously(this, GetActorLocation(), BestTarget);

		//resets the timer
		GetWorldTimerManager().ClearTimer(TimerHandle_RefreshPath);

		//get new path after 5 seconds if problems happen
		GetWorldTimerManager().SetTimer(TimerHandle_RefreshPath, this, &ATrackerBot::RefreshPath, 5.0f, false);

		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			//returns the path to take
			return NavPath->PathPoints[1];
		}
	}

	//failed to find path
	return GetActorLocation();
}


//Tracker Bot self Destruct
void ATrackerBot::SelfDestruct()
{
	//if it has already exploded
	if (bExploded)
	{
		return;
	}

	//sets exploded to true
	bExploded = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	MeshComp->SetVisibility(false, true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Role == ROLE_Authority)
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);

		//apply damage
		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

		//displays information when enabled
		if (DebugTrackerBotDrawing)
		{
			DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 2.0f, 0, 1.0f);
		}

		//how long itis left after it dies
		SetLifeSpan(2.0f);
	}
}


//lets it damage itseld
void ATrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}


// Called every frame
void ATrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority && !bExploded)
	{

		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

		if (DistanceToTarget <= RequiredDistanceToTarget)
		{
			NextPathPoint = GetNextPathPoint();

			//displays information when enabled
			if (DebugTrackerBotDrawing)
			{
				DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached!");
			}
			
		}
		else
		{
			//keep moving to the next target
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();

			ForceDirection *= MovementForce;

			MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);

			//displays information when enabled
			if (DebugTrackerBotDrawing)
			{
				DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 32, FColor::Yellow, false, 0.0f, 1.0f);
			}
		}

		//displays information when enabled
		if (DebugTrackerBotDrawing)
		{
			DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Yellow, false, 0.0f, 1.0f);
		}
	}
}

//notification when it is overlaping a target
void ATrackerBot::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//checks if it has not started selfdestruct yet or has not exploding 
	if (!bStartedSelfDestruction && !bExploded)
	{
		AMCharacter* PlayerPawn = Cast<AMCharacter>(OtherActor);
		
		//checks if the pawn the bot passed is NOT on the same team
		if (PlayerPawn && !UHealthComponent::IsFriendly(OtherActor, this))
		{
			if (Role == ROLE_Authority)
			{
				//start of self destruction
				GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ATrackerBot::DamageSelf, SelfDamageInterval, true, 0.0f);
			}

			//sets self destruction to true
			bStartedSelfDestruction = true;
		}
	}
}

//gets a new path point if it gets stuck
void ATrackerBot::RefreshPath()
{
	NextPathPoint = GetNextPathPoint();
}