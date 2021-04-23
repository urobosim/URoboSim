
#include "ROSCommunication/Actions/FollowTrajectoryAction/FTAServer.h"
#include "ROSCommunication/Actions/FollowTrajectoryAction/FTAResultPublisher.h"
#include "ROSCommunication/Actions/FollowTrajectoryAction/FTAFeedbackPublisher.h"
#include "ROSCommunication/Actions/FollowTrajectoryAction/FTAStatePublisher.h"
#include "ROSCommunication/Actions/FollowTrajectoryAction/FTAGoalSubscriber.h"

URFollowTrajectoryActionServer::URFollowTrajectoryActionServer()
{
  ActionName = TEXT("/whole_body_controller/body/follow_joint_trajectory");
  CancelSubscriber = CreateDefaultSubobject<URActionCancelSubscriber>(TEXT("FTACancelSubscriber"));
  StatusPublisher = CreateDefaultSubobject<URFollowJointTrajectoryActionStatePublisher>(TEXT("FTAStatusPublisher"));
  ResultPublisher = CreateDefaultSubobject<URFollowTrajectoryActionResultPublisher>(TEXT("FTAResultPublisher"));
  GoalSubscriber = CreateDefaultSubobject<URFollowJointTrajectoryActionGoalSubscriber>(TEXT("FTAGoalSubscriber"));
  FeedbackPublisher = CreateDefaultSubobject<URJointTrajectoryFeedbackPublisher>(TEXT("FTAFeedbackPublisher"));
}
