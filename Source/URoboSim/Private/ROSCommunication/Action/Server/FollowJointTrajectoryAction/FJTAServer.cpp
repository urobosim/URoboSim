#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAServer.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTACancelSubscriber.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAFeedbackPublisher.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAGoalSubscriber.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAResultPublisher.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAStatusPublisher.h"

URFJTAServer::URFJTAServer()
{
  ActionName = TEXT("/whole_body_controller/body/follow_joint_trajectory");
  FrameId = TEXT("odom");
  JointParamPath = TEXT("whole_body_controller/body/joints");
  JointControllerName = TEXT("JointController");
  CancelSubscriber = CreateDefaultSubobject<URFJTACancelSubscriber>(TEXT("FJTACancelSubscriber"));
  StatusPublisher = CreateDefaultSubobject<URFJTAStatusPublisher>(TEXT("FJTAStatusPublisher"));
  ResultPublisher = CreateDefaultSubobject<URFJTAResultPublisher>(TEXT("FJTAResultPublisher"));
  GoalSubscriber = CreateDefaultSubobject<URFJTAGoalSubscriber>(TEXT("FJTAGoalSubscriber"));
  FeedbackPublisher = CreateDefaultSubobject<URFJTAFeedbackPublisher>(TEXT("FJTAFeedbackPublisher"));
}

void URFJTAServer::Init()
{
  Cast<URFJTAFeedbackPublisher>(FeedbackPublisher)->FrameId = FrameId;
  Cast<URFJTAResultPublisher>(ResultPublisher)->FrameId = FrameId;
  Cast<URFJTAStatusPublisher>(StatusPublisher)->FrameId = FrameId;

  Cast<URFJTAFeedbackPublisher>(FeedbackPublisher)->JointParamPath = JointParamPath;

  Cast<URFJTAFeedbackPublisher>(FeedbackPublisher)->JointControllerName = JointControllerName;
  Cast<URFJTAResultPublisher>(ResultPublisher)->JointControllerName = JointControllerName;
  Cast<URFJTAGoalSubscriber>(GoalSubscriber)->JointControllerName = JointControllerName;
  Cast<URFJTACancelSubscriber>(CancelSubscriber)->ControllerName = JointControllerName;
  Cast<URFJTAStatusPublisher>(StatusPublisher)->ControllerName = JointControllerName;
}

void URFJTAServer::SetActionServerParameters(URActionServerParameter *&ActionServerParameters)
{
  URFJTAServerParameter *FJTAServerParameter = Cast<URFJTAServerParameter>(ActionServerParameters);
  if (FJTAServerParameter)
  {
    Super::SetActionServerParameters(ActionServerParameters);
    FrameId = FJTAServerParameter->FrameId;
    JointParamPath = FJTAServerParameter->JointParamPath;
    JointControllerName = FJTAServerParameter->JointControllerName;
  }
}