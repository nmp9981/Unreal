// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"

void AABPlayerController::PostInitializeComponents() {
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AABPlayerController::OnPossess(APawn* aPawn) {
	ABLOG_S(Warning);
	Super::OnPossess(aPawn);
}

//게임에게만 입력 전달
void AABPlayerController::BeginPlay() {
	Super::BeginPlay();

	FInputModeGameOnly InputMode;//게임에게만
	SetInputMode(InputMode);//입력전달
}
