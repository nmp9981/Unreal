// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Fountain.generated.h"

//클래스 선언 매크로(해당 클래스가 언리얼 오브젝트)
UCLASS()
class ARENABATTLE_API AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent *Body;//분수대 구조물
	UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent *Water;//물
	UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent *Light;//조명
	UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent *Splash;//이펙트

	UPROPERTY(EditAnywhere,Category=ID)
	int32 ID;//초기화

};
