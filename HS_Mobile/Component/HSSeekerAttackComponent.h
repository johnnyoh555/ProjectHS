#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HSSeekerAttackComponent.generated.h"

// 전방 선언
class UBoxComponent;
class IHSAttackableInterface;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HS_MOBILE_API UHSSeekerAttackComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHSSeekerAttackComponent();

    // === 공격 요청 ===
    void RequestAttack();
    void HandleAttackHitNotify();

protected:
    // === 컴포넌트 생명주기 ===
    virtual void OnRegister() override;
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // === 감지 영역 및 타겟 관리 ===
    UPROPERTY(VisibleAnywhere, Category = "Attack")
    TObjectPtr<UBoxComponent> AttackBox;

    UPROPERTY()
    TArray<TScriptInterface<IHSAttackableInterface>> DetectedTargets;

    UFUNCTION()
    void OnAttackBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnAttackBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    TScriptInterface<IHSAttackableInterface> FindNearestTarget();

    // === 공격 애니메이션 ===
    UPROPERTY(Replicated, EditAnywhere, Category = "Attack")
    TObjectPtr<UAnimMontage> AttackMontage;

    UFUNCTION(NetMulticast, Unreliable)
    void MulticastPlayAttackMontage();

    void PerformAttack();

    // === 쿨타임 처리 ===
    UPROPERTY(EditDefaultsOnly, Category = "Attack")
    float CooldownTime = 5.0f;

    bool bCanAttack = true;
    FTimerHandle AttackCooldownHandle;

    void StartAttackCooldown();
    void ResetAttackCooldown();

    // === 클라이언트 쿨타임 예측 ===
    bool bLocalCanAttack = true;
    FTimerHandle LocalAttackCooldownHandle;

    UFUNCTION(Client, Reliable)
    void ClientStartLocalCooldown();

    // === 서버 RPC ===
    UFUNCTION(Server, Reliable)
    void ServerPerformAttack();
};
