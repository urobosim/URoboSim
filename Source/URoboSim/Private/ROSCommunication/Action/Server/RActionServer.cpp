#include "ROSCommunication/Action/Server/RActionServer.h"
#include "ROSCommunication/RROSCommunicationComponent.h"

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
    UE_LOG(LogRActionServer, Log, TEXT("%s is connected to ROSBridge"), *GetName())
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
  ARModel *Owner = nullptr;
  if (Cast<ARModel>(GetOuter()))
  {
    Owner = Cast<ARModel>(GetOuter());
  }
  else if (Cast<URROSCommunicationComponent>(GetOuter()) && Cast<ARModel>(Cast<URROSCommunicationComponent>(GetOuter())->GetOwner()))
  {
    Owner = Cast<ARModel>(Cast<URROSCommunicationComponent>(GetOuter())->GetOwner());
  }
  if (Owner)
  {
    URController *Controller = Owner->GetController(ControllerName);
    
    if (GoalSubscriber)
    {
      UE_LOG(LogRActionServer, Log, TEXT("Initialize %s of %s"), *GoalSubscriber->GetName(), *GetName())
      GoalSubscriber->SetController(Controller);
      GoalSubscriber->SetOwner(Owner);
      GoalSubscriber->Init(Handler, ActionName + TEXT("/goal"));
    }
    if (CancelSubscriber)
    {
      UE_LOG(LogRActionServer, Log, TEXT("Initialize %s of %s"), *CancelSubscriber->GetName(), *GetName())
      CancelSubscriber->SetController(Controller);
      CancelSubscriber->SetOwner(Owner);
      CancelSubscriber->Init(Handler, ActionName + TEXT("/cancel"));
    }
    if (StatusPublisher)
    {
      UE_LOG(LogRActionServer, Log, TEXT("Initialize %s of %s"), *StatusPublisher->GetName(), *GetName())
      StatusPublisher->SetController(Controller);
      StatusPublisher->SetOwner(Owner);
      StatusPublisher->Init(Handler, ActionName + TEXT("/status"));
    }
    if (FeedbackPublisher)
    {
      UE_LOG(LogRActionServer, Log, TEXT("Initialize %s of %s"), *FeedbackPublisher->GetName(), *GetName())
      FeedbackPublisher->SetController(Controller);
      FeedbackPublisher->SetOwner(Owner);
      FeedbackPublisher->Init(Handler, ActionName + TEXT("/feedback"));
    }
    if (ResultPublisher)
    {
      UE_LOG(LogRActionServer, Log, TEXT("Initialize %s of %s"), *ResultPublisher->GetName(), *GetName())
      ResultPublisher->SetController(Controller);
      ResultPublisher->SetOwner(Owner);
      ResultPublisher->Init(Handler, ActionName + TEXT("/result"));
    }
  }
  else
  {
    UE_LOG(LogRActionServer, Error, TEXT("Owner of %s not found, Outer is %s"), *GetName(), *GetOuter()->GetName())
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