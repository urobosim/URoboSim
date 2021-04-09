#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAServer.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTACancelSubscriber.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAFeedbackPublisher.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAGoalSubscriber.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAResultPublisher.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAStatusPublisher.h"

URFJTAServer::URFJTAServer()
{
  CancelSubscriber = CreateDefaultSubobject<URFJTACancelSubscriber>(TEXT("FJTACancelSubscriber"));
  StatusPublisher = CreateDefaultSubobject<URFJTAStatusPublisher>(TEXT("FJTAStatusPublisher"));
  ResultPublisher = CreateDefaultSubobject<URFJTAResultPublisher>(TEXT("FJTAResultPublisher"));
  GoalSubscriber = CreateDefaultSubobject<URFJTAGoalSubscriber>(TEXT("FJTAGoalSubscriber"));
  FeedbackPublisher = CreateDefaultSubobject<URFJTAFeedbackPublisher>(TEXT("FJTAFeedbackPublisher"));
}

void URFJTAServer::Init()
{
  if (!Cast<URFJTAServerParameter>(ActionServerParameters))
  {
    ActionServerParameters = CreateDefaultSubobject<URFJTAServerParameter>(TEXT("FJTAServerParameters"));
  }
  
  URFJTAServerParameter *FJTAServerParameter = Cast<URFJTAServerParameter>(ActionServerParameters);
  Cast<URFJTAFeedbackPublisherParameter>(Cast<URFJTAFeedbackPublisher>(FeedbackPublisher)->PublisherParameters)->FrameId = FJTAServerParameter->FrameId;
  Cast<URFJTAResultPublisherParameter>(Cast<URFJTAResultPublisher>(ResultPublisher)->PublisherParameters)->FrameId = FJTAServerParameter->FrameId;

  Cast<URFJTAFeedbackPublisherParameter>(Cast<URFJTAFeedbackPublisher>(FeedbackPublisher)->PublisherParameters)->JointParamPath = FJTAServerParameter->JointParamPath;
}