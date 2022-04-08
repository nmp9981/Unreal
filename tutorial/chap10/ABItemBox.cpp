// Fill out your copyright notice in the Description page of Project Settings.


#include "ABItemBox.h"
#include "ABWeapon.h"
#include "ABCharacter.h"

// Sets default values
AABItemBox::AABItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));//콜리전 컴포넌트
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));//스태틱메쉬
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));//이펙트

	RootComponent = Trigger;//루트 : 콜리전 컴포넌트
	Box->SetupAttachment(RootComponent);//자식 : 스태틱메쉬
	Effect->SetupAttachment(RootComponent);//자식 : 이펙트

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));//전체크기의 절반

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (SM_BOX.Succeeded()) {
		Box->SetStaticMesh(SM_BOX.Object);
	}

	//이펙트 레퍼런스 불러오기
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_CHESTOPEN.Succeeded()) {
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;
	}

	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));//박스 세팅 위치
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));//아이템 박스를 통과하면
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponItemClass = AABWeapon::StaticClass();//속성 클래스값 지정
}

// Called when the game starts or when spawned
void AABItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

//바인딩 함수
void AABItemBox::PostInitializeComponents() {
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
}
//overlap델리게이트
void AABItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ABLOG_S(Warning);//로그출력

	auto ABCharacter = Cast<AABCharacter>(OtherActor);
	ABCHECK(nullptr != ABCharacter);

	if (nullptr != ABCharacter && nullptr != WeaponItemClass) {
		if (ABCharacter->CanSetWeapon()) {
			auto NewWeapon = GetWorld()->SpawnActor<AABWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			ABCharacter->SetWeapon(NewWeapon);
			Effect->Activate(true);
			Box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);
			Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
		}
		else {
			ABLOG(Warning, TEXT("%s can't equip weapon currently."), *ABCharacter->GetName());
		}
	}
}
//이펙트를 마쳤을떄
void AABItemBox::OnEffectFinished(UParticleSystemComponent* PSystem) {
	Destroy();
}
// Called every frame
void AABItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

