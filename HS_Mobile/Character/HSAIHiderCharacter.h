#pragma once

#include "CoreMinimal.h"
#include "Character/HSBaseHiderCharacter.h" // 혹은 네가 상속한 기본 클래스
#include "HSAIHiderCharacter.generated.h"

UCLASS()
class HS_MOBILE_API AHSAIHiderCharacter : public AHSBaseHiderCharacter
{
	GENERATED_BODY()

	public:
	AHSAIHiderCharacter();

protected:
	virtual void BeginPlay() override;

	/** 달리기 상태 전환용 */
	FTimerHandle RunToggleTimerHandle;

	/** 상태 전환 로직 */
	void ToggleRunState();

public:
	/** 현재 움직이고 있는지 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsMoving = false;
};
