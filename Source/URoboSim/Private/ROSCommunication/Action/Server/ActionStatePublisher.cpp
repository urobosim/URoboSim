
#include "ROSCommunication/Action/Server/ActionStatePublisher.h"
#include "actionlib_msgs/GoalStatusArray.h"

void URActionStatePublisher::SetMessageType()
{
  MessageType = TEXT("actionlib_msgs/GoalStatusArray");
}

void URActionStatePublisher::Publish()
{
  TSharedPtr<actionlib_msgs::GoalStatusArray> GoalStatusArrayMsg =
    MakeShareable(new actionlib_msgs::GoalStatusArray());

  GoalStatusArrayMsg->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

  TArray<actionlib_msgs::GoalStatus> GSArray;

  for(auto& StatusInfo : Owner->GoalStatusList)
    {
      actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, "");
      GSArray.Add(GS);
    }
  GoalStatusArrayMsg->SetStatusList(GSArray);

  Handler->PublishMsg(Topic, GoalStatusArrayMsg);
  Handler->Process();

  Seq++ ;
  // UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
}
