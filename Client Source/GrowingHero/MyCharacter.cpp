// Fill out your copyright notice in the Description page of Project Settings.
#include "MyCharacter.h"
#include "MyCharacterController.h"
#include "CombatComponent.h"
#include "EnemyCharacter.h"
#include "StatComponent.h"

#include "Components/SphereComponent.h"
#include "Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Protocol.pb.h"

AMyCharacter::AMyCharacter() 
	: OverlappingEnemies{}, BaseTurnRate{}, BaseLookUpRate{}, m_bIsAdventureMode{}, m_nOpenedUICount{}, m_DashIdx{}, m_DesiredDashYaw{}
{
	
	// 틱 함수가 동작하도록 설정
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// 캐릭터 클래스가 기본적으로 가지고 있는 캡슐 콜라이더의 크기를 초기화
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// 캐릭터가 카메라의 회전을 따라서 회전하지 않도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터의 무브먼트를 규정
	// 캐릭터를 이동시키기 전에 이동 방향과 현재 캐릭터의 방향이 다르면 캐릭터를 이동 방향으로 초당 640도의 회전 속도로 회전시킨다음 이동
	// 캐릭터의 이동을 평면으로 제한
	// 시작할 때 캐릭터의 위치가 평면을 벗어난 상태라면 가까운 평면으로 붙여서 시작되도록 설정	
	// 평면이란 내비게이션 메시를 의미
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 카메라를 캐릭터에게서 적절한 위지를 잡도록 도와주는 스프링 암 컴포넌트를 생성하고 설정
	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArmComponent"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraSpringArmComponent->TargetArmLength = 800.0f;
	CameraSpringArmComponent->bUsePawnControlRotation = true;
	// 캐릭터가 회전할 때 카메라는 회전하지 않았으면 좋겠음.
	CameraSpringArmComponent->SetUsingAbsoluteRotation(true);
	// 위의 코드로는 해결이 안돼서 다음 추가
	CameraSpringArmComponent->bInheritPitch = false;
	CameraSpringArmComponent->bInheritRoll = false;
	CameraSpringArmComponent->bInheritYaw = false;
	// 카메라와 캐릭터의 거리를 800으로 설정
	// 스프링 암을 회전시켜 위에서 캐릭터를 내려다보도록 설정
	// CameraSpringArmComponent->SetRelativeRotation(FRotator(-60.f, 45.0f, 0.0f));
	// 카메라가 벽에 닿으면 충돌 계산을 통해 카메라와 캐릭터의 거리를 좁혀 카메라가 벽을 뚫지 않게 만들어주는 프로퍼티.
	CameraSpringArmComponent->bDoCollisionTest = false;


	// 카메라 컴포넌트를 생성하고 스프링 암 컴포넌트에 붙이는 작업
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraSpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;


	// 범위 안에 들어오면 전투가능 상태
	CombatRange = CreateDefaultSubobject<USphereComponent>(TEXT("CombatRange"));
	CombatRange->SetupAttachment(GetRootComponent());
	CombatRange->InitSphereRadius(m_FStat.AttackRange);

	DashNiagara = CreateDefaultSubobject<UNiagaraComponent >(TEXT("DashNiagara"));
	DashNiagara->SetupAttachment(RootComponent);
	DashNiagara->bAutoActivate = false;

	BaseTurnRate = 65.f;
	BaseLookUpRate = 10.f;

	initStat(100.f);
	m_eCharType = ECharType::E_Hero;

	m_nOpenedUICount = 0;
	m_DashIdx = 4;
	m_DesiredDashYaw = 0.f;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	CombatRange->SetSphereRadius(getUnitStat()->AttackRange);

	m_pMyController = Cast<AMyCharacterController>(GetController());
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCharacter::initStat(float MaxHP)
{
	m_FStat.m_MaxHP = MaxHP;
	m_FStat.m_HP = m_FStat.m_MaxHP;
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent); // PlayerInputComponent가 false라면 코드 멈춤

}

void AMyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMyCharacter::DestroyChar()
{
	Super::DestroyChar();
	FInputModeUIOnly InputModeData;
	m_pMyController->SetInputMode(InputModeData);
}

void AMyCharacter::DieEvent()
{
	GameOverUMG();
}

bool AMyCharacter::IsAdventureMode()
{
	return m_bIsAdventureMode;
}

void AMyCharacter::AttackEnemy(AEnemyCharacter* Enemy)
{
	if (GetDistanceTo(Enemy) <= m_FStat.AttackRange)
	{
		CombatComponent->setInit(Enemy, Enemy->getUnitType());
		CombatComponent->Attack();
	}
}

void AMyCharacter::GainEXT(int32 nEXP)
{
	bool IsLvUp = false;
	while (nEXP > 0)
	{
		getUnitStat()->CurEXP += nEXP;
		nEXP = 0;
		if (getUnitStat()->CurEXP >= getUnitStat()->LvUpEXP)
		{
			nEXP = getUnitStat()->CurEXP - getUnitStat()->LvUpEXP;
			IsLvUp = true;
			LevelUp();
		}
	}

	if (IsLvUp)
	{
		UGameplayStatics::SpawnSound2D(GetWorld(), LevelUpSound);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LevelUpEffect, GetActorLocation());
	}
}

void AMyCharacter::LevelUp()
{
	m_pMyController->CharLevelUp();

	getUnitStat()->CurEXP = 0;
	getUnitStat()->LvUpEXP = (int32)((float)getUnitStat()->LvUpEXP * 1.3f);
	getUnitStat()->m_HP = getUnitStat()->m_MaxHP;
	getUnitStat()->m_MP = getUnitStat()->m_MaxMP;
	HPWarningOff();
}

void AMyCharacter::MediateAttackRange(float fAddRangeAmount)
{
	CombatRange->SetSphereRadius(CombatRange->GetScaledSphereRadius() + fAddRangeAmount);
}
