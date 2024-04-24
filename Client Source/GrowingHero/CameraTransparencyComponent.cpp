// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTransparencyComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Containers/Set.h"

// Sets default values for this component's properties
UCameraTransparencyComponent::UCameraTransparencyComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    CapsulePercentageForTrace = 1.0f;
    DebugLineTraces = true;
    IsOcclusionEnabled = true;
}


void UCameraTransparencyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

// Called when the game starts
void UCameraTransparencyComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UCameraTransparencyComponent::init(USpringArmComponent* SpringArm, UCameraComponent* Camera, UCapsuleComponent* Capsule, APawn* Pawn)
{
    ActiveSpringArm = SpringArm;
    ActiveCamera = Camera;
    ActiveCapsuleComponent = Capsule;
    ActivePawn = Pawn;

    FTimerHandle DestroyTimer{};
    GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &UCameraTransparencyComponent::SyncOccludedActors, 0.1f, true, 0.f);
}

bool UCameraTransparencyComponent::OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
    for (int i = 0; i < OccludedActor.StaticMesh->GetNumMaterials(); ++i)
    {
        OccludedActor.StaticMesh->SetMaterial(i, FadeMaterial);
    }

    return true;
}

void UCameraTransparencyComponent::ShowOccludedActor(FCameraOccludedActor& OccludedActor)
{
    if (!IsValid(OccludedActor.Actor))
    {
        OccludedActors.Remove(OccludedActor.Actor);
    }

    OccludedActor.IsOccluded = false;
    OnShowOccludedActor(OccludedActor);
}

bool UCameraTransparencyComponent::OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
    for (int matIdx = 0; matIdx < OccludedActor.Materials.Num(); ++matIdx)
    {
        OccludedActor.StaticMesh->SetMaterial(matIdx, OccludedActor.Materials[matIdx]);
    }

    return true;
}

void UCameraTransparencyComponent::ForceShowOccludedActors()
{
    for (auto& Elem : OccludedActors)
    {
        if (Elem.Value.IsOccluded)
        {
            ShowOccludedActor(Elem.Value);
        }
    }
}

bool UCameraTransparencyComponent::HideOccludedActor(const AActor* Actor)
{
    // 우리의 OccludedActors에 해당 Actor가 우선 있는지 확인한다.
    FCameraOccludedActor* ExistingOccludedActor = OccludedActors.Find(Actor);

    // 이미 투명화 된 상태라면 그냥 넘어가고
    if (ExistingOccludedActor && ExistingOccludedActor->IsOccluded)
    {
        return false;
    }

    if (ExistingOccludedActor && IsValid(ExistingOccludedActor->Actor))
    {
        ExistingOccludedActor->IsOccluded = true; // 투명화 되었다고 변수 초기화 해주고
        OnHideOccludedActor(*ExistingOccludedActor); // 우리가 만든 투명 머티리얼로 바꾸어 주기
    }
    else // 우리의 OccludedActors에는 없던 Actor라면?
    {
        UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(
            Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
        if (StaticMesh)
        {
            FCameraOccludedActor OccludedActor;
            OccludedActor.Actor = Actor;
            OccludedActor.StaticMesh = StaticMesh;
            OccludedActor.Materials = StaticMesh->GetMaterials();
            OccludedActor.IsOccluded = true;
            OccludedActors.Add(Actor, OccludedActor); // 새롭게 우리의 OccludedActors에 추가해주고
            OnHideOccludedActor(OccludedActor); // 우리가 만든 투명 머티리얼로 바꾸어 준다.
        }
    }

    return true;
}

void UCameraTransparencyComponent::SyncOccludedActors()
{
    // 기본 조건 체크
    if (!ShouldCheckCameraOcclusion()) return;

    // 스프링암이 물체 가까이 가면 거리가 확 좁아지는게 켜져있다면
    if (ActiveSpringArm->bDoCollisionTest)
    {
        ForceShowOccludedActors();
        // -> ShowOccludedActor
        // -> OnShowOccludedActor : OccludedActors에 들어있는 Actor들 다 원래 Material로 바꾸어줌
        return;
    }

    // 이제 겹치는 놈들은 다 투명처리 할 것이다.


    // ========================================================= 겹침 애들 찾기 ==============================================
    FVector Start = ActiveCamera->GetComponentLocation();
    FVector End = ActivePawn->GetActorLocation();

    TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;
    CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
    TArray<AActor*> ActorsToIgnore;
    TArray<FHitResult> OutHits;

    // 라인트레이스 디버그용으로 그리기 위함. 그리기 싫으면 EDrawDebugTrace::None 사용하면 된다.
    auto ShouldDebug = DebugLineTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

    bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
        GetWorld(), Start, End, ActiveCapsuleComponent->GetScaledCapsuleRadius() * CapsulePercentageForTrace,
        ActiveCapsuleComponent->GetScaledCapsuleHalfHeight() * CapsulePercentageForTrace, CollisionObjectTypes, true,
        ActorsToIgnore,
        EDrawDebugTrace::None,
        OutHits, true, FLinearColor::Red, FLinearColor::Green, 5.f);
    // ========================================================= 겹침 애들 찾기 종료 ==========================================

    if (bGotHits) // 겹친게 있음.
    {
        // 라인트레이스에 겹친 애들 거를건데, 얘네는 이제 투명화 될 것이다.
        TSet<const AActor*> ActorsJustOccluded;

        // 투명화 작업
        for (FHitResult Hit : OutHits)
        {
            const AActor* HitActor = Cast<AActor>(Hit.GetActor());
            HideOccludedActor(HitActor);
            // OnHideOccludedActor : 해당 Actor 투명화 작업 진행

            ActorsJustOccluded.Add(HitActor); // 같은 Mesh를 반복해서 처리하지 않기 위한 Set 작업
        }

        // 이미 투명화 된 Actor들 + 이제 투명화 된 Actor들은 OccludedActors에 저장되어 있다. 
        // OccludedActors에 들어있는 Actor들 중에 ActorsJustOccluded에 없다면? 이제 투명화 종료해야 하는 Actor인 것이다.
        for (auto& Elem : OccludedActors)
        {
            // OccludedActors안에는 있지만 ActorsJustOccluded에는 없는 Actor. 그리고 그 Actor가 현재 투명화 되어있다면
            if (!ActorsJustOccluded.Contains(Elem.Value.Actor) && Elem.Value.IsOccluded)
            {
                // 해당 Actor는 이제 투명화 종료
                ShowOccludedActor(Elem.Value);
            }
        }
    }
    else // 라인 트레이스에 겹치는 Actor가 하나도 없다는 소리. 즉, 모든 OccludedActors안의 Actor들은 이제 투명화 종료 시켜야 한다.
    {
        ForceShowOccludedActors();
    }
}