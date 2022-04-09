// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UABCharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;//위젯 내용 업데이트
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UABCharacterStatComponent> CurrentCharacterStat;//약포인터 선언

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
