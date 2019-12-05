#include "ROSCommunication/RActionServer.h"

void URActionServer::Init(TSharedPtr<FROSBridgeHandler> InHandler, UObject* InOwner, FString InActionName)
{
	Handler = InHandler;
	Owner = InOwner;
	if(!InActionName.Equals(""))
	{
		ActionName = InActionName;
	}
	Init();
}

void URActionServer::Init(FString InHostIp, uint32 InPort, UObject* InOwner, FString InActionName )
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(InHostIp, InPort));
  Handler->Connect();
  Owner = InOwner;
  if(!InActionName.Equals(""))
    {
      ActionName = InActionName;
    }
  Init();
}

void URActionServer::Tick()
{
  if(Handler.IsValid())
    {
      Handler->Process();
    }

  if(FeedbackPublisher)
    {
      FeedbackPublisher->Publish();
    }

  if(ResultPublisher)
    {
      ResultPublisher->Publish();
    }

  if(StatusPublisher)
    {
      StatusPublisher->Publish();
    }
}

void URActionServer::Init()
{
  CancelSubscriber->ControllerName = ControllerName;
  StatusPublisher->ControllerName = ControllerName;
  ResultPublisher->ControllerName = ControllerName;
  FeedbackPublisher->ControllerName = ControllerName;
  GoalSubscriber->ControllerName = ControllerName;

  CancelSubscriber->Init(Owner, Handler, ActionName + "/cancel");
  GoalSubscriber->Init(Owner, Handler, ActionName + "/goal");
  FeedbackPublisher->Init(Owner, Handler, ActionName + "/feedback");
  ResultPublisher->Init(Owner, Handler, ActionName + "/result");
  StatusPublisher->Init(Owner, Handler, ActionName + "/status");
}


URFollowTrajectoryActionServer::URFollowTrajectoryActionServer()
{
  ActionName = TEXT("/whole_body_controller/body/follow_joint_trajectory");
  CancelSubscriber = CreateDefaultSubobject<URActionCancelSubscriber>(TEXT("FTACancelSubscriber"));
  StatusPublisher = CreateDefaultSubobject<URFollowJointTrajectoryActionStatePublisher>(TEXT("FTAStatusPublisher"));
  ResultPublisher = CreateDefaultSubobject<URFollowTrajectoryActionResultPublisher>(TEXT("FTAResultPublisher"));
  GoalSubscriber = CreateDefaultSubobject<URFollowJointTrajectoryActionGoalSubscriber>(TEXT("FTAGoalSubscriber"));
  FeedbackPublisher = CreateDefaultSubobject<URJointTrajectoryFeedbackPublisher>(TEXT("FTAFeedbackPublisher"));
}

URPointHeadActionServer::URPointHeadActionServer()
{
  ActionName = TEXT("/head_traj_controller/point_head_action");
  CancelSubscriber = CreateDefaultSubobject<URActionCancelSubscriber>(TEXT("PHACancelSubscriber"));
  StatusPublisher = CreateDefaultSubobject<URPR2HeadActionStatePublisher>(TEXT("PHAStatusPublisher"));
  ResultPublisher = CreateDefaultSubobject<URPR2HeadActionResultPublisher>(TEXT("PHAResultPublisher"));
  GoalSubscriber = CreateDefaultSubobject<URPR2HeadActionGoalSubscriber>(TEXT("PHAGoalSubscriber"));
  FeedbackPublisher = CreateDefaultSubobject<URPR2HeadActionFeedbackPublisher>(TEXT("PHAFeedbackPublisher"));
}

URGripperCommandActionServer::URGripperCommandActionServer()
{
  ActionName = TEXT("/l_gripper_controller/gripper_action");
  CancelSubscriber = CreateDefaultSubobject<URActionCancelSubscriber>(TEXT("GCACancelSubscriber"));
  StatusPublisher = CreateDefaultSubobject<URPR2GripperCommandActionStatePublisher>(TEXT("GCAStatusPublisher"));
  ResultPublisher = CreateDefaultSubobject<URPR2GripperCommandActionResultPublisher>(TEXT("GCAResultPublisher"));
  GoalSubscriber = CreateDefaultSubobject<URPR2GripperCommandActionGoalSubscriber>(TEXT("GCAGoalSubscriber"));
  FeedbackPublisher = CreateDefaultSubobject<URPR2GripperCommandActionFeedbackPublisher>(TEXT("GCAFeedbackPublisher"));

}

URPerceiveObjectActionServer::URPerceiveObjectActionServer()
{
  ActionName = TEXT("/perceive_object");
  CancelSubscriber = CreateDefaultSubobject<URActionCancelSubscriber>(TEXT("GCACancelSubscriber"));
  StatusPublisher = CreateDefaultSubobject<URPerceiveObjectActionStatePublisher>(TEXT("POAStatusPublisher"));
  ResultPublisher = CreateDefaultSubobject<URPerceiveObjectActionResultPublisher>(TEXT("POAResultPublisher"));
  GoalSubscriber = CreateDefaultSubobject<URPerceiveObjectActionGoalSubscriber>(TEXT("POAGoalSubscriber"));
  FeedbackPublisher = CreateDefaultSubobject<URPerceiveObjectActionFeedbackPublisher>(TEXT("POAFeedbackPublisher"));
}
