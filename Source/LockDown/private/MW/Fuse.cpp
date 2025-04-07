// Fill out your copyright notice in the Description page of Project Settings.


#include "MW/Fuse.h"
#include <utility>
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "shDebug.h"

AFuse::AFuse()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject <UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent (BoxComp);

	FirstMesh = CreateDefaultSubobject<UStaticMeshComponent> (TEXT("FirstMesh"));
	FirstMesh->SetupAttachment (RootComponent);

	SecondMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("SecondMesh"));
	SecondMesh->SetupAttachment (RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> FirstTmp(TEXT("/Script/Engine.StaticMesh'/Game/MW/Assets/Fuse/obj_bevel_block.obj_bevel_block'"));
	if ( FirstTmp.Succeeded() )
	{
		FirstMesh->SetStaticMesh (FirstTmp.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> SecondTmp(TEXT("/Script/Engine.StaticMesh'/Game/MW/Assets/Fuse/obj_slant_block2.obj_slant_block2'"));
	if ( SecondTmp.Succeeded() )
	{
		SecondMesh->SetStaticMesh(SecondTmp.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> TmpRedMat(TEXT("/Script/Engine.Material'/Game/MW/Assets/Fuse/Mat_Fuse_Red.Mat_Fuse_Red'"));
	if ( TmpRedMat.Succeeded() )
	{
		MatColors.Add(TmpRedMat.Object);
		ColorMap.Add (PartColor::Red, TmpRedMat.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial> TmpYellowMat(TEXT("/Script/Engine.Material'/Game/MW/Assets/Fuse/Mat_Fuse_Yellow.Mat_Fuse_Yellow'"));
	if ( TmpYellowMat.Succeeded() )
	{
		MatColors.Add(TmpYellowMat.Object);
		ColorMap.Add(PartColor::Yellow, TmpYellowMat.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial> TmpBlueMat(TEXT("/Script/Engine.Material'/Game/MW/Assets/Fuse/Mat_Fuse_Blue.Mat_Fuse_Blue'"));
	if ( TmpBlueMat.Succeeded() )
	{
		MatColors.Add(TmpBlueMat.Object);
		ColorMap.Add(PartColor::Blue, TmpBlueMat.Object);
	}

	BoxComp->SetBoxExtent (FVector( 23.f, 22.f, 40.f));
	BoxComp->SetRelativeScale3D (FVector(0.3f));
	FirstMesh->SetRelativeScale3D (FVector(5.f, 10.f, 10.f));
	FirstMesh->SetRelativeLocation (FVector(( -9.f, 0.f, -4.3f )));
	SecondMesh->SetRelativeScale3D(FVector(5.f, 10.f, 10.f));
	SecondMesh->SetRelativeLocation (FVector(10.f, 0.f, -4.3f ));


}
// Called when the game starts or when spawned
void AFuse::BeginPlay()
{
	Super::BeginPlay();
		

}
// Called every frame
void AFuse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}