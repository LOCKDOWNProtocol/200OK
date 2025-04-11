#include "MW/GasCylider.h"

AGasCylider::AGasCylider()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGasCylider::BeginPlay()
{
	Super::BeginPlay();

}

void AGasCylider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGasCylider::Explode()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Black, TEXT("해당 가스통 터짐!"));
	// Todo. 파티클, 사운드, 콜리전 처리
}
