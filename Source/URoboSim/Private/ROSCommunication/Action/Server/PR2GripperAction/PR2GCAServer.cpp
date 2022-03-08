#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GCAServer.h"
#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GCACancelSubscriber.h"
#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GCAFeedbackPublisher.h"
#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GCAGoalSubscriber.h"
#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GCAResultPublisher.h"
#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GCAStatusPublisher.h"

URPR2GCAServer::URPR2GCAServer()
{
  ActionName = TEXT("/l_gripper_controller/gripper_action");
  ControllerName = TEXT("GripperController");
}

void URPR2GCAServer::SetActionServerParameters(URActionServerParameter *&ActionServerParameters)
{
  if (URPR2GCAServerParameter *PR2GCAServerParameter = Cast<URPR2GCAServerParameter>(ActionServerParameters))
  {
    Super::SetActionServerParameters(ActionServerParameters);
    FrameId = PR2GCAServerParameter->FrameId;
  }
}

void URPR2GCAServer::CreateActionServer()
{
  CancelSubscriber = NewObject<URPR2GCACancelSubscriber>(GetOwner());
  StatusPublisher = NewObject<URPR2GCAStatusPublisher>(GetOwner());
  ResultPublisher = NewObject<URPR2GCAResultPublisher>(GetOwner());
  GoalSubscriber = NewObject<URPR2GCAGoalSubscriber>(GetOwner());
  FeedbackPublisher = NewObject<URPR2GCAFeedbackPublisher>(GetOwner());

  Cast<URPR2GCAFeedbackPublisher>(FeedbackPublisher)->FrameId = FrameId;
  Cast<URPR2GCAResultPublisher>(ResultPublisher)->FrameId = FrameId;
  Cast<URPR2GCAStatusPublisher>(StatusPublisher)->FrameId = FrameId;
}