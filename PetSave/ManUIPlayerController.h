// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ManUIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PETSAVE_API AManUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = UI)//UI인스턴스 생성
	TSubclassOf<class UUserWidget> UIWidgetClass;//뷰포트에 띄움

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* UIWidgetInstance;//입력은 UI에만 전달
};
