// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance() {
	CurrentPawnSpeed = 0.0f;//초기값
	IsInAir = false;//초기상태는 점프상태가 아니다.
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();//폰에 접근해 폰의 정보 얻어옴, 폰 객체가 유효한가?
	if (::IsValid(Pawn)) {
		CurrentPawnSpeed = Pawn->GetVelocity().Size();//폰의 속력을 구함
		auto Character = Cast<ACharacter>(Pawn);
		if (Character) IsInAir = Character->GetMovementComponent()->IsFalling();//캐릭터가 공중어 떠있는지에 대한 정보를 가져온다.
	}
}
