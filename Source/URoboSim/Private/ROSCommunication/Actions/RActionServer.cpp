#include "ROSCommunication/Actions/RActionServer.h"

void URActionServer::Init(TSharedPtr<FROSBridgeHandler> InHandler, UObject* InOwner, FString InActionName)
{
	Handler = InHandler;
	Owner = InOwner;
	if(!InActionName.Equals(""))
	{
		ActionName = InActionName;
	}
	Init();
}

void URActionServer::Init(FString InHostIp, uint32 InPort, UObject* InOwner, FString InActionName )
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(InHostIp, InPort));
  Handler->Connect();
  Owner = InOwner;
  if(!InActionName.Equals(""))
    {
      ActionName = InActionName;
    }
  Init();
}

void URActionServer::Tick()
{
  if(Handler.IsValid())
    {
      Handler->Process();
    }

  if(FeedbackPublisher)
    {
      FeedbackPublisher->Publish();
    }

  if(ResultPublisher)
    {
      ResultPublisher->Publish();
    }

  if(StatusPublisher)
    {
      StatusPublisher->Publish();
    }
}

void URActionServer::Init()
{
  CancelSubscriber->ControllerName = ControllerName;
  StatusPublisher->ControllerName = ControllerName;
  ResultPublisher->ControllerName = ControllerName;
  FeedbackPublisher->ControllerName = ControllerName;
  GoalSubscriber->ControllerName = ControllerName;

  CancelSubscriber->Init(Owner, Handler, ActionName + "/cancel");
  GoalSubscriber->Init(Owner, Handler, ActionName + "/goal");
  FeedbackPublisher->Init(Owner, Handler, ActionName + "/feedback");
  ResultPublisher->Init(Owner, Handler, ActionName + "/result");
  StatusPublisher->Init(Owner, Handler, ActionName + "/status");
}
