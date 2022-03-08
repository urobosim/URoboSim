#include "ROSCommunication/Subscriber/JointStateReplaySubscriber.h"
#include "sensor_msgs/JointState.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJointStateReplaySubscriber, Log, All)

URJointStateReplaySubscriber::URJointStateReplaySubscriber()
{
  MessageType = TEXT("sensor_msgs/JointState");
  JointControllerName = TEXT("JointController");
}

void URJointStateReplaySubscriber::SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters)
{
  URJointStateReplaySubscriberParameter *JointStateReplaySubscriberParameters = Cast<URJointStateReplaySubscriberParameter>(SubscriberParameters);
  if (JointStateReplaySubscriberParameters)
  {
    Super::SetSubscriberParameters(SubscriberParameters);
    JointControllerName = JointStateReplaySubscriberParameters->JointControllerName;
  }
}

void URJointStateReplaySubscriber::CreateSubscriber()
{
  if (GetOwner())
  {
    Subscriber = MakeShareable<FRJointStateReplaySubscriberCallback>(
        new FRJointStateReplaySubscriberCallback(Topic, MessageType, GetOwner()->GetController(JointControllerName)));
  }
}

FRJointStateReplaySubscriberCallback::FRJointStateReplaySubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  JointController = Cast<URJointController>(InController);
}

TSharedPtr<FROSBridgeMsg> FRJointStateReplaySubscriberCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<sensor_msgs::JointState> JointStateMessage =
      MakeShareable<sensor_msgs::JointState>(new sensor_msgs::JointState());

  JointStateMessage->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(JointStateMessage);
}

void FRJointStateReplaySubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (JointController)
  {
    TSharedPtr<sensor_msgs::JointState> JointState = StaticCastSharedPtr<sensor_msgs::JointState>(Msg);

    TArray<FString> Names = JointState->GetName();
    TArray<double> Positions = JointState->GetPosition();
    JointController->SetJointNames(Names);
    for (int i = 0; i < Names.Num(); i++)
    {
      float &DesiredJointPosition = JointController->DesiredJointStates.FindOrAdd(Names[i]).JointPosition;
      DesiredJointPosition = Positions[i];
    }
  }
  else
  {
    UE_LOG(LogRJointStateReplaySubscriber, Error, TEXT("JointController not found"));
  }
}
