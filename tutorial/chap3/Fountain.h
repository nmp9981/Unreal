// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"//앞으로는 ArenaBattle헤더를 참조
#include "GameFramework/RotatingMovementComponent.h"//동작 컴포넌트
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;//액터 퇴장
	virtual void PostInitializeComponents() override;//세팅완료

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent *Body;//분수대 구조물
	UPROPERTY(VisibleAnyWhere)
	UStaticMeshComponent *Water;//물
	UPROPERTY(VisibleAnyWhere)
	UPointLightComponent *Light;//조명
	UPROPERTY(VisibleAnyWhere)
	UParticleSystemComponent *Splash;//이펙트

	UPROPERTY(EditAnywhere,Category=ID)
	int32 ID;//초기화

	UPROPERTY(VisibleAnyWhere)
	URotatingMovementComponent* Movement;

private:
	//데이터 은닉을 위한 캠슐화
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float RotateSpeed;//각속도->분수대를 z축기준으로 회전

};
