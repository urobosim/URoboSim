#include "ROSCommunication/Actions/PR2GripperAction/PR2GripperReplaySubscriberCallback.h"
#include "pr2_controllers_msgs/PR2GripperCommandActionGoal.h"

FROSGripperReplaySubscriberCallback::FROSGripperReplaySubscriberCallback(
	const FString& InTopic, const FString& InType, UObject* InController) :
	FROSBridgeSubscriber(InTopic, InType)
{
  Controller = Cast<URGripperController>(InController);
}

FROSGripperReplaySubscriberCallback::~FROSGripperReplaySubscriberCallback()
{
}

TSharedPtr<FROSBridgeMsg> FROSGripperReplaySubscriberCallback::ParseMessage
(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionGoal> Message =
    MakeShareable<pr2_controllers_msgs::PR2GripperCommandActionGoal>(new pr2_controllers_msgs::PR2GripperCommandActionGoal());

  Message->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(Message);
}

void FROSGripperReplaySubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if(Controller)
    {
      TSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionGoal> Command = StaticCastSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionGoal>(Msg);

      actionlib_msgs::GoalID Id = Command->GetGoalId();

      AsyncTask(ENamedThreads::GameThread, [this, Id, Command]()
                                           {
                                             float Pos = Command->GetGoal().GetCommand().GetPosition() * 100.0;
                                             if(Pos > Position)
                                               {
                                                 UE_LOG(LogTemp, Log, TEXT("Release"), Controller->Position);
                                                 Controller->Release();
                                               }
                                             else
                                               {
                                                 UE_LOG(LogTemp, Log, TEXT("Grasp"));
                                                 Controller->Grasp();
                                               }

                                             Position = Pos;

                                           }
        );
    }

}
