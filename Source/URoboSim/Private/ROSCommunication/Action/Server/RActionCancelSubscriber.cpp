#include "ROSCommunication/Action/Server/RActionCancelSubscriber.h"
#include "actionlib_msgs/GoalID.h"

DEFINE_LOG_CATEGORY_STATIC(LogRActionCancelSubscriber, Log, All)

URActionCancelSubscriber::URActionCancelSubscriber()
{
  MessageType = TEXT("actionlib_msgs/GoalID");
}

void URActionCancelSubscriber::Init()
{
  if (GetOwner())
  {
    URControllerComponent *ControllerComponent = Cast<URControllerComponent>(GetOwner()->GetPlugin(TEXT("ControllerComponent")));
    if (ControllerComponent)
    {
      Controller = ControllerComponent->GetController(ControllerName);
    }
    else
    {
      UE_LOG(LogRActionCancelSubscriber, Error, TEXT("%s not found in %s"), *ControllerName, *GetName())
    }
  }
}