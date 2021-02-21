#include "ROSCommunication/Actions/PR2GripperAction/PR2GCASubscriberCallback.h"
#include "pr2_controllers_msgs/PR2GripperCommandActionGoal.h"

FROSGripperCommandSubscriberCallback::FROSGripperCommandSubscriberCallback(
	const FString& InTopic, const FString& InType, UObject* InController) :
	FROSBridgeSubscriber(InTopic, InType)
{
  Controller = Cast<URGripperController>(InController);
}

FROSGripperCommandSubscriberCallback::~FROSGripperCommandSubscriberCallback()
{
}

TSharedPtr<FROSBridgeMsg> FROSGripperCommandSubscriberCallback::ParseMessage
(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionGoal> Message =
    MakeShareable<pr2_controllers_msgs::PR2GripperCommandActionGoal>(new pr2_controllers_msgs::PR2GripperCommandActionGoal());

  Message->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(Message);
}

void FROSGripperCommandSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if(Controller)
    {
      TSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionGoal> Command = StaticCastSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionGoal>(Msg);

      actionlib_msgs::GoalID Id = Command->GetGoalId();

      AsyncTask(ENamedThreads::GameThread, [this, Id, Command]()
                                           {
                                             float Pos = Command->GetGoal().GetCommand().GetPosition() * 100.0;
                                             if(Pos > 8.5)
                                               {
                                                 UE_LOG(LogTemp, Warning, TEXT("%s: DesiredPosition should not exceed 8.5"), *Controller->GetName());
                                                 Pos = 8.5;
                                               }

                                             Controller->Position = Pos;

                                             Controller->MaxEffort = Command->GetGoal().GetCommand().GetMaxEffort();
                                             UE_LOG(LogTemp, Log, TEXT("Recieved GripperCommand Id: %s Goal: %f"), *Id.GetId(), Controller->Position);
                                             Controller->GoalStatusList.Add(FGoalStatusInfo(Id.GetId(), Id.GetStamp().Secs, Id.GetStamp().NSecs));
                                             Controller->UpdateGripper();
                                           }
        );
    }

}
