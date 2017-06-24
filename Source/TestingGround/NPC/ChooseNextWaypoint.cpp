// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolRouteComponent.h"
#include "AIController.h"



EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto PatrolRouteComponent = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UPatrolRouteComponent>();

	// protect against no patrol route
	if (!PatrolRouteComponent) { return EBTNodeResult::Failed; }

	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);

	// TODO check against empty way points
	auto PatrolPoints = PatrolRouteComponent->GetPatrolPoints();

	// No route, no need to go further
	if (PatrolPoints.Num() <= 0) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("%s is missing patrol points"), *(OwnerComp.GetAIOwner()->GetName()))
		return EBTNodeResult::Failed;
	}

	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;

}