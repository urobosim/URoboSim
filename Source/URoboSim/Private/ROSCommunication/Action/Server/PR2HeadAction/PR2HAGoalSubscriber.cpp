#include "ROSCommunication/Action/Server/PR2HeadAction/PR2HAGoalSubscriber.h"
#include "pr2_controllers_msgs/PR2PointHeadActionGoal.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPR2HAGoalSubscriber, Log, All)

URPR2HAGoalSubscriber::URPR2HAGoalSubscriber()
{
  MessageType = TEXT("pr2_controllers_msgs/PointHeadActionGoal");
}

void URPR2HAGoalSubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FRPR2HAGoalSubscriberCallback>(
      new FRPR2HAGoalSubscriberCallback(Topic, MessageType, Controller));
  if (Subscriber.IsValid())
  {
    UE_LOG(LogRPR2HAGoalSubscriber, Log, TEXT("Subscriber connected to RosBridge"));
  }
}

FRPR2HAGoalSubscriberCallback::FRPR2HAGoalSubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  HeadController = Cast<URPR2HeadTrajectoryController>(InController);
  if(!HeadController)
    {
      UE_LOG(LogRPR2HAGoalSubscriber, Error, TEXT("HeadController not found"));
    }
}

TSharedPtr<FROSBridgeMsg> FRPR2HAGoalSubscriberCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<pr2_controllers_msgs::PR2PointHeadActionGoal> Message =
      MakeShareable<pr2_controllers_msgs::PR2PointHeadActionGoal>(new pr2_controllers_msgs::PR2PointHeadActionGoal());

  Message->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(Message);
}

void FRPR2HAGoalSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (HeadController)
  {
    TSharedPtr<pr2_controllers_msgs::PR2PointHeadActionGoal> Command = StaticCastSharedPtr<pr2_controllers_msgs::PR2PointHeadActionGoal>(Msg);
    HeadController->Point = FConversions::ROSToU(Command->GetGoal().GetTarget().GetPoint().GetVector());
    HeadController->FrameId = Command->GetGoal().GetTarget().GetHeader().GetFrameId();
    HeadController->PointingFrame = Command->GetGoal().GetPointingFrame();
    HeadController->Axis = Command->GetGoal().GetPointingAxis().GetVector();

    actionlib_msgs::GoalID Id = Command->GetGoalId();
    HeadController->AddGoalStatus(FGoalStatusInfo(Id.GetId(), Id.GetStamp().Secs, Id.GetStamp().NSecs));
    UE_LOG(LogRPR2HAGoalSubscriber, Log, TEXT("Recieved HeadAction Id: %s Goal: %s"), *Id.GetId(), *HeadController->Point.ToString());
    AsyncTask(ENamedThreads::GameThread, [this]() {
      HeadController->UpdateHeadDirection();
      HeadController->bActive = true;
    });
  }
}
