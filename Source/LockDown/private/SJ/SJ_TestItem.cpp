#include "SJ/SJ_TestItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

ASJ_TestItem::ASJ_TestItem()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	BoxComp->SetBoxExtent(FVector(50));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(BoxComp);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TmpMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (TmpMesh.Succeeded()) {
		MeshComp->SetStaticMesh(TmpMesh.Object);
	}
}

void ASJ_TestItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASJ_TestItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

