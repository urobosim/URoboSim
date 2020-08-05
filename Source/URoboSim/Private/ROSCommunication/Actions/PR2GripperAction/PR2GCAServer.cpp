
#include "ROSCommunication/Actions/PR2GripperAction/PR2GCAServer.h"
#include "ROSCommunication/Actions/PR2GripperAction/PR2GCAResultPublisher.h"
#include "ROSCommunication/Actions/PR2GripperAction/PR2GCAFeedbackPublisher.h"
#include "ROSCommunication/Actions/PR2GripperAction/PR2GCAStatePublisher.h"
#include "ROSCommunication/Actions/PR2GripperAction/PR2GCAGoalSubscriber.h"

URGripperCommandActionServer::URGripperCommandActionServer()
{
  ActionName = TEXT("/l_gripper_controller/gripper_action");
  CancelSubscriber = CreateDefaultSubobject<URActionCancelSubscriber>(TEXT("GCACancelSubscriber"));
  StatusPublisher = CreateDefaultSubobject<URPR2GripperCommandActionStatePublisher>(TEXT("GCAStatusPublisher"));
  ResultPublisher = CreateDefaultSubobject<URPR2GripperCommandActionResultPublisher>(TEXT("GCAResultPublisher"));
  GoalSubscriber = CreateDefaultSubobject<URPR2GripperCommandActionGoalSubscriber>(TEXT("GCAGoalSubscriber"));
  FeedbackPublisher = CreateDefaultSubobject<URPR2GripperCommandActionFeedbackPublisher>(TEXT("GCAFeedbackPublisher"));

}
