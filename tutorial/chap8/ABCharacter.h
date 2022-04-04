// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

UCLASS()
class ARENABATTLE_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//디아블로 방식
	enum class EControlMode {
		GTA,
		DIABLO
	};
	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	//초기값 설정
	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;//델리게이트 바인딩

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//액터 불러오기
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

private:
	//방향키 이동
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	//마우스 이동
	void LookUp(float NewAxisValue);
	void Trun(float NewAxisValue);

	void ViewChange();//시점 변환
	void Attack();//공격

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);//몽타주

	void AttackStartComboState();//공격속성지정
	void AttackEndComboState();//공격종료

private:
	UPROPERTY(VisibleInatanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;//공격 중인가?

	UPROPERTY(VisibleInatanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;//다음 콤보로의 이동 가능 여부

	UPROPERTY(VisibleInatanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;//콤보 입력 여부

	UPROPERTY(VisibleInatanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;//현재 실행중인 콤보 카운터

	UPROPERTY(VisibleInatanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;//콤보 최대치

	//애님 인스턴스 전방선언
	UPROPERTY()
	class UABAnimInstance* ABAnim;
};
