
#include "ROSCommunication/Action/Server/FollowTrajectoryAction/FTAServer.h"
#include "ROSCommunication/Action/Server/FollowTrajectoryAction/FTAResultPublisher.h"
#include "ROSCommunication/Action/Server/FollowTrajectoryAction/FTAFeedbackPublisher.h"
#include "ROSCommunication/Action/Server/FollowTrajectoryAction/FTAStatePublisher.h"
#include "ROSCommunication/Action/Server/FollowTrajectoryAction/FTAGoalSubscriber.h"

URFollowTrajectoryActionServer::URFollowTrajectoryActionServer()
{
  ActionName = TEXT("/whole_body_controller/body/follow_joint_trajectory");
  CancelSubscriber = CreateDefaultSubobject<URActionCancelSubscriber>(TEXT("FTACancelSubscriber"));
  StatusPublisher = CreateDefaultSubobject<URFollowJointTrajectoryActionStatePublisher>(TEXT("FTAStatusPublisher"));
  ResultPublisher = CreateDefaultSubobject<URFollowTrajectoryActionResultPublisher>(TEXT("FTAResultPublisher"));
  GoalSubscriber = CreateDefaultSubobject<URFollowJointTrajectoryActionGoalSubscriber>(TEXT("FTAGoalSubscriber"));
  FeedbackPublisher = CreateDefaultSubobject<URJointTrajectoryFeedbackPublisher>(TEXT("FTAFeedbackPublisher"));
}
