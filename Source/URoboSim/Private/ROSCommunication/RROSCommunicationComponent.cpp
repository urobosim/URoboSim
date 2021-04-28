#include "ROSCommunication/RROSCommunicationComponent.h"
#include "Controller/RControllerComponent.h"

URROSCommunicationComponent::URROSCommunicationComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
  PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void URROSCommunicationComponent::BeginPlay()
{
  Super::BeginPlay();

  AActor * Owner = GetOwner();
  ROSCommunication.ControllerComponent = Owner->FindComponentByClass<URControllerComponent>();
  if(ROSCommunication.ControllerComponent)
    {
      UE_LOG(LogTemp, Error, TEXT("Found controller comp"));
      ROSCommunication.Init();
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("No FROSBridgeHandler created."));
    }
}

void URROSCommunicationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  ROSCommunication.Tick();
}

void URROSCommunicationComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  ROSCommunication.DeInit();
  Super::EndPlay(EndPlayReason);
}
