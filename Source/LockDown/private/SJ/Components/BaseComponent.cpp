#include "SJ/Components/BaseComponent.h"
#include "SJ/SJ_Character.h"

UBaseComponent::UBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}


void UBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	me=Cast<ASJ_Character>(GetOwner());
}

void UBaseComponent::InitializeComponent()
{
	me=Cast<ASJ_Character>(GetOwner());
	me->InputBindingDelegate.AddUObject(this, &UBaseComponent::SetupInputBinding);
}

void UBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBaseComponent::SetupInputBinding(class UEnhancedInputComponent* Input)
{

}
