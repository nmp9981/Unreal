// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()

public:
	AABAIController();
	virtual void OnPossess(APawn* aPawn) override;

	static const FName HomePosKey;//npc생성 위치
	static const FName PatrolPosKey;//순찰할 위치벙보 보관
	static const FName TargetKey;//타겟

private:
	UPROPERTY()
	class UBebaviorTree* BTAsset;
	
	UPROPERTY()
	class UBlackboardData* BBAsset;
};
