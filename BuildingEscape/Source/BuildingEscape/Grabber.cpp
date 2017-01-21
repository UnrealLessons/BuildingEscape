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
	/// LINE TRACE and see if reach actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	/// If hit valid actor, attach a physics handle
	if (ActorHit)
	{
		if (!physicsHandler) { return; }
		physicsHandler->GrabComponent(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release()
{
	if (!physicsHandler) { return; }
	physicsHandler->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!physicsHandler) { return; }
	if (physicsHandler->GrabbedComponent)
	{
		physicsHandler->SetTargetLocation(GetPlayerReach());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	GetPlayerReach();

	// Ray-casting out to reach distance (private variable to store reach)
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetPlayerStart(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	// Return Actor that was hit
	return HitResult;
}

FVector UGrabber::GetPlayerStart()
{
	/// Get player viewpoint start
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocationVector,
		OUT PlayerRotator
	);
	return PlayerLocationVector;
}

FVector UGrabber::GetPlayerReach()
{
	/// Get player reach
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocationVector,
		OUT PlayerRotator
	);
	return PlayerLocationVector + PlayerRotator.Vector() * Reach;
}
