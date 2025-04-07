// Fill out your copyright notice in the Description page of Project Settings.


#include "MW/Battery.h"
#include "Components/BoxComponent.h"

ABattery::ABattery()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject <UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent (BoxComp);

	MeshComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment (RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TmpMesh(TEXT("/Script/Engine.StaticMesh'/Game/MW/Assets/Battery/SmartCell.SmartCell'"));;
	if ( TmpMesh.Succeeded() )
	{
		MeshComp->SetStaticMesh (TmpMesh.Object);
	}

	BoxComp->SetBoxExtent (FVector(22.f, 29.f, 32.f));
	BoxComp->SetRelativeScale3D(FVector(0.4f));

	MeshComp->SetRelativeScale3D (FVector(0.25f));
	MeshComp->SetRelativeLocation (FVector(0.f,0.f, -0.9f));

}
// Called when the game starts or when spawned
void ABattery::BeginPlay()
{
	Super::BeginPlay();
}
// Called every frame
void ABattery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}