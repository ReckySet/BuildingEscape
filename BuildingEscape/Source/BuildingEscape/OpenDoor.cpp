// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"

#define OUT


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

	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() > 50.f) ///TODO make into a parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all the ovelapping actors
	TArray<AActor*>OverlappingActors;
	TArray<AActor*>null;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	FString ReportStr;

	for (auto* ActorOnThePlate : OverlappingActors) 
	{
		UPrimitiveComponent* ComponentOnThePlate = ActorOnThePlate->FindComponentByClass<UPrimitiveComponent>();
		ReportStr += ActorOnThePlate->GetName();
		ReportStr += TEXT(" ");
		TotalMass += ComponentOnThePlate->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("The thing on plate is : %s"), *ReportStr)
		UE_LOG(LogTemp, Warning, TEXT("The mass on plate is : %f"), &TotalMass)
	}



	//Iterate through them adding their masses

	return TotalMass;
}