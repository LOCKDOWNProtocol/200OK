// Fill out your copyright notice in the Description page of Project Settings.


#include "MW/Knife.h"

AKnife::AKnife()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent (MeshComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/MW/Assets/Knife/yanagiba.yanagiba'"));
	if ( TempMesh.Succeeded() )
	{
		MeshComp->SetStaticMesh (TempMesh.Object);
	}

	MeshComp->SetRelativeScale3D (FVector(0.12f,0.15f,0.15f));


}
// Called when the game starts or when spawned
void AKnife::BeginPlay()
{
	Super::BeginPlay();
}
// Called every frame
void AKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}