// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "Skill/SkillBase.h"
#include "MyCharacter.h"
#include "MyCharacterController.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
USkillComponent::USkillComponent() :
	m_nMaxLength{},
	m_bCanSkillActive{},
	m_bCanCoolDown{},
	m_nSkillPoint{},
	m_arSkill{},
	m_arSkillMontageName{},
	m_mapCantDuplicatedSkill{}
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void USkillComponent::initSkill()
{
	// m_arSkill 초기화
	for (int32 SkillNum = (int32)ESkillNum::E_Skill_1; SkillNum < (int32)ESkillNum::E_MAX; SkillNum++)
	{
		FString RowName = FString::FromInt(SkillNum);
		if ((SkillDB->FindRow<FSkillData>(FName(*RowName) , RowName) != nullptr))
		{
			m_arSkill[SkillNum] = *(SkillDB->FindRow<FSkillData>(FName(*RowName), RowName));
			if (!m_arSkill[SkillNum].CanDuplicated)
				m_mapCantDuplicatedSkill.Add((ESkillNum)SkillNum, nullptr);
		}
	}

	// ======================================= 시작할 때 스킬 하나 주고 시작 =============================
	m_arSkill[0].SKill_CurLev++;
	m_arSkill[0].SKill_CoolTime += m_arSkill[0].LvUpChangeSkill_CoolTime;
	m_arSkill[0].Skill_MP += m_arSkill[0].LvUpChangeSkill_MP;
	// =================================================================================================

	// m_arSkillMontageName 초기화
	m_arSkillMontageName[(int32)ESkillNum::E_Skill_1] = FName("Projectile");
	m_arSkillMontageName[(int32)ESkillNum::E_Skill_2] = FName("Summon");
	m_arSkillMontageName[(int32)ESkillNum::E_Skill_3] = FName("Summon");
	m_arSkillMontageName[(int32)ESkillNum::E_Skill_4] = FName("Projectile");
	m_arSkillMontageName[(int32)ESkillNum::E_Skill_5] = FName("Buff");
	m_arSkillMontageName[(int32)ESkillNum::E_Skill_6] = FName("Buff");
}

bool USkillComponent::ConsumeMP(int32 nNeedMP)
{
	if (m_pOwnerUnit->getUnitStat()->m_MP >= nNeedMP)
	{
		m_pOwnerUnit->getUnitStat()->m_MP -= nNeedMP;
		UE_LOG(LogTemp, Warning, TEXT("MP Consume"));
		return true;
	}

	GEngine->AddOnScreenDebugMessage(1, 1.5f, FColor::Red, FString::Printf(TEXT("Need more MP")));
	return false;
}

void USkillComponent::init()
{
	m_nSkillPoint = 4;

	m_nMaxLength = (int32)ESkillNum::E_MAX;

	m_arSkill.Reserve(m_nMaxLength);
	m_arSkill.Init(FSkillData(), m_nMaxLength);

	m_arSkillMontageName.Reserve(m_nMaxLength);
	m_arSkillMontageName.Init(FName(), m_nMaxLength);

	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_pOwnerUnit = Cast<AMyCharacter>(m_pMyController->GetPawn());

	// 스킬 사용 여부 초기화 
	m_bCanSkillActive.Reserve(m_nMaxLength);
	m_bCanSkillActive.Init(true, m_nMaxLength);
	m_bCanCoolDown.Reserve(m_nMaxLength);
	m_bCanCoolDown.Init(false, m_nMaxLength);
	
	initSkill();
}

void USkillComponent::releaseSkill(ASkillBase* pReleaseSkill)
{
	const ESkillNum* find = m_mapCantDuplicatedSkill.FindKey(pReleaseSkill);
	if (find != nullptr)
	{
		(*m_mapCantDuplicatedSkill.Find(*find))->Disappear();
		*(m_mapCantDuplicatedSkill.Find(*find)) = nullptr;
		return;
	}

	pReleaseSkill->Disappear();
}

ASkillBase* USkillComponent::SpawnSkill(ESkillNum eSkillNum)
{
	if (!IsValid(m_pOwnerUnit))
		return nullptr;
	UWorld* world = GetWorld();
	
	FActorSpawnParameters spawnParams{};
	FRotator rotator{};
	FVector location{};

	rotator = m_pOwnerUnit->GetActorRotation();
	
	location = m_pOwnerUnit->GetMesh()->GetSocketLocation("SpawnSkillPoint");
	if (m_arSkill[(int32)eSkillNum].SkillType == ESkillType::E_SummonsFix)
	{
		location = m_pMyController->getMouseLocation();
		location.Z += 50.f;
	}
	// ASkillBase* SpawnedSkill = world->SpawnActor<ASkillBase>(m_arSkill[(int)eSkillNum].SpawnSkill.Get(), location, rotator, spawnParams);
	ASkillBase* SpawnedSkill = Cast<ASkillBase>(world->SpawnActor(m_arSkill[(int)eSkillNum].SpawnSkill, &location));
	SpawnedSkill->ED_ActivateSuccessful.BindUFunction(this, FName("SkillAnimPlay"));
	SpawnedSkill->setSkillOwner(m_pOwnerUnit);
	SpawnedSkill->init(m_arSkill[(int32)eSkillNum].SKill_CurLev, m_arSkill[(int32)eSkillNum].Skill_MP, (int32)eSkillNum);

	return SpawnedSkill;
}

