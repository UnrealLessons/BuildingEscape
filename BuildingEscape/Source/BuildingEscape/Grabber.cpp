// Copyright Marc Sevilleja 2016.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	thisOwner = GetOwner()->GetName();
	findPhysicsHandleComponent();
	setupInputComponent();

}

/// Look for attached Physics Handle
void UGrabber::findPhysicsHandleComponent()
{
	/// Look for attached Physics Handle
	physicsHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandler)
	{
		// Physics handle is found
	}
	else
	{
		// Physics handle is missing
		UE_LOG(LogTemp, Error, TEXT("Physics Handler component missing from %s!!!"), *thisOwner)
	}
}

/// Look for attached Input Component (only appears at runtime)
void UGrabber::setupInputComponent()
{
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		// Input component is found
		UE_LOG(LogTemp, Warning, TEXT("Input component found!"))
			///Bind the input axis
			inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		// Input component is missing
		UE_LOG(LogTemp, Error, TEXT("Input component missing from %s!!!"), *thisOwner)
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbing!"))

		/// LINE TRACE and see if reach actors with physics body collision channel set
		GetFirstPhysicsBodyInReach();

	/// If hit valid actor, attach a physics handle
		// TODO Attach physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release!"))
	// TODO Release physics handle
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	// If physics handle is attached
		// move object this is holding
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get player viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocationVector,
		OUT PlayerRotator
	);

	logVector = PlayerLocationVector.ToString();
	logRotator = PlayerRotator.ToString();

	LineTraceEnd = PlayerLocationVector + PlayerRotator.Vector() * Reach;

	// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// Ray-casting out to reach distance (private variable to store reach)
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerLocationVector,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// See what we hit
	AActor *ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
	}

	return FHitResult();
}
