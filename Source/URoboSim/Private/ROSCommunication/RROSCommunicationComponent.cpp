#include "ROSCommunication/RROSCommunicationComponent.h"
#include "Controller/RControllerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRROSCommunication, Log, All);

URROSCommunicationComponent::URROSCommunicationComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
  PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void URROSCommunicationComponent::BeginPlay()
{
  Super::BeginPlay();

  ROSCommunication.ControllerComponent = GetOwner()->FindComponentByClass<URControllerComponent>();
  if (ROSCommunication.ControllerComponent)
  {
    UE_LOG(LogRROSCommunication, Log, TEXT("Found controller component"));
    ROSCommunication.Init();
  }
  else
  {
    UE_LOG(LogRROSCommunication, Error, TEXT("No FROSBridgeHandler created."));
  }
}

void URROSCommunicationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
  ROSCommunication.Tick();
}

void URROSCommunicationComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  ROSCommunication.DeInit();
  Super::EndPlay(EndPlayReason);
}