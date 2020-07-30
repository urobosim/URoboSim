
#include "ROSCommunication/Actions/PR2HeadAction/PR2HAServer.h"
#include "ROSCommunication/Actions/PR2HeadAction/PR2HAResultPublisher.h"
#include "ROSCommunication/Actions/PR2HeadAction/PR2HAFeedbackPublisher.h"
#include "ROSCommunication/Actions/PR2HeadAction/PR2HAStatePublisher.h"

URPointHeadActionServer::URPointHeadActionServer()
{
  ActionName = TEXT("/head_traj_controller/point_head_action");
  CancelSubscriber = CreateDefaultSubobject<URActionCancelSubscriber>(TEXT("PHACancelSubscriber"));
  StatusPublisher = CreateDefaultSubobject<URPR2HeadActionStatePublisher>(TEXT("PHAStatusPublisher"));
  ResultPublisher = CreateDefaultSubobject<URPR2HeadActionResultPublisher>(TEXT("PHAResultPublisher"));
  GoalSubscriber = CreateDefaultSubobject<URPR2HeadActionGoalSubscriber>(TEXT("PHAGoalSubscriber"));
  FeedbackPublisher = CreateDefaultSubobject<URPR2HeadActionFeedbackPublisher>(TEXT("PHAFeedbackPublisher"));
}
