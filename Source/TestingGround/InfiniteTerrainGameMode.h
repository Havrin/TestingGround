// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestingGroundGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API AInfiniteTerrainGameMode : public ATestingGroundGameMode
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
	void PopulateBoundsVolumePool();
	
private:
	void AddToPool(class ANavMeshBoundsVolume* VolumeToAdd);
	
};
