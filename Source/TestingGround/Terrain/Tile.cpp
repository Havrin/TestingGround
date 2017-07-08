// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "AI/Navigation/NavigationSystem.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 0);
	NavMeshOffset = FVector(2000, 0, 0);

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float MinScale, float MaxScale, float Radius)
{
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int32 i = 0; i < NumberToSpawn; i++)
	{
		FVector RandomPoint;
		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		if (FindEmptyLocation(RandomPoint, Radius * RandomScale))
		{
			float Rotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, RandomPoint, Rotation, RandomScale);
		}
		
	}
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector RandomPoint, float Rotation, float RandomScale)
{
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn);
	SpawnedActor->SetActorRelativeLocation(RandomPoint);
	SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	SpawnedActor->SetActorRotation(FRotator(0, Rotation, 0));
	SpawnedActor->SetActorScale3D(FVector(RandomScale));
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ActorPool->Return(NavMeshBoundsVolume);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::SetActorPool(UActorPool* ActorPoolToSet)
{
	ActorPool = ActorPoolToSet;
	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = ActorPool->Checkout();
	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Not enough actors in pool."), *GetName());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked out: {%s}"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavMeshOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

bool ATile::IsAbleToSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
	FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	return !HasHit;
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius) 
{
	FBox Bounds(MinExtent, MaxExtent);
	const int MAX_ATTEMPTS = 30;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector RandomPoint = FMath::RandPointInBox(Bounds);
		if (IsAbleToSpawnAtLocation(RandomPoint, Radius))
		{
			OutLocation = RandomPoint;
			return true;
		}
	}
	
	return false;
}

