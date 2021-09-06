#include "ROSCommunication/Action/Server/RActionServer.h"

DEFINE_LOG_CATEGORY_STATIC(LogRActionServer, Log, All)

void URActionServer::SetActionServerParameters(URActionServerParameter *&ActionServerParameters)
{
  if (ActionServerParameters)
  {
    ActionName = ActionServerParameters->ActionName;
    ControllerName = ActionServerParameters->ControllerName;
  }
}

void URActionServer::Init()
{
  SetOwner(GetOuter());
  CreateActionServer();
  if (GetOwner())
  {
    URController *Controller = GetOwner()->GetController(ControllerName);
    if (!Controller)
    {
      UE_LOG(LogRActionServer, Error, TEXT("%s not found in %s"), *ControllerName, *GetName())
      return;
    }

    if (GoalSubscriber)
    {
      UE_LOG(LogRActionServer, Log, TEXT("Initialize %s of %s"), *GoalSubscriber->GetName(), *GetName())
      GoalSubscriber->SetController(Controller);
      GoalSubscriber->Topic = ActionName + TEXT("/goal");
      GoalSubscriber->Connect(Handler);
    }
    if (CancelSubscriber)
    {
      UE_LOG(LogRActionServer, Log, TEXT("Initialize %s of %s"), *CancelSubscriber->GetName(), *GetName())
      CancelSubscriber->SetController(Controller);
      CancelSubscriber->Topic = ActionName + TEXT("/cancel");
      CancelSubscriber->Connect(Handler);
    }
    if (StatusPublisher)
    {
      UE_LOG(LogRActionServer, Log, TEXT("Initialize %s of %s"), *StatusPublisher->GetName(), *GetName())
      StatusPublisher->SetController(Controller);
      StatusPublisher->Topic = ActionName + TEXT("/status");
      StatusPublisher->Connect(Handler);
    }
    if (FeedbackPublisher)
    {
      UE_LOG(LogRActionServer, Log, TEXT("Initialize %s of %s"), *FeedbackPublisher->GetName(), *GetName())
      FeedbackPublisher->SetController(Controller);
      FeedbackPublisher->Topic = ActionName + TEXT("/feedback");
      FeedbackPublisher->Connect(Handler);
    }
    if (ResultPublisher)
    {
      UE_LOG(LogRActionServer, Log, TEXT("Initialize %s of %s"), *ResultPublisher->GetName(), *GetName())
      ResultPublisher->SetController(Controller);
      ResultPublisher->Topic = ActionName + TEXT("/result");
      ResultPublisher->Connect(Handler);
    }
  }
  else
  {
    UE_LOG(LogRActionServer, Error, TEXT("Owner of %s not found, Outer is %s"), *GetName(), *GetOuter()->GetName())
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
