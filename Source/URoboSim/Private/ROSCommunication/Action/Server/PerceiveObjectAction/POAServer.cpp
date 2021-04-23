
#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAServer.h"
#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAResultPublisher.h"
#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAFeedbackPublisher.h"
#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAStatePublisher.h"
#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAGoalSubscriber.h"


URPerceiveObjectActionServer::URPerceiveObjectActionServer()
{
  ActionName = TEXT("/perceive_object");
  CancelSubscriber = CreateDefaultSubobject<URActionCancelSubscriber>(TEXT("GCACancelSubscriber"));
  StatusPublisher = CreateDefaultSubobject<URPerceiveObjectActionStatePublisher>(TEXT("POAStatusPublisher"));
  ResultPublisher = CreateDefaultSubobject<URPerceiveObjectActionResultPublisher>(TEXT("POAResultPublisher"));
  GoalSubscriber = CreateDefaultSubobject<URPerceiveObjectActionGoalSubscriber>(TEXT("POAGoalSubscriber"));
  FeedbackPublisher = CreateDefaultSubobject<URPerceiveObjectActionFeedbackPublisher>(TEXT("POAFeedbackPublisher"));
}
