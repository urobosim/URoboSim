#pragma once

#include "ROSBridgeHandler.h"
#include "RActionServerParameter.h"
#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
// clang-format off
#include "RActionServer.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URActionServer : public UObject
{
  GENERATED_BODY()

public:
  URActionServer(){};

public:
  void Init(UObject *InOwner, const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InActionName = "");

  void Init(UObject *InOwner, const FString &WebsocketIPAddr, const uint32 &WebsocketPort, const FString &InActionName = "");

  void DeInit() 
  { 
    if (Handler.IsValid())
    {
      Handler->Disconnect(); 
    }
  }

  void Tick();

  virtual void SetActionServerParameters(URActionServerParameter *&ActionServerParameters);

protected:
  void Init(UObject *InOwner, const FString &InActionName);
  
  void Init(UObject *&InOwner);

  virtual void Init() {}

protected:
  UPROPERTY(EditAnywhere)
  FString ActionName;

  UPROPERTY(VisibleAnywhere)
  URSubscriber *GoalSubscriber;

  UPROPERTY(VisibleAnywhere)
  URSubscriber *CancelSubscriber;

  UPROPERTY(VisibleAnywhere)
  URPublisher *StatusPublisher;

  UPROPERTY(VisibleAnywhere)
  URPublisher *ResultPublisher;

  UPROPERTY(VisibleAnywhere)
  URPublisher *FeedbackPublisher;

  TSharedPtr<FROSBridgeHandler> Handler;
};
