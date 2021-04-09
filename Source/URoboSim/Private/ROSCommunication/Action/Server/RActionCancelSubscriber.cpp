#include "ROSCommunication/Action/Server/RActionCancelSubscriber.h"
#include "actionlib_msgs/GoalID.h"

DEFINE_LOG_CATEGORY_STATIC(LogRActionCancelSubscriber, Log, All)

void URActionCancelSubscriber::Init()
{
  if (!SubscriberParameters)
  {
    SubscriberParameters = CreateDefaultSubobject<URActionCancelSubscriberParameter>(TEXT("VelocityCommandSubscriberParameters"));
  }

  if (GetOwner())
  {
    ControllerComponent = Cast<URControllerComponent>(GetOwner()->GetPlugin(TEXT("ControllerComponent")));
  }
}