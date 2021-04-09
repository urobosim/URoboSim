#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAStatusPublisher.h"
#include "Controller/RControllerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRFJTAStatusPublisher, Log, All);

URFJTAStatusPublisher::URFJTAStatusPublisher()
{
  PublisherParameters = CreateDefaultSubobject<URActionStatusPublisherParameter>(TEXT("FJTAStatusPublisherParameters"));
}

void URFJTAStatusPublisher::Init()
{
  if (!PublisherParameters)
  {
    PublisherParameters = CreateDefaultSubobject<URActionStatusPublisherParameter>(TEXT("FJTAStatusPublisherParameters"));
  }
  
  if (GetOwner())
  {
    URControllerComponent *ControllerComponent = Cast<URControllerComponent>(GetOwner()->GetPlugin(TEXT("ControllerComponent")));
    if (ControllerComponent)
    {
      Controller = ControllerComponent->GetController(TEXT("JointController"));
    }
    else
    {
      UE_LOG(LogRFJTAStatusPublisher, Error, TEXT("JointController not found in %s"), *GetName())
    }
  }
}