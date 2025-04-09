#include "SJ/Components/InteractionComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedPlayerInput.h"
#include "SJ/SJ_Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "MW/Items.h"
#include "SJ/SJ_TestButton.h"
#include "SJ/SJ_PlayerAnimInstance.h"
#include "MW/Tablet.h"
#include "MW/Knife.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	FindTablet();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInteractionComponent::SetupInputBinding(class UEnhancedInputComponent* Input)
{
	Super::SetupInputBinding(Input);

	Input->BindAction(IA_PrimaryAction, ETriggerEvent::Started, this, &ThisClass::InputPrimaryAction);
	Input->BindAction(IA_SecondaryAction, ETriggerEvent::Started, this, &ThisClass::InputSecondaryAction);
	Input->BindAction(IA_ReleaseItem, ETriggerEvent::Started, this, &ThisClass::InputFKey);
	Input->BindAction(IA_Inventory, ETriggerEvent::Started, this, &ThisClass::Inventory);
	Input->BindAction(IA_Tablet, ETriggerEvent::Started, this, &ThisClass::TakeTablet);

}

void UInteractionComponent::InputPrimaryAction()
{
	// 아이템 장비 중이면 아이템 휘두르기 공격
	if ( bHasItem ) {
		AttackItem();
		return;
	}

	// 칼 장비 중이면 칼 찌르기 공격
	if ( bHasKnife ) {
		StabKnife();
		return;
	}

	// 맨손 일 때 아이템인지 버튼인지 먼저 판별
	FHitResult HitResult;
	FVector StartPos=me->CameraComp->GetComponentLocation();
	FVector EndPos=StartPos + me->CameraComp->GetForwardVector() * TraceLength;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(me);

	DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, 1.0f, 0, 1.0f);
	bool bHit=GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndPos, ECC_Visibility, Params);
	if ( !bHit )return;
	AActor* HitActor=HitResult.GetActor();

	// 아이템이라면 줍고 버튼이라면 누르기 
	if ( Cast<AItems>(HitActor) ) {
		// knife일 경우에는 다른 애니메이션 출력하고 싶다
		if ( AKnife* Knife=Cast<AKnife>(HitActor) )
		{
			HoldAKnife(HitActor);
			return;
		}
		PickupItem(HitActor);
	}
	else if ( Cast<ASJ_TestButton>(HitActor) ) {
		PressButton(HitActor);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, TEXT("Cannot Interaction Actor"));
	}
}

void UInteractionComponent::PickupItem(AActor* HitActor)
{
	if ( bHasTablet || bHasKnife) return;

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("Item Casting Success"));

	// AnimInstance -> bHasItem = true로 변경
	if ( USJ_PlayerAnimInstance* AnimInst=Cast<USJ_PlayerAnimInstance>(me->GetMesh()->GetAnimInstance()) )
	{
		AnimInst->bHasItem=true;
	}
	// 아이템 소유 표시
	bHasItem=true;
	ownedItem=HitActor;
	// 소켓에 붙이기
	if ( UStaticMeshComponent* ItemMesh=HitActor->FindComponentByClass<UStaticMeshComponent>() )
	{
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ItemMesh->AttachToComponent(me->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("ItemPos"));
		ItemMesh->SetRelativeLocation(FVector(-11.f, 3.f, 0.f));
		ItemMesh->SetRelativeRotation(FRotator(0.f, -180.f, -180.f));
	}
}

void UInteractionComponent::HoldAKnife(AActor* HitActor)
{
	if ( bHasTablet || bHasItem) return;

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Knife Casting Success"));

	// AnimInstance -> bHasItem = true로 변경
	if ( USJ_PlayerAnimInstance* AnimInst=Cast<USJ_PlayerAnimInstance>(me->GetMesh()->GetAnimInstance()) )
	{
		AnimInst->bHasKnife=true;
	}
	// 아이템 소유 표시
	bHasKnife=true;
	ownedItem=HitActor;
	// 소켓에 붙이기
	if ( UStaticMeshComponent* ItemMesh=HitActor->FindComponentByClass<UStaticMeshComponent>() )
	{
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ItemMesh->AttachToComponent(me->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("ItemPos"));
		ItemMesh->SetRelativeLocation(FVector(-11.f, 3.f, 0.f));
		ItemMesh->SetRelativeRotation(FRotator(0.f, -180.f, -180.f));
	}
}

void UInteractionComponent::AttackItem()
{
	// 근접공격 : 장비중일 때 아이템으로 휘두르기
	if ( !bHasItem || !ownedItem || bHasTablet ) return;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Item Attack!"));
}

void UInteractionComponent::StabKnife()
{
	// 근접공격 : 장비중일 때 칼로 찌르기
	if ( !bHasKnife || !ownedItem || bHasTablet ) return;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Knife Stab!"));
}

void UInteractionComponent::ReleaseItem()
{
	// 장비중일 때 아이템 버리기
	if ( !bHasItem || !ownedItem || bHasTablet ) return;

	// 라인트레이스로 mesh에 붙인 아이템을 버리기
	// 소유off + Detach + 물리on + 콜리전처리
	if ( UStaticMeshComponent* ItemMesh=ownedItem->FindComponentByClass<UStaticMeshComponent>() )
	{
		ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetCollisionObjectType(ECC_PhysicsBody);
	}

	// AnimInstance -> bHasItem = false로 변경
	if ( USJ_PlayerAnimInstance* AnimInst=Cast<USJ_PlayerAnimInstance>(me->GetMesh()->GetAnimInstance()) )
	{
		AnimInst->bHasItem=false;
	}

	bHasItem=false;
	ownedItem=nullptr;

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue, TEXT("Release Item!"));
}

void UInteractionComponent::ReleaseKnife()
{
	// 장비중일 때 아이템 버리기
	if ( !bHasKnife || !ownedItem || bHasTablet ) return;

	// 라인트레이스로 mesh에 붙인 아이템을 버리기
	// 소유off + Detach + 물리on + 콜리전처리
	if ( UStaticMeshComponent* ItemMesh=ownedItem->FindComponentByClass<UStaticMeshComponent>() )
	{
		ItemMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->SetCollisionObjectType(ECC_PhysicsBody);
	}

	// AnimInstance -> bHasItem = false로 변경
	if ( USJ_PlayerAnimInstance* AnimInst=Cast<USJ_PlayerAnimInstance>(me->GetMesh()->GetAnimInstance()) )
	{
		AnimInst->bHasKnife=false;
	}

	bHasKnife=false;
	ownedItem=nullptr;

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue, TEXT("Release Item!"));
}

void UInteractionComponent::InputFKey()
{
	if ( bHasItem ) {
		ReleaseItem();
	}
	else {
		ReleaseKnife();
	}
}


void UInteractionComponent::PressButton(AActor* HitActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Button Casting Success"));

	// 테스트용 거리 : 버튼과의 거리가 1m 이상일 경우 누르지 못함
	float Distance=FVector::Dist(HitActor->GetActorLocation(), me->GetActorLocation());
	if ( Distance > 100.f ) return;

	// 버튼 누르기
	if ( USJ_PlayerAnimInstance* AnimInst=Cast<USJ_PlayerAnimInstance>(me->GetMesh()->GetAnimInstance()) )
	{
		AnimInst->PlayPressButtonAnim();
	}
}

void UInteractionComponent::InputSecondaryAction()
{
	// Todo. Mouse R 액션
	// 일단 아이템 착용 중이 아닐땐 리턴으로만 테스트
	if ( !bHasItem || !ownedItem || !bHasKnife) return;

	// 놓을 수 있는 곳이 있을 경우, 특정 위치에 두울 수 있다
	// else 아이템 던지기
	ThrowItem();
}

void UInteractionComponent::ThrowItem()
{
	// 아이템에 Projectile 컴포넌트 추가해서 던지자
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("Mouse Right"));
}


void UInteractionComponent::Inventory()
{
	// Todo. E key 누르면 아이템을 인벤토리에 보관
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("E key : Inventory"));
}

void UInteractionComponent::TakeTablet()
{
	// Todo. 태블릿 들고 이동도 가능해야함, 아이템 줍기, 공격 등은 불가능
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, TEXT("Q key : TakeTablet"));
	
	if ( TabletActor )
	{
		bHasTablet = !bHasTablet;
		me->isTakingTablet = bHasTablet;

		if(bHasTablet)
			TabletActor->SwitchTablet(bHasTablet);
		else
			TabletActor->SwitchTablet(bHasTablet);
	}
}

void UInteractionComponent::FindTablet()
{
	TArray <AActor*> ChildArray;
	me->GetAllChildActors(ChildArray);

	for ( auto& c : ChildArray )
	{
		if ( c->GetActorNameOrLabel().Contains("BP_Tablet") )
		{
			TabletActor = Cast<ATablet>(c);
		}
	}
}