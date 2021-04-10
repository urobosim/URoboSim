#include "ROSCommunication/Action/Server/RActionStatusPublisher.h"
#include "actionlib_msgs/GoalStatusArray.h"

DEFINE_LOG_CATEGORY_STATIC(LogRActionStatusPublisher, Log, All)

URActionStatusPublisher::URActionStatusPublisher()
{
  MessageType = TEXT("actionlib_msgs/GoalStatusArray");
  FrameId = TEXT("odom");
}

void URActionStatusPublisher::Init()
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
      UE_LOG(LogRActionStatusPublisher, Error, TEXT("%s not found in %s"), *ControllerName, *GetName())
    }
  }
}

void URActionStatusPublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  URActionStatusPublisherParameter *ActionStatusPublisherParameters = Cast<URActionStatusPublisherParameter>(PublisherParameters);
  if (ActionStatusPublisherParameters)
  {
    Super::SetPublishParameters(PublisherParameters);
    MessageType = ActionStatusPublisherParameters->MessageType;
    FrameId = ActionStatusPublisherParameters->FrameId;
    ControllerName = ActionStatusPublisherParameters->ControllerName;
  }
}

void URActionStatusPublisher::Publish()
{
  if (Controller)
  {
    static int Seq = 0;
    TSharedPtr<actionlib_msgs::GoalStatusArray> GoalStatusArrayMsg =
        MakeShareable(new actionlib_msgs::GoalStatusArray());

    GoalStatusArrayMsg->SetHeader(std_msgs::Header(Seq++, FROSTime(), FrameId));

    TArray<actionlib_msgs::GoalStatus> GoalStatusArray;
    for (const FGoalStatusInfo &GoalStatusInfo : Controller->GetGoalStatusList())
    {
      actionlib_msgs::GoalStatus GoalStatus(actionlib_msgs::GoalID(FROSTime(GoalStatusInfo.Secs, GoalStatusInfo.NSecs), GoalStatusInfo.Id), GoalStatusInfo.Status, TEXT(""));
      GoalStatusArray.Add(GoalStatus);
    }
    GoalStatusArrayMsg->SetStatusList(GoalStatusArray);

    Handler->PublishMsg(Topic, GoalStatusArrayMsg);
    Handler->Process();
  }
  else
  {
    UE_LOG(LogRActionStatusPublisher, Error, TEXT("Controller not found in %s"), *GetName())
  }
}
