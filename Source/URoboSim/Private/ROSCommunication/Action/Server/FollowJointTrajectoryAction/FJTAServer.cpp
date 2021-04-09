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
  Cast<URFJTAFeedbackPublisherParameter>(Cast<URFJTAFeedbackPublisher>(FeedbackPublisher)->PublisherParameters)->FrameId = InFrameId;
  Cast<URFJTAResultPublisherParameter>(Cast<URFJTAResultPublisher>(ResultPublisher)->PublisherParameters)->FrameId = InFrameId;
}

void URFJTAServer::SetJointParamPath(const FString &InJointParamPath)
{
  Cast<URFJTAFeedbackPublisherParameter>(Cast<URFJTAFeedbackPublisher>(FeedbackPublisher)->PublisherParameters)->JointParamPath = InJointParamPath;
}