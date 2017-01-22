// Copyright Marc Sevilleja 2016.

#include "BuildingEscape.h"
#include "OpenDoor.h"
#include "Grabber.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	if (Owner == nullptr) { return; }
	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	// Set the door rotation
	if (Owner == nullptr) { return; }
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	if (Owner == nullptr) { return; }
	Owner->SetActorRotation(FRotator(0.f, CloseAngle, 0.f));
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	/// Check if time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (PressurePlate == nullptr) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OverlappingActors);

	// Iterate through them adding their masses
	for (const auto& Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is on the pressure plate!"), *(Actor->GetName()))
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

