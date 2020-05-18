// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

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

	SetupInput();
	SetupPhysicsHandle();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!PhysicsHandle) return;
	if(PhysicsHandle->GetGrabbedComponent()) PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
}

void UGrabber::SetupInput()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();

	if(Input)
	{
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::SetupPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(!PhysicsHandle) UE_LOG(LogTemp, Error, TEXT("PhysicsHandle component is not attached to actor, make sure %s have a PhysicsHandle attached to it!"), *GetOwner()->GetName());
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* HitActor = HitResult.GetActor();

	if(HitActor)
	{
		if(!PhysicsHandle) return;
		
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetLineTraceEnd());
	}
}

void UGrabber::Release()
{
	if(!PhysicsHandle) return;

	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetLineTraceEnd() const
{
	return GetPlayerWorldPosition().GetLocation() + GetPlayerWorldPosition().GetRotation().Rotator().Vector() * Reach;
}

FTransform UGrabber::GetPlayerWorldPosition() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	FTransform PlayerWorldPosition = { PlayerViewPointRotation, PlayerViewPointLocation, FVector(1, 1, 1) };

	return PlayerWorldPosition;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionObjectQueryParams ObjectParameters = { ECollisionChannel::ECC_PhysicsBody };
	FCollisionQueryParams TraceParameters = { FName(TEXT("")), false, GetOwner() };

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		GetPlayerWorldPosition().GetLocation(),
		GetLineTraceEnd(),
		ObjectParameters,
		TraceParameters
	);

	if(Hit.GetActor() != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor name: %s"), *Hit.GetActor()->GetName());
	}

	return Hit;
}

