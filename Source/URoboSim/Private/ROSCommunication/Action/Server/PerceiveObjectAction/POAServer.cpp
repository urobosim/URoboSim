#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAServer.h"
#include "ROSCommunication/Action/Server/PerceiveObjectAction/POACancelSubscriber.h"
#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAFeedbackPublisher.h"
#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAGoalSubscriber.h"
#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAResultPublisher.h"
#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAStatusPublisher.h"

URPOAServer::URPOAServer()
{
  ActionName = TEXT("/perceive_object");
  ControllerName = TEXT("CameraController");
}

void URPOAServer::CreateActionServer()
{
  CancelSubscriber = NewObject<URPOACancelSubscriber>(GetOwner());
  StatusPublisher = NewObject<URPOAStatusPublisher>(GetOwner());
  ResultPublisher = NewObject<URPOAResultPublisher>(GetOwner());
  GoalSubscriber = NewObject<URPOAGoalSubscriber>(GetOwner());
  FeedbackPublisher = NewObject<URPOAFeedbackPublisher>(GetOwner());
}
