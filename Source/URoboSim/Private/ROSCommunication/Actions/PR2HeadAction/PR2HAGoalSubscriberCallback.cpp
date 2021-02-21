#include "ROSCommunication/Actions/PR2HeadAction/PR2HAGoalSubscriberCallback.h"
#include "Conversions.h"
#include "pr2_controllers_msgs/PR2PointHeadActionGoal.h"

FROSPointHeadGoalSubscriberCallback::FROSPointHeadGoalSubscriberCallback(
	const FString& InTopic, const FString& InType, UObject* InController) :
	FROSBridgeSubscriber(InTopic, InType)
{
  Controller = Cast<URPR2HeadTrajectoryController>(InController);
}

FROSPointHeadGoalSubscriberCallback::~FROSPointHeadGoalSubscriberCallback()
{
}

TSharedPtr<FROSBridgeMsg> FROSPointHeadGoalSubscriberCallback::ParseMessage
(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<pr2_controllers_msgs::PR2PointHeadActionGoal> Message =
    MakeShareable<pr2_controllers_msgs::PR2PointHeadActionGoal>(new pr2_controllers_msgs::PR2PointHeadActionGoal());

  Message->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(Message);
}

void FROSPointHeadGoalSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if(Controller)
    {
      TSharedPtr<pr2_controllers_msgs::PR2PointHeadActionGoal> Command = StaticCastSharedPtr<pr2_controllers_msgs::PR2PointHeadActionGoal>(Msg);
      Controller->Point = FConversions::ROSToU(Command->GetGoal().GetTarget().GetPoint().GetVector());
      Controller->FrameId = Command->GetGoal().GetTarget().GetHeader().GetFrameId();
      Controller->PointingFrame = Command->GetGoal().GetPointingFrame();
      Controller->Axis = Command->GetGoal().GetPointingAxis().GetVector();

      actionlib_msgs::GoalID Id = Command->GetGoalId();
      Controller->GoalStatusList.Add(FGoalStatusInfo(Id.GetId(), Id.GetStamp().Secs, Id.GetStamp().NSecs));
      UE_LOG(LogTemp, Log, TEXT("Recieved HeadAction Id: %s Goal: %s"), *Id.GetId(), *Controller->Point.ToString());
      AsyncTask(ENamedThreads::GameThread, [this]()
                                           {
                                             Controller->UpdateHeadDirection();
                                             Controller->bActive = true;
                                           }
        );
    }
}
