// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AABAIController::HomePosKey(TEXT("HomePos"));
const FName AABAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AABAIController::TargetKey(TEXT("Target"));

//쿨타임
AABAIController::AABAIController() {
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Book/AI/BB_ABCharacter.BB_ABCharacter"));
	if (BBObject.Succeeded()) {
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Book/AI/BT_ABCharacter.BT_ABCharacter"));
	if (BTObject.Succeeded()) {
		BTAsset = BTObject.Object;
	}
}
void AABAIController::OnPossess(APawn* InPawn) {
	Super::Possess(InPawn);
	if (UseBlackboard(BBAsset, Blackboard)) {
		//npc가 임의 위치로 순찰
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());//순찰 위치
		if (!RunBehaviorTree(BTAsset)) {
			ABLOG(Error, TEXT("AIController could't run behavior tree!"));
		}
	}
}
