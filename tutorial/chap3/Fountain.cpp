// Fill out your copyright notice in the Description page of Project Settings.


#include "Fountain.h"

// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//static mesh 컴포넌트를 구현
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
	Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));
	Movement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("MOVEMENT"));
	//Body가 루트 컴포넌트, 나머지는는 자식
	RootComponent = Body;
	Water->SetupAttachment(Body);
	Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);

	//각 컴포넌트의 기본위치값 설정
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 135.0f));
	Light->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	Splash->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));

	//생성자 클래스에서 애셋 불러오기(경로값 전달, 게임 실행중 변경X)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));
	//스태틱메시 컴포넌트의 SetStaticMesh()에 전달
	if (SM_BODY.Succeeded()) Body->SetStaticMesh(SM_BODY.Object);

	//물 애셋
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_WATER(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_02.SM_Plains_Castle_Fountain_02'"));
	if(SM_WATER.Succeeded()) Water->SetStaticMesh(SM_WATER.Object);
	//이펙트 애셋
	static ConstructorHelpers::FObjectFinder<UParticleSystem> 
		PS_SPLASH(TEXT("ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01.P_Water_Fountain_Splash_Base_01'"));
	if (PS_SPLASH.Succeeded()) Splash->SetTemplate(PS_SPLASH.Object);

	RotateSpeed = 30.0f;//초당 회전 속도
	Movement->RotationRate = FRotator(0.0f, RotateSpeed, 0.0f);//독립적으로 액터에 부착
}

// Called when the game starts or when spawned(게임 시작하고 가장 먼저 실행하는 엑터의 함수)
void AFountain::BeginPlay()
{
	Super::BeginPlay();
	//ArenaBattle카테고리를 사용한 로그를 남김
	ABLOG_S(Warning);
	ABLOG(Warning,TEXT("Actor Name : %s, ID : %d, Location X : %.3f"), *GetName(),ID,GetActorLocation().X);
	
}

//Endplay()에 로그 남기기
void AFountain::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	ABLOG_S(Warning);
}

//PostInitializeComponents()에 로그 남기기
void AFountain::PostInitializeComponents() {
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

// Called every frame, 틱마다 동작
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0.0f, RotateSpeed * DeltaTime, 0.0f));//z축을 기준으로 회전, Deltatime은 프레임 타임 정보
}

