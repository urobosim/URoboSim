#include "ROSCommunication/Action/Server/RActionServer.h"

DEFINE_LOG_CATEGORY_STATIC(LogRActionServer, Log, All)

void URActionServer::Init(const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InActionName)
{
  Handler = InHandler;
  if (Handler.IsValid())
  {
    Init(InActionName);
  }
  else
  {
    UE_LOG(LogRActionServer, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URActionServer::Init(const FString &WebsocketIPAddr, const uint32 &WebsocketPort, const FString &InActionName)
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  if (Handler.IsValid())
  {
    Handler->Connect();
    Init(InActionName);
  }
  else
  {
    UE_LOG(LogRActionServer, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URActionServer::Init(const FString &InActionName)
{
  if (!InActionName.Equals(TEXT("")))
  {
    ActionName = InActionName;
  }
  Init();
}

void URActionServer::Init()
{
  ARModel *Owner = Cast<ARModel>(GetOuter());
  if (Owner)
  {
    URController *Controller = Owner->GetController(ControllerName);

    if (GoalSubscriber)
    {
      GoalSubscriber->SetController(Controller);
      GoalSubscriber->SetOwner(Owner);
      GoalSubscriber->Init(Handler, ActionName + TEXT("/goal"));
    }
    if (CancelSubscriber)
    {
      CancelSubscriber->SetController(Controller);
      CancelSubscriber->SetOwner(Owner);
      CancelSubscriber->Init(Handler, ActionName + TEXT("/cancel"));
    }
    if (StatusPublisher)
    {
      StatusPublisher->SetController(Controller);
      StatusPublisher->SetOwner(Owner);
      StatusPublisher->Init(Handler, ActionName + TEXT("/status"));
    }
    if (FeedbackPublisher)
    {
      FeedbackPublisher->SetController(Controller);
      FeedbackPublisher->SetOwner(Owner);
      FeedbackPublisher->Init(Handler, ActionName + TEXT("/feedback"));
    }
    if (ResultPublisher)
    {
      ResultPublisher->SetController(Controller);
      ResultPublisher->SetOwner(Owner);
      ResultPublisher->Init(Handler, ActionName + TEXT("/result"));
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