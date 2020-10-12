// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

#include "Engine/Scene.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PickupSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Pickup Scene Componenet"));
	RootComponent = PickupSceneComponent;

	PickupRoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Picup Rounding Box"));
	PickupRoundingBox->AttachTo(RootComponent);
	PickupRoundingBox->SetGenerateOverlapEvents(true);

	PickupRoundingBox->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnEnterPickup);
	PickupRoundingBox->SetWorldScale3D(FVector(1.0f,2.0f,1.0f));

}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::OnEnterPickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnPickup(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("Overlap Event"));
}

void APickup::OnPickup(AActor* ActorThatPickedUp)
{
}

void APickup::OnGenerate()
{
}

