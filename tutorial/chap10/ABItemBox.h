// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLE_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;//델리게이트에 바인딩

public:
	UPROPERTY(VisibleAnywhere, Category = Box)//콜리전 컴포넌트 정의
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)//스태틱메시 정의
	UStaticMeshComponent* Box;

	UPROPERTY(VisibleAnywhere, Category = Effect)//이펙트 재생
	UParticleSystemComponent* Effect;

	UPROPERTY(EditInstanceOnly, Category = Box)//특정 클래스와 상속받은 클래스들로 목록 한정
	TSubclassOf<class AABWeapon> WeaponItemClass;

private:
	//overlap 이벤트
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);//이펙트가 끝날때

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
