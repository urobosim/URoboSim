#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAGoalSubscriberCallback.h"
#include "urobosim_msgs/PerceiveObjectActionGoal.h"

FROSPerceiveObjectGoalSubscriberCallback::FROSPerceiveObjectGoalSubscriberCallback(
	const FString& InTopic, const FString& InType, UObject* InController) :
	FROSBridgeSubscriber(InTopic, InType)
{
	Controller = Cast<URCameraController>(InController);
}

FROSPerceiveObjectGoalSubscriberCallback::~FROSPerceiveObjectGoalSubscriberCallback()
{
}

TSharedPtr<FROSBridgeMsg> FROSPerceiveObjectGoalSubscriberCallback::ParseMessage
(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<urobosim_msgs::PerceiveObjectActionGoal> Message =
    MakeShareable<urobosim_msgs::PerceiveObjectActionGoal>(new urobosim_msgs::PerceiveObjectActionGoal());

  Message->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(Message);
}

void FROSPerceiveObjectGoalSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if(Controller)
    {
      TSharedPtr<urobosim_msgs::PerceiveObjectActionGoal> Command = StaticCastSharedPtr<urobosim_msgs::PerceiveObjectActionGoal>(Msg);
      actionlib_msgs::GoalID Id = Command->GetGoalId();
      Controller->GoalStatusList.Add(FGoalStatusInfo(Id.GetId(), Id.GetStamp().Secs, Id.GetStamp().NSecs));
      Controller->TypeToPerceive = Command->GetGoal().GetType();
      Controller->bActive = true;
      AsyncTask(ENamedThreads::GameThread, [this]()
                                           {
                                             // // Controller->Camera->UpdateImage();
                                             Controller->PerceiveObject();
                                           }
        );

    }
}
