// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolRouteComponent.generated.h"

/**
 * Adds patroling functionality to actor (i.e. third person character)
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUND_API UPatrolRouteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolRouteComponent();

	TArray<AActor*> GetPatrolPoints() const;

private:
	UPROPERTY(EditInstanceOnly, Category = "Patrol Route Component")
		TArray<AActor*> PatrolPoints;
		
};
