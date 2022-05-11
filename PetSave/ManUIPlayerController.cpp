// Fill out your copyright notice in the Description page of Project Settings.


#include "ManUIPlayerController.h"
#include "Blueprint/UserWidget.h"

#define ABCHECK(Expr, ...)

void AManUIPlayerController::BeginPlay() {

	Super::BeginPlay();

	ABCHECK(nullptr != UIWidgetClass);

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	ABCHECK(nullptr != UIWidgetInstance);

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}
