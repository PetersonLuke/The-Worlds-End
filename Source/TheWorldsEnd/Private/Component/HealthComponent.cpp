/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

HealthComponent:  This give health to the player and AI.  When it reaches 0 the player dies

Gives the pawn a set amount of health (Can be changed in blueprints)
Checks if the player is dead
checks if the health has changed
lets the player get healed
sets the team for players and AI
handles the taking of any type of damage

*/

#include "HealthComponent.h"
#include "TheWorldsEndGameMode.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	//normal health amount
	DefaultHealth = 100;
	//is it dead?  not yet!
	bIsDead = false;

	//defult team number
	TeamNum = 255;

	//multiplayer stuff
	SetIsReplicated(true);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	//only works if we are the server
	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
		}
	}
	
	Health = DefaultHealth;
}


//used to have the client get its health value
void UHealthComponent::OnRep_Health(float OldHealth)
{
	//gets the health subtracted by oldhealth for the damage
	float Damage = Health - OldHealth;

	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}


//handling of damage that is taken
void UHealthComponent::HandleTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	//CHecks if the person dealing damage is not themselves
	//then checks if on the same team
	if (DamageCauser != DamagedActor && IsFriendly(DamagedActor, DamageCauser))
	{
		//does no damage if on the same team
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	//debug output log
	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));

	//is dead if health is less then or equle to 0
	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if(bIsDead)
	{ 
		ATheWorldsEndGameMode* GM = Cast<ATheWorldsEndGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}
}


//healing
void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || Health <= 0.0f)
	{
		return;
	}

	//how much to heal
	Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s (+%s)"), *FString::SanitizeFloat(Health), *FString::SanitizeFloat(HealAmount));

	//changes the health
	OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);
}


//Check for if on same team
bool UHealthComponent::IsFriendly(AActor * ActorA, AActor * ActorB)
{
	if (ActorA == nullptr || ActorB == nullptr)
	{
		//friendly actor
		return true;
	}

	//gets healthcomponent for actor A and B
	UHealthComponent* HealthCompA = Cast<UHealthComponent>(ActorA->GetComponentByClass(UHealthComponent::StaticClass()));
	UHealthComponent* HealthCompB = Cast<UHealthComponent>(ActorB->GetComponentByClass(UHealthComponent::StaticClass()));

	//checks if on same team
	if (HealthCompA == nullptr || HealthCompB == nullptr)
	{
		//friendly actor
		return true;
	}

	//if equal they are friendly
	return HealthCompA->TeamNum == HealthCompB->TeamNum;
}

//gets the health.  used to do stuff in blueprints
float UHealthComponent::GetHealth() const
{
	return Health;
}

//needed to allow muiltiplayer
void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, Health);
}