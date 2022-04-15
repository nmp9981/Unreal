// Fill out your copyright notice in the Description page of Project Settings.
//새로운 캐릭터

#include "ABCharacter.h"
#include "ABAnimInstance.h" 
#include "DrawDebugHelpers.h"
#include "ABWeapon.h"
#include "ABCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "ABCharacterWidget.h"
#include "Components/ProgressBar.h"
#include "ABAIController.h"

// Sets default values
AABCharacter::AABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));//체력바

	//상속
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	//초기위치
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	//애셋지정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded()) GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	//애니메이션 지정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint"));
	if (WARRIOR_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);

	//무기지정
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket)) {
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
		if (SK_WEAPON.Succeeded()) {
			Weapon->SetSkeletalMesh(SK_WEAPON.Object);
		}
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}
	SetControlMode(EControlMode::DIABLO);

	//속도지정
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;
	GetCharacterMovement()->JumpZVelocity = 800.0f;//캐릭터 점프 높이 조절
	
	IsAttacking = false;//초기상태는 공격중이 아니다

	MaxCombo = 4;//최대 콤보
	AttackEndComboState();//콤보 속성

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter"));//캡슐 컴포넌트 설정

	AttackRange = 200.0f;//공격반경
	AttackRadius = 50.0f;//캡슐반지름

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);//항상 플레이어를 향하게
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar"));
	if (UI_HUD.Succeeded()) {
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));//UI크기
	}

	AIControllerClass = AABAIController::StaticClass();//npc생성자
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//캐릭터에 무기 부착
	FName WeaponSocket(TEXT("hand_rSocket"));
	//월드에서 새 액터 생성, getworld()로 월드의 포인터를 가져옴
	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon) {
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
}
//무기세팅
bool AABCharacter::CanSetWeapon() {
	return (nullptr == CurrentWeapon);
}

//무기 장착
void AABCharacter::SetWeapon(AABWeapon* NewWeapon) {
	ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon) {
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

/*
//GTA방식 3인칭 조작
void AABCharacter::SetControlMode(int32 ControlMode) {
	if (ControlMode == 0) {//0번 모드
		//SpringArm->TargetArmLength = 450.0f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		//회전여부
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		//캐릭터가 움직이는 방향으로 자동회전
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	}
}
*/
//디아블로 방식 3인칭 구현
void AABCharacter::SetControlMode(EControlMode NewControlMode) {
	CurrentControlMode = NewControlMode;//컨트롤 모드 값

	switch (CurrentControlMode) {
	case EControlMode::GTA:
		//SpringArm->TargetArmLength = 450.0f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengthTo = 450.0f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;//캐릭터의 회전 보완
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::DIABLO:
		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;//캐릭터의 회전 보완
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
}

//OnMontageEnded 바인딩 함수, 애님 인스턴스를 멤버변수로 선언
void AABCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != ABAnim);

	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);
	ABAnim->OnNextAttackCheck.AddLambda([this]()->void {
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;//콤보상태가 아님
		if (IsComboInputOn) {
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);//공격 범위 체크

	//죽었을때
	CharacterStat->OnHPIsZero.AddLambda([this]()->void {
		ABLOG(Warning, TEXT("OnHPIsZero"));
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	});

	//UI와 캐릭터 컴포넌트 연결
	auto CharacterWidget = Cast<UABCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget) {
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);//목표까지 서서히 변경
	//목표지점까지 서서히 변경
	switch (CurrentControlMode) {
	case EControlMode::DIABLO:
		SpringArm -> RelativeRotation = FMath::RInterpTo(SpringArm -> RelativeRotation, ArmRotationTo, DeltaTime, ArmRotationSpeed);
		break;
	}

	switch (CurrentControlMode) {
	case EControlMode::DIABLO:
		if (DirectionToMove.SizeSquared() > 0.0f) {
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);//캐릭터 움직임 설정
		}
		break;
	}

}

void AABCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	if (IsPlayerControlled()) {
		SetControlMode(EControlMode::DIABLO);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;//캐릭터 최대속력
	}
	else {
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;//NPC 최대속력
	}
}
// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//액션매핑 입력설정과 연동
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);
	//ACharacter클래스에는 Jump라는 멤버함수가 있다.
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	//공격 액션 매핑
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);
	//방향키 이동
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	//마우스 좌우이동
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Trun"), this, &AABCharacter::Trun);
}
/*
//캐릭터 움직임 설정, 회전값으로부터 시선방향, 우측값의 벡터 가져오기
void AABCharacter::UpDown(float NewAxisValue) {
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);//x축 : 시선방향
}
void AABCharacter::LeftRight(float NewAxisValue) {
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);//y축 : 우측방향
}
*/
//위아래 x축
void AABCharacter::UpDown(float NewAxisValue) {
	switch (CurrentControlMode) {
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.0f,GetControlRotation().Yaw,0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);//x축 : 시선방향
		break;
	case EControlMode::DIABLO:
		DirectionToMove.X = NewAxisValue;
		break;
	}
}
//좌우 y축
void AABCharacter::LeftRight(float NewAxisValue) {
	switch (CurrentControlMode) {
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);//y축 : 시선방향
		break;
	case EControlMode::DIABLO:
		DirectionToMove.Y = NewAxisValue;
		break;
	}
}

//마우스 좌우이동
/*
void AABCharacter::LookUp(float NewAxisValue) {
	AddControllerPitchInput(NewAxisValue);
}
*/
void AABCharacter::LookUp(float NewAxisValue) {
	switch (CurrentControlMode) {
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);
		break;
	}
}
/*
void AABCharacter::Trun(float NewAxisValue) {
	AddControllerYawInput(NewAxisValue);
}
*/
void AABCharacter::Trun(float NewAxisValue) {
	switch (CurrentControlMode) {
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);
		break;
	}
}

//시점변환 액션매핑, 캐릭터 컨트롤 변경
void AABCharacter::ViewChange() {
	switch (CurrentControlMode) {
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());//캐릭터 방향
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->RelativeRotation);//SpringArm회전
		SetControlMode(EControlMode::GTA);
		break;
	}
}

//공격 애니 재생
void AABCharacter::Attack() {
	//공격중
	if (IsAttacking) {
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo) {
			IsComboInputOn = true;
		}
	}
	else {//공격중이 아님
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;//공격중으로 변환
	}
}

void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	ABCHECK(IsAttacking);//공격 매크로
	ABCHECK(CurrentCombo > 0);//현재 콤보가 진행중인가?
	IsAttacking = false;//공격중이 아님을 표시하여 공격 매크로 종료
	AttackEndComboState();//콤보 종료
	OnAttackEnd.Broadcast();//공격이 끝났는가
}

//콤보 속성 지정
void AABCharacter::AttackStartComboState() {
	CanNextCombo = true;//다음 콤보로 넘어갈 수 있다.
	IsComboInputOn = false;//이미 입력했으니 false로
	ABCHECK(FMath::IsWithinInclusive<int32>)(CurrentCombo, 0, MaxCombo - 1);//현재콤보
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);//다음콤보로
}
//콤보공격 종료, 콤보 초기화
void AABCharacter::AttackEndComboState() {
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

//데미지 전달
float AABCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EvenInstigator, AActor* DamageCauser) {
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EvenInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	
	CharacterStat->SetDamage(FinalDamage);
	return FinalDamage;
}

//공격 범위 체크
void AABCharacter::AttackCheck() {
	FHitResult HitResult;//구조체 설정
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,//구조체
		GetActorLocation(),//시작위치
		GetActorLocation() + GetActorForwardVector() * AttackRange,//끝위치
		FQuat::Identity,//도형 회전
		ECollisionChannel::ECC_GameTraceChannel2,//콜리전 채널
		FCollisionShape::MakeSphere(AttackRadius),//탐지에 사용할 도형 제작
		Params//탐색방법
	);
#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;//공격판정결과
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);
#endif
	if (bResult) {
		if (HitResult.Actor.IsValid()) {
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());//로그 나타내기

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);//데미지 전달
		}
	}
}