void USkillComponent::resetSkillCoolTime(int32 nSkillNum)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill Reset"));
	m_bCanSkillActive[nSkillNum] = true;
	m_bCanCoolDown[nSkillNum] = false;
}

void USkillComponent::SkillAnimPlay(int32 nSkillNum)
{
	if (m_pOwnerUnit == nullptr)
		return;

	if (m_pOwnerUnit->getUnitState() == EUNIT_STATE::E_Dead ||
		m_pOwnerUnit->getUnitState() == EUNIT_STATE::E_UnderAttack)
		return;

	m_pOwnerUnit->setUnitState(EUNIT_STATE::E_Attack);
	UAnimInstance* AnimIns = m_pOwnerUnit->GetMesh()->GetAnimInstance();
	if (AnimIns && SkillMontage)
	{
		AnimIns->Montage_Play(SkillMontage, 1.f); // 0.5로하면 기존 스피드 1/2배
		AnimIns->Montage_JumpToSection(m_arSkillMontageName[nSkillNum], SkillMontage);
	}
	m_bCanCoolDown[nSkillNum] = true;

	// 스킬 쿨타임 적용
	if (m_arSkill[nSkillNum].SKill_CoolTime <= 0)
		return;

	m_bCanSkillActive[nSkillNum] = false;
	FTimerHandle ResetTimer{};
	GetWorld()->GetTimerManager().SetTimer(ResetTimer, [this, nSkillNum] {
		if (IsValid(this))
			this->resetSkillCoolTime(nSkillNum);
		else
		{
			//this died before callback triggered
		}
		}, m_arSkill[nSkillNum].SKill_CoolTime, false);
}

void USkillComponent::SKillLevelUp(ESkillNum eSkillNum)
{
	if (m_arSkill.IsValidIndex((int32)eSkillNum) && 
		m_arSkill[(int32)eSkillNum].SKill_CurLev < m_arSkill[(int32)eSkillNum].Skill_MaxLev)
	{
		m_arSkill[(int32)eSkillNum].SKill_CurLev++;
		m_arSkill[(int32)eSkillNum].SKill_CoolTime += m_arSkill[(int32)eSkillNum].LvUpChangeSkill_CoolTime;
		m_arSkill[(int32)eSkillNum].Skill_MP += m_arSkill[(int32)eSkillNum].LvUpChangeSkill_MP;
		m_nSkillPoint--;
		m_pMyController->SkillFrameRefresh();
	}
}

int32 USkillComponent::getSkillPoint()
{
	return m_nSkillPoint;
}

void USkillComponent::UsingSkill(ESkillNum eSkillNum)
{
	if (m_arSkill.IsValidIndex((int32)eSkillNum) &&
		m_arSkill[(int32)eSkillNum].SpawnSkill != nullptr && 
		ConsumeMP(m_arSkill[(int32)eSkillNum].Skill_MP))
	{
		// 쿨타임이 있는 스킬이고 아직 쿨타임이 남아있다면 스킬 발동 실패s
		if (!m_bCanSkillActive[(int32)eSkillNum])
		{
			ConsumeMP(m_arSkill[(int32)eSkillNum].Skill_MP * -1);
			return;
		}

		// 중복 스폰이 불가능한 스킬이라면 (Summons Skill) 스폰된거 처리 하고 스폰해야 한다.
		if (!m_arSkill[(int32)eSkillNum].CanDuplicated && *(m_mapCantDuplicatedSkill.Find(eSkillNum)) != nullptr)
		{
			releaseSkill(*(m_mapCantDuplicatedSkill.Find(eSkillNum)));
		}

		// 스킬 스폰
		ASkillBase* pSpawnedSkill = SpawnSkill(eSkillNum);
		
		// 스폰된 스킬. '중복 안되는 스킬인데 스폰된 스킬' 저장소인 TMap에 저장 Key : eSkillNum  Value : 스폰되어있는 스킬
		if (m_mapCantDuplicatedSkill.Contains(eSkillNum))
			*(m_mapCantDuplicatedSkill.Find(eSkillNum)) = pSpawnedSkill;
	}
}

void USkillComponent::AddSkillPoint(int32 nPoint)
{
	m_nSkillPoint += nPoint;
}

void USkillComponent::SkillAnimPlayEnd()
{
	m_pOwnerUnit->setUnitState(EUNIT_STATE::E_Idle);
}

bool USkillComponent::IsSkillCoolTimeOK(ESkillNum eSkillNum)
{
	return m_bCanCoolDown[(int32)eSkillNum];
}

bool USkillComponent::IsSkillCanActivat(ESkillNum eSkillNum)
{
	return m_bCanSkillActive[(int32)eSkillNum];
}

