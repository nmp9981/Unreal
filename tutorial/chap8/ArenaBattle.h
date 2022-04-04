// Fill out your copyright notice in the Description page of Project Settings.
//해당 로그 매크로를 선언하고 모듈의 모든 헤더들이 이 헤더를 참조
#pragma once

#include "EngineMinimal.h"
#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(ArenaBattle, Log, All);

//간편하게 로그 남기기
#define ABLOG_CALLINFO (FString(__FUNCTION__)+TEXT("(")+FString::FromInt(__LINE__)+TEXT(")") )
//로그를 사용한 함수의 실행시점 남기기
#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle,Verbosity,TEXT("%s"),*ABLOG_CALLINFO)
//ABLOG_S 정보에 형식 문자열로 추가 정보를 지정해 로그를 남긴다
#define ABLOG(Verbosity,Format,...) UE_LOG(ArenaBattle,Verbosity,TEXT("%s%s"),*ABLOG_CALLINFO, *FString::Printf(Format,##__VA_ARGS__))
//런타임에서 문제 발생시 붉은색으로 에러로그를 뿌리고 바로 함수 반환
#define ABCHECK(Expr, ...) {if(!(Expr)){ABLOG(Error, TEXT("ASSERTION : %s"),TEXT("'"#Expr"'")); return __VA_ARGS__; }}

