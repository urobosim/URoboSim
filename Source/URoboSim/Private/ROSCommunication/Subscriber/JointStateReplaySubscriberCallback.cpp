#include "ROSCommunication/Subscriber/JointStateReplaySubscriberCallback.h"
#include "sensor_msgs/JointState.h"

FROSJointStateReplaySubscriberCallback::FROSJointStateReplaySubscriberCallback(
                                                                               const FString& InTopic, const FString& InType, UObject* InController) :
  FROSBridgeSubscriber(InTopic, InType)
{
  JointController = Cast<URJointController>(InController);
}

TSharedPtr<FROSBridgeMsg> FROSJointStateReplaySubscriberCallback::ParseMessage
(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<sensor_msgs::JointState> JointStateMessage =
        MakeShareable<sensor_msgs::JointState>(new sensor_msgs::JointState());

  JointStateMessage->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(JointStateMessage);
}

void FROSJointStateReplaySubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if(JointController)
    {
      TSharedPtr<sensor_msgs::JointState> JointState = StaticCastSharedPtr<sensor_msgs::JointState>(Msg);

      TArray<FString> Names = JointState->GetName();
      TArray<double>  Positions = JointState->GetPosition();
      for(int i = 0; i < Names.Num(); i++)
        {
          float& DesiredJointPosition = JointController->DesiredJointState.FindOrAdd(Names[i]);
          DesiredJointPosition = Positions[i];
        }
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("JonintController not found"));
    }
}
