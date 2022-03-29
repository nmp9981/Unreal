// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPawn.h"

// Sets default values
AABPawn::AABPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//생성자 코드에서 컴포넌트 생성
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	//상속관계
	RootComponent = Capsule;//루트
	Mesh->SetupAttachment(Capsule);//Mesh는 Capsule의 자식
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(Capsule);

	//초기정보 설정
	Capsule->SetCapsuleHalfHeight(88.0f);//캐릭터의 절반 높이
	Capsule->SetCapsuleRadius(34.0f);//캐릭터의 몸둘레
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));//캐릭터위치 보정(애니메이션과 맞추기 위해)
	SpringArm->TargetArmLength = 400.0f;//카메라 지지대 높이
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));//컴포넌트 y축 회전

	//애셋이 자동 로딩되도록
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded()) Mesh->SetSkeletalMesh(SK_CARDBOARD.Object);

}

// Called when the game starts or when spawned
void AABPawn::BeginPlay()
{
	Super::BeginPlay();
	//달리기 애니메이션 애셋 로딩
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/Book/Animations/WarriorRun.WarriorRun"));
	if (AnimAsset != nullptr) Mesh->PlayAnimation(AnimAsset, true);
}

// Called every frame
void AABPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//로그 확인
void AABPawn::PostInitializeComponents() {
	Super::InitializeComponents();
	ABLOG_S(Warning);
}
void AABPawn::PossessedBy(AController* NewController) {
	ABLOG_S(Warning);
	Super::PossessedBy(NewController);
}

// Called to bind functionality to input
void AABPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//입력설정, 입력과 연동할 언리얼 오브젝트 인스턴스의 함수 포인터 지정
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABPawn::UpDown);//위아래
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABPawn::LeftRight);//좌우
}
//위아래 로그 확인, 축입력값은 -1~1
void AABPawn::UpDown(float NewAxisValue) {
	ABLOG(Warning, TEXT("%f"), NewAxisValue);
	AddMovementInput(GetActorForwardVector(), NewAxisValue);//위아래 이동
}
//좌우 로그 확인
void AABPawn::LeftRight(float NewAxisValue) {
	ABLOG(Warning, TEXT("%f"), NewAxisValue);
	AddMovementInput(GetActorRightVector(), NewAxisValue);//좌우 이동
}

