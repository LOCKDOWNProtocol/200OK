// Fill out your copyright notice in the Description page of Project Settings.


#include "MW/GasCylider.h"

AGasCylider::AGasCylider()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
// Called when the game starts or when spawned
void AGasCylider::BeginPlay()
{
	Super::BeginPlay();
}
// Called every frame
void AGasCylider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
