// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
#include "ABCharacter.h"//기본폰 설정
#include "ABPlayerController.h"

AABGameMode::AABGameMode() {//클래스명에 A라는 접두사가 붙음
	//게임모드에 폰 지정
	DefaultPawnClass = AABCharacter::StaticClass();//기본폰 설정
	PlayerControllerClass = AABPlayerController::StaticClass();
}

void AABGameMode::PostLogin(APlayerController* NewPlayer) {
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));
}
