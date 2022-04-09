// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterWidget.h"
#include "ABCharacterStatComponent.h"

void UABCharacterWidget::BindCharacterStat(UABCharacterStatComponent* NewCharacterStat) {
	ABCHECK(nullptr != NewCharacterStat);
	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChange.AddUObject(this, &UABCharacterWidget::UpdateHPWidget);
}
//위젯내용 업데이트
void UABCharacterWidget::NativeConstruct() {
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	ABCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UABCharacterWidget::UpdateHPWidget() {
	if (CurrentCharacterStat.IsValid()) {
		if (nullptr != HPProgressBar) {
			HPProgressBar -> SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
