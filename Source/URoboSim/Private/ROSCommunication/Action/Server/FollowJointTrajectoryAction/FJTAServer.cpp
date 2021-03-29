#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAServer.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTACancelSubscriber.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAFeedbackPublisher.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAGoalSubscriber.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAResultPublisher.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAStatusPublisher.h"

URFJTAServer::URFJTAServer()
{
  ActionName = TEXT("/whole_body_controller/body/follow_joint_trajectory");
  CancelSubscriber = CreateDefaultSubobject<URFJTACancelSubscriber>(TEXT("FJTACancelSubscriber"));
  StatusPublisher = CreateDefaultSubobject<URFJTAStatusPublisher>(TEXT("FJTAStatusPublisher"));
  ResultPublisher = CreateDefaultSubobject<URFJTAResultPublisher>(TEXT("FJTAResultPublisher"));
  GoalSubscriber = CreateDefaultSubobject<URFJTAGoalSubscriber>(TEXT("FJTAGoalSubscriber"));
  FeedbackPublisher = CreateDefaultSubobject<URFJTAFeedbackPublisher>(TEXT("FJTAFeedbackPublisher"));
}

void URFJTAServer::SetFrameId(const FString &InFrameId)
{
  Cast<URFJTAFeedbackPublisher>(FeedbackPublisher)->FrameId = InFrameId;
  Cast<URFJTAResultPublisher>(ResultPublisher)->FrameId = InFrameId;
}

void URFJTAServer::SetJointParamPath(const FString &InJointParamPath)
{
  Cast<URFJTAFeedbackPublisher>(FeedbackPublisher)->JointParamPath = InJointParamPath;
}