// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void FindAudioComponent();
	void FindPressurePlate();
	float GetTotalMass() const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float InitialYaw;
	float CurrentYaw;
	float LastOpenedTime;
	bool bIsDoorOpen = false;
	
	UAudioComponent* AudioComponent = nullptr;

	UPROPERTY(EditAnywhere)
	float OpenSpeed = 1.f;
	
	UPROPERTY(EditAnywhere)
	float CloseSpeed = 7.f;

	UPROPERTY(EditAnywhere)
	float CloseDelay = 0.5f;
	
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	float SoundPlayTresholdAngle = 5.f;

	UPROPERTY(EditAnywhere)
	float OpenRequiredMass = 50.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
};
