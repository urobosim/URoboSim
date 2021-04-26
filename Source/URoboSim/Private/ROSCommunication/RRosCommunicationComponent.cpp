#include "ROSCommunication/RRosCommunicationComponent.h"
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
  RosComunication.ControllerComponent = Owner->FindComponentByClass<URControllerComponent>();
  if(RosComunication.ControllerComponent)
    {
      UE_LOG(LogTemp, Error, TEXT("Found controller comp"));
      RosComunication.Init();
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("No FROSBridgeHandler created."));
    }
}

void URROSCommunicationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  RosComunication.Tick();
}

void URROSCommunicationComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  RosComunication.DeInit();
  Super::EndPlay(EndPlayReason);
}
