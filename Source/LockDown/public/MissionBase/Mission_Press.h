#pragma once

#include "CoreMinimal.h"
#include "MissionBase/MissionBase.h"
#include "Mission_Press.generated.h"


UCLASS()
class LOCKDOWN_API AMission_Press : public AMissionBase
{
	GENERATED_BODY()
	

public:
	virtual void StartMission() override;
	virtual int32 ClearMission() override;


public:
    // 연결된 가스통 4개
    UPROPERTY()
    TArray<class AGasCylider*> SlotCylinders;       // 슬롯당 1개의 가스통 넣는 곳
    TArray<bool> SlotIsSafe;                        // 슬롯 안정 상태

    FTimerHandle PressureTestTimer;                 // 압력테스트 타이머

    // 각 장치 상태
    bool bIsMachine1Clear;                          
    bool bIsMachine2Clear;

    // 압력 판단용 배열
    TArray<int32> Machine1Pressures;
    TArray<int32> Machine2Pressures;

    // 미션 진행 함수들
    void InitializeArray();                         // 랜덤 장착
    void MiddleCheck();                             // 체크 후 파괴 여부 판단
    void FinalCheck();                              // 최종 클리어 여부
    void StopMachine();                             // 빨간 버튼으로 중지
};
