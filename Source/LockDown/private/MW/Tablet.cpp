// Fill out your copyright notice in the Description page of Project Settings.


#include "MW/Tablet.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "SJ/SJ_Character.h"
#include "shDebug.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATablet::ATablet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowComp = CreateDefaultSubobject<UArrowComponent> (TEXT("ArrowComp"));
	SetRootComponent(ArrowComp);

	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetRelativeRotation (FRotator(0.f, -90.f, 0.f));
	MeshComp->SetupAttachment (RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TmpMesh(TEXT("/Script/Engine.StaticMesh'/Game/MW/Assets/Tablet/scene.scene'"));
	if ( TmpMesh.Succeeded() )
	{
		MeshComp->SetStaticMesh (TmpMesh.Object);
	}

	WidgetComp = CreateDefaultSubobject <UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment (MeshComp);
	WidgetComp->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	ConstructorHelpers::FClassFinder<UUserWidget> TmpWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MW/UI/Tablet/WBP_TabletWidget.WBP_TabletWidget_C'"));
	if ( TmpWidget.Succeeded() )
	{
		WidgetComp->SetWidgetClass (TmpWidget.Class);
	}




}

// Called when the game starts or when spawned
void ATablet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATablet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ATablet::SwitchTablet(bool value)
{
	player = Cast<ASJ_Character>(UGameplayStatics::GetPlayerPawn (GetWorld (),0));
	if (!player ) return;

	APlayerController* pc = Cast<APlayerController>(player->GetController ());
	if(!pc ) return;

	isTabletOpen = value;
	
	if ( isTabletOpen )
	{
		pc->SetShowMouseCursor (isTabletOpen);
	}
	else
	{
		pc->SetShowMouseCursor (isTabletOpen);
	}
}

