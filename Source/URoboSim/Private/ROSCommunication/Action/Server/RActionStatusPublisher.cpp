#include "ROSCommunication/Action/Server/RActionStatusPublisher.h"
#include "actionlib_msgs/GoalStatusArray.h"

DEFINE_LOG_CATEGORY_STATIC(LogRActionStatusPublisher, Log, All)

void URActionStatusPublisher::Publish()
{
  if (Controller)
  {
    static int Seq = 0;
    const URActionStatusPublisherParameter *ActionStatusPublisherParameters = GetActionStatusPublisherParameters();
    if (ActionStatusPublisherParameters)
    {
      TSharedPtr<actionlib_msgs::GoalStatusArray> GoalStatusArrayMsg =
          MakeShareable(new actionlib_msgs::GoalStatusArray());

      GoalStatusArrayMsg->SetHeader(std_msgs::Header(Seq++, FROSTime(), ActionStatusPublisherParameters->FrameId));

      TArray<actionlib_msgs::GoalStatus> GoalStatusArray;
      for (const FGoalStatusInfo &GoalStatusInfo : Controller->GetGoalStatusList())
      {
        actionlib_msgs::GoalStatus GoalStatus(actionlib_msgs::GoalID(FROSTime(GoalStatusInfo.Secs, GoalStatusInfo.NSecs), GoalStatusInfo.Id), GoalStatusInfo.Status, TEXT(""));
        GoalStatusArray.Add(GoalStatus);
      }
      GoalStatusArrayMsg->SetStatusList(GoalStatusArray);

      Handler->PublishMsg(ActionStatusPublisherParameters->Topic, GoalStatusArrayMsg);
      Handler->Process();
    }
    else
    {
      UE_LOG(LogRActionStatusPublisher, Error, TEXT("Controller not found in %s"), *GetName())
    }
  }
}
