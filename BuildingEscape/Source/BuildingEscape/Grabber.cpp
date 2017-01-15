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
	if (physicsHandler == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handler component missing from %s!!!"), *thisOwner)
	}
}

/// Look for attached Input Component (only appears at runtime)
void UGrabber::setupInputComponent()
{
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component missing from %s!!!"), *thisOwner)
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbing!"))

	/// LINE TRACE and see if reach actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If hit valid actor, attach a physics handle
	if (ActorHit)
	{
		physicsHandler->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release!"))
	physicsHandler->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	// Get player viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocationVector,
		OUT PlayerRotator
	);
	LineTraceEnd = PlayerLocationVector + PlayerRotator.Vector() * Reach;

	// If physics handle is attached
	if (physicsHandler->GrabbedComponent)
	{
		// move object this is holding
		physicsHandler->SetTargetLocation(LineTraceEnd);
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocationVector,
		OUT PlayerRotator
	);
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

	return Hit;
}
