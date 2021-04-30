#include "ROSCommunication/Action/Server/PR2HeadAction/PR2HAServer.h"
#include "ROSCommunication/Action/Server/PR2HeadAction/PR2HACancelSubscriber.h"
#include "ROSCommunication/Action/Server/PR2HeadAction/PR2HAFeedbackPublisher.h"
#include "ROSCommunication/Action/Server/PR2HeadAction/PR2HAGoalSubscriber.h"
#include "ROSCommunication/Action/Server/PR2HeadAction/PR2HAResultPublisher.h"
#include "ROSCommunication/Action/Server/PR2HeadAction/PR2HAStatusPublisher.h"

URPR2HAServer::URPR2HAServer()
{
  ActionName = TEXT("/head_traj_controller/point_head_action");
  ControllerName = TEXT("HeadController");
}

void URPR2HAServer::SetActionServerParameters(URActionServerParameter *&ActionServerParameters)
{
  if (URPR2HAServerParameter *PR2HAServerParameter = Cast<URPR2HAServerParameter>(ActionServerParameters))
  {
    Super::SetActionServerParameters(ActionServerParameters);
    FrameId = PR2HAServerParameter->FrameId;
  }
}

void URPR2HAServer::CreateActionServer()
{
  CancelSubscriber = NewObject<URPR2HACancelSubscriber>(GetOwner());
  StatusPublisher = NewObject<URPR2HAStatusPublisher>(GetOwner());
  ResultPublisher = NewObject<URPR2HAResultPublisher>(GetOwner());
  GoalSubscriber = NewObject<URPR2HAGoalSubscriber>(GetOwner());
  FeedbackPublisher = NewObject<URPR2HAFeedbackPublisher>(GetOwner());

  Cast<URPR2HAFeedbackPublisher>(FeedbackPublisher)->FrameId = FrameId;
  Cast<URPR2HAResultPublisher>(ResultPublisher)->FrameId = FrameId;
  Cast<URPR2HAStatusPublisher>(StatusPublisher)->FrameId = FrameId;
}