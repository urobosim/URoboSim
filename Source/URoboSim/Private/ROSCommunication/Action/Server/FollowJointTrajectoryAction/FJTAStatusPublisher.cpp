#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAStatusPublisher.h"
#include "Controller/RControllerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRFJTAStatusPublisher, Log, All);

void URFJTAStatusPublisher::Init()
{
  if (GetOwner())
  {
    URControllerComponent *ControllerComponent = Cast<URControllerComponent>(GetOwner()->GetPlugin(TEXT("Controllers")));
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