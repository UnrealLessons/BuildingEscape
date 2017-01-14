// Copyright Marc Sevilleja 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	FVector PlayerLocationVector, LineTraceEnd;
	FRotator PlayerRotator;

	FString logVector, logRotator, thisOwner;
	float Reach = 100.f;

	UPhysicsHandleComponent *physicsHandler = nullptr;
	UInputComponent *inputComponent = nullptr;

	// Ray-cast and grab what's in reach
	void Grab();

	// Release grabbed object
	void Release();

	// Find (assumed) attached physics handle
	void findPhysicsHandleComponent();

	// Setup (assumed) attached input component
	void setupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
};
