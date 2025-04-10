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

	player = Cast<ASJ_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if ( !player ) return;
	player->TabletComp->SetVisibility(isTabletOpen);

	//OnTabletPos = player->CameraComp->GetForwardVector ()
	
}

// Called every frame
void ATablet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ATablet::SwitchTablet(bool value)
{
	if (!player ) return;

	APlayerController* pc = Cast<APlayerController>(player->GetController ());
	if(!pc ) return;

	isTabletOpen = value;
	pc->SetShowMouseCursor(isTabletOpen);

	GetWorldTimerManager ().SetTimer(TimerHandle, this, &ATablet::LerpPosRot, 0.05f, true);

	if( isTabletOpen )
		player->TabletComp->SetVisibility(isTabletOpen);

	
	//TabletActor->SwitchTablet(bHasTablet, OnTabletPos, OffTabletPos, OnTabletRot, OffTabletRot);
}

void ATablet::LerpPosRot()
{
	elapedTime+=0.05;

	float alpha = FMath::Clamp(elapedTime / 0.5f, 0.f, 1.f);

	float c1 = 1.70158f;
	float c3 = c1 + 1.f;

	//alpha = 1 + c3 * FMath::Pow(alpha - 1, 3) + c1 * FMath::Pow(alpha - 1, 2);
	alpha = 1 - FMath::Pow(1 - alpha, 5);
	

	if ( isTabletOpen )
	{
		
		FVector newPos = FMath::Lerp (OffTabletPos, OnTabletPos, alpha);
		FRotator newRot = FMath::Lerp (OffTabletRot, OnTabletRot, alpha);

		player->TabletComp->SetRelativeLocationAndRotation (newPos, newRot);
		

	}
	else
	{
		FVector newPos=FMath::Lerp(OnTabletPos, OffTabletPos, alpha);
		FRotator newRot=FMath::Lerp(OnTabletRot, OffTabletRot, alpha);

		player->TabletComp->SetRelativeLocationAndRotation(newPos, newRot);
	}
	if ( alpha >= 1.f )
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		elapedTime=0.f;
		if( !isTabletOpen )
			player->TabletComp->SetVisibility(isTabletOpen);
	}

}

