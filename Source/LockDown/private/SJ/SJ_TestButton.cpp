#include "SJ/SJ_TestButton.h"
#include "Components/BoxComponent.h"
#include "Components/BoxComponent.h"

ASJ_TestButton::ASJ_TestButton()
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

void ASJ_TestButton::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASJ_TestButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

