// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance() {
	CurrentPawnSpeed = 0.0f;//초기값
	IsInAir = false;//초기상태는 점프상태가 아니다.
	//몽타주 애니메이션 재생
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded()) AttackMontage = ATTACK_MONTAGE.Object;
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

//애니메이션 몽타주 재생
void UABAnimInstance::PlayAttackMontage() {
	Montage_Play(AttackMontage, 1.0f);//델리게이트에 의해 공격 시작과 끝 감지
}

//애니메이션 노티피아 로그
void UABAnimInstance::AnimNotify_AttackHitCheck() {
	ABLOG_S(Warning);
}

void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection) {
	ABCHECK(Montage_IsPlaying(AttackMontage));//문제발생시 에러로그 뿌림
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection),AttackMontage);
}

//델리게이트 명령
void UABAnimInstance::AnimNotify_AttackHitCheck() {
	OnAttackHitCheck.Broadcast();
}
void UABAnimInstance::AnimNotify_NextAttackCheck() {
	OnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 Section) {
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
