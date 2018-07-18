/*

Author:		Luke Peterson
Project:	The worlds End
Date:		2018

MCharacter:  This is what creates the pawn that the player will controlle
It handles Crafting inforamtion getting components ids, the product ID and if the components should be destroyed
inventory information is also used.  It gets the items ID name action used, thumbnail used in inventory
descrption for the item, crafting combinations that can be done,and if it can be used

Movments:	Forwards and backwards movments are created
Left and right movments
crouching

Sets the energy, health, hunger components to the Blueprint playerPawn
Gets the current weapon being used by the playerPawn
Sets what is the starting weapon for the playerPawn
"attaches" the weapon to the socket on the playerPawn
deals with health changes
Checks if dead
Checks if the player is looking at interactables
Gets player input
Gets the playPawns view location
Checks when the player starts swinging
Checks when the player stops swinging

*/


#include "MCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameplayController.h"
#include "Interactable.h"
#include "Components/CapsuleComponent.h"
#include "TheWorldsEnd.h"
#include "HealthComponent.h"
#include "EnergyComponent.h"
#include "HungerComponent.h"
#include "WeaponSwords.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AMCharacter::AMCharacter()
{
 	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	//Creates the camera for the playerPawn
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	//uses the pawns rotations to also move the camera
	CameraComp->bUsePawnControlRotation = true;
	//Makes it attached to the root component in PlayerPawn
	CameraComp->SetupAttachment(RootComponent);

	//makes it so the hit box is not the capsule
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	//gets the HealthComponent class
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HelthComp"));

	//gets the UEnergyComponent class
	EnergyComp = CreateDefaultSubobject<UEnergyComponent>(TEXT("EnergyComp"));

	//gets the UHungerComponent class
	HungerComp = CreateDefaultSubobject<UHungerComponent>(TEXT("HungerComp"));

	//determans the name for the socked the weapon should be attached to
	WeaponAttachSocketName = "WeaponSocket";

}


// Called when the game starts or when spawned
void AMCharacter::BeginPlay()
{
	Super::BeginPlay();

	//gets information about health changing
	HealthComp->OnHealthChanged.AddDynamic(this, &AMCharacter::OnHealthChanged);
	
	//sets being able to crouch as true
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	//allows multiplayer stuff
	if (Role == ROLE_Authority)
	{
		//Spawn a default weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//gets the currentweapon
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponSwords>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon)
		{
			//attaches the weapon to the socket
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
	}
}


//forwards and backwards movment
void AMCharacter::ForwardBackward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}


//left and right movement
void AMCharacter::LeftRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}


//starts crouch on button hold
void AMCharacter::BeginCrouch()
{
	Crouch();	//funciton implemented with the engine
}


//ends crouch on button lift
void AMCharacter::EndCrouch()
{
	UnCrouch();	//function implemented with the engine
}


//starts swing on button hold
void AMCharacter::StartSwing()
{
	//checks if there is a weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->StartSwing();
	}
}


//stops swing on button lift
void AMCharacter::StopSwing()
{
	//checks if there is a weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->StopSwing();
	}
}


//check for when health has changed
void AMCharacter::OnHealthChanged(UHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	//checks if the players health is less then 0 and not dead
	if (Health <= 0.0f && !bDied)
	{
		//death!
		bDied = true;
		//stops movment
		GetMovementComponent()->StopMovementImmediately();
		//stops collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();
		//body stays for 10 seconds
		SetLifeSpan(10.0f);
	}
}

//start of looking for objects the player can interact with 
void AMCharacter::CheckForInteractables()
{
	// Create a LineTrace to check for a hit
	FHitResult HitResult;

	//how far the player can look
	int32 Range = 500;
	//start of the trace
	FVector StartTrace = CameraComp->GetComponentLocation();
	//end of the trace
	FVector EndTrace = (CameraComp->GetForwardVector() * Range) + StartTrace;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	AGameplayController* IController = Cast<AGameplayController>(GetController());

	if (IController)
	{
		// Check if something is hit
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, QueryParams))
		{
			// Cast the actor to AInteractable
			AInteractable* Interactable = Cast<AInteractable>(HitResult.GetActor());
			// If the cast is successful
			if (Interactable)
			{
				IController->CurrentInteractable = Interactable;
				return;
			}
		}

		IController->CurrentInteractable = nullptr;
	}
}


// Called every frame
void AMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForInteractables();
}


// Called to bind functionality to input
void AMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Keyboard movements
	//Moves the Character backwards and forwards
	PlayerInputComponent->BindAxis("ForwardBackward", this, &AMCharacter::ForwardBackward);
	//Moves the Character Left and right
	PlayerInputComponent->BindAxis("LeftRight", this, &AMCharacter::LeftRight);				

	//Mouse movements
	//Lets the Character look Up and down
	PlayerInputComponent->BindAxis("VerticalView", this, &AMCharacter::AddControllerPitchInput);

	//lets the character look left and right
	PlayerInputComponent->BindAxis("HorizontalView", this, &AMCharacter::AddControllerYawInput);	

	//Player up and down movment
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMCharacter::BeginCrouch);	//lets the character crouch and button hold
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMCharacter::EndCrouch);		//uncrouchs the character on button release
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);	//lets the character jump
	PlayerInputComponent->BindAction("Swing", IE_Pressed, this, &AMCharacter::StartSwing);
	PlayerInputComponent->BindAction("Swing", IE_Released, this, &AMCharacter::StopSwing);
}


FVector AMCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

//needed to allow muiltiplayer magical stuff!
void AMCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMCharacter, CurrentWeapon);
	DOREPLIFETIME(AMCharacter, bDied);
}