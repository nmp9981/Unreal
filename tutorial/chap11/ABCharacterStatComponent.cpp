// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacterStatComponent.h"
#include "ABGameInstance.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;//컴포넌트 초기화 설정

	Level = 1;//초기 레벨
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

//컴포넌트 초기화
void UABCharacterStatComponent::InitializeComponent() {
	Super::InitializeComponent();
	SetNewLevel(Level);//레벨 변경
}

//레벨 변경
void UABCharacterStatComponent::SetNewLevel(int32 NewLevel) {
	auto ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ABCHECK(nullptr != ABGameInstance);
	CurrentStatData = ABGameInstance->GetABChracterData(NewLevel);
	if (nullptr != CurrentStatData) {
		Level = NewLevel;
		SetHP(CurrentStatdata->MaxHP);
	}
	else {
		ABLOG(Error, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}
}

//데미지설정
void UABCharacterStatComponent::SetDamage(float NewDamage) {//피해량
	ABCHECK(nullptr != CurrentStatData);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

//hp세팅
void UABCharacterStatComponent::SetHP(float NewHP) {
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER) {
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}
//공격
float UABCharacterStatComponent::GetAttack() {
	ABCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}
//hp바 비율
float UABCharacterStatComponent::GetHPRatio() {
	ABCHECK(nullptr != CurrentStatData, 0.0f);

	return (CurrentStatdata->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatdata->MaxHP);
}
