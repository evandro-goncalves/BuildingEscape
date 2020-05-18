// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// Get door's Yaw positions and set open degree
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;;
	OpenAngle += InitialYaw;

	FindPressurePlate();
	FindAudioComponent();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Test collision and open or close the door
	if(GetTotalMass() > OpenRequiredMass)
	{
		OpenDoor(DeltaTime);
		
		LastOpenedTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if(GetWorld()->GetTimeSeconds() >= LastOpenedTime + CloseDelay) CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, OpenSpeed);
	
	GetOwner()->SetActorRotation(FRotator(0.f, CurrentYaw, 0.f));

	if(!AudioComponent) return;

	if(!bIsDoorOpen)
	{
		AudioComponent->Play();

		bIsDoorOpen = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, CloseSpeed);
		
	GetOwner()->SetActorRotation(FRotator(0.f, CurrentYaw, 0.f));

	if(!AudioComponent) return;

	if(bIsDoorOpen && CurrentYaw <= InitialYaw + SoundPlayTresholdAngle)
	{		
		AudioComponent->Play();

		bIsDoorOpen = false;
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if(!AudioComponent) UE_LOG(LogTemp, Error, TEXT("No audio component on %s"), *GetOwner()->GetName());
}

void UOpenDoor::FindPressurePlate()
{
	if(!PressurePlate) UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressure plate is set!"), *GetOwner()->GetName());
}

float UOpenDoor::GetTotalMass() const
{
	int TotalMass = 0.f;

	TArray<AActor*> OverlapingActors;
	if(!PressurePlate) return TotalMass;

	PressurePlate->GetOverlappingActors(OverlapingActors);

	for(AActor* OverlapingActor : OverlapingActors)
	{
		TotalMass += OverlapingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}