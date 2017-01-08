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

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
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

	logVector = PlayerLocationVector.ToString();
	logRotator = PlayerRotator.ToString();

	// Log out to Test
	/*UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s."),
		*logVector,
		*logRotator
	);*/

	LineTraceEnd = PlayerLocationVector + PlayerRotator.Vector() * Reach;

	// Draw a red trace in the world
	DrawDebugLine(
		GetWorld(),
		PlayerLocationVector,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
		);

	// Ray-casting out to reach distance (private variable to store reach)

	// See what we hit
}

