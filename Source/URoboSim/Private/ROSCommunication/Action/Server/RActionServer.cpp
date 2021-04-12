#include "ROSCommunication/Action/Server/RActionServer.h"

DEFINE_LOG_CATEGORY_STATIC(LogRActionServer, Log, All)

void URActionServer::Init(UObject *InOwner, const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InActionName)
{
  Handler = InHandler;
  if (Handler.IsValid())
  {
    Init(InOwner, InActionName);
  }
  else
  {
    UE_LOG(LogRActionServer, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URActionServer::Init(UObject *InOwner, const FString &WebsocketIPAddr, const uint32 &WebsocketPort, const FString &InActionName)
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  if (Handler.IsValid())
  {
    Handler->Connect();
    Init(InOwner, InActionName);
  }
  else
  {
    UE_LOG(LogRActionServer, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URActionServer::Init(UObject *InOwner, const FString &InActionName)
{
  if (!InActionName.Equals(TEXT("")))
  {
    ActionName = InActionName;
  }
  Init(InOwner);
}

void URActionServer::Init(UObject *&InOwner)
{
  ARModel *Owner = Cast<ARModel>(InOwner);
  if (Owner)
  {
    URController *Controller = Owner->GetController(ControllerName);

    Init();

    if (GoalSubscriber)
    {
      GoalSubscriber->SetController(Controller);
      GoalSubscriber->Init(InOwner, Handler, ActionName + TEXT("/goal"));
    }
    if (CancelSubscriber)
    {
      CancelSubscriber->SetController(Controller);
      CancelSubscriber->Init(InOwner, Handler, ActionName + TEXT("/cancel"));
    }
    if (StatusPublisher)
    {
      StatusPublisher->SetController(Controller);
      StatusPublisher->Init(InOwner, Handler, ActionName + TEXT("/status"));
    }
    if (FeedbackPublisher)
    {
      FeedbackPublisher->SetController(Controller);
      FeedbackPublisher->Init(InOwner, Handler, ActionName + TEXT("/feedback"));
    }
    if (ResultPublisher)
    {
      ResultPublisher->SetController(Controller);
      ResultPublisher->Init(InOwner, Handler, ActionName + TEXT("/result"));
    }
  }
}

void URActionServer::DeInit()
{
  if (Handler.IsValid())
  {
    Handler->Disconnect();
  }
}

void URActionServer::Tick()
{
  if (Handler.IsValid())
  {
    Handler->Process();
  }
  if (StatusPublisher)
  {
    StatusPublisher->Publish();
  }
  if (FeedbackPublisher)
  {
    FeedbackPublisher->Publish();
  }
  if (ResultPublisher)
  {
    ResultPublisher->Publish();
  }
}

void URActionServer::SetActionServerParameters(URActionServerParameter *&ActionServerParameters)
{
  if (ActionServerParameters)
  {
    ActionName = ActionServerParameters->ActionName;
  }
}