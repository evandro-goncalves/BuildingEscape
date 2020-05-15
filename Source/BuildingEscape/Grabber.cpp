// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Grabber.h"
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
}

void UGrabber::SetupInput()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();

	if(Input)
	{
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("We haven't found an input component!"));
	}
}

void UGrabber::SetupPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle component is not attached to actor, make sure %s have a PhysicsHandle attached to it!"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	//UE_LOG(LogTemp, Warning, TEXT("Grabbed!"));
	GetFirstPhysicsBodyInReach();
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released!"));
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	FHitResult Hit;
	FCollisionQueryParams TraceParameters = { FName(TEXT("")), false, GetOwner() };

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	if(Hit.GetActor() != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor name: %s"), *Hit.GetActor()->GetName());
	}

	return Hit;
}

