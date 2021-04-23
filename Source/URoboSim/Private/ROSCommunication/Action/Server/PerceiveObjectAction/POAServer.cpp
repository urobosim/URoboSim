
#include "ROSCommunication/Actions/PerceiveObjectAction/POAServer.h"
#include "ROSCommunication/Actions/PerceiveObjectAction/POAResultPublisher.h"
#include "ROSCommunication/Actions/PerceiveObjectAction/POAFeedbackPublisher.h"
#include "ROSCommunication/Actions/PerceiveObjectAction/POAStatePublisher.h"
#include "ROSCommunication/Actions/PerceiveObjectAction/POAGoalSubscriber.h"


URPerceiveObjectActionServer::URPerceiveObjectActionServer()
{
  ActionName = TEXT("/perceive_object");
  CancelSubscriber = CreateDefaultSubobject<URActionCancelSubscriber>(TEXT("GCACancelSubscriber"));
  StatusPublisher = CreateDefaultSubobject<URPerceiveObjectActionStatePublisher>(TEXT("POAStatusPublisher"));
  ResultPublisher = CreateDefaultSubobject<URPerceiveObjectActionResultPublisher>(TEXT("POAResultPublisher"));
  GoalSubscriber = CreateDefaultSubobject<URPerceiveObjectActionGoalSubscriber>(TEXT("POAGoalSubscriber"));
  FeedbackPublisher = CreateDefaultSubobject<URPerceiveObjectActionFeedbackPublisher>(TEXT("POAFeedbackPublisher"));
}
