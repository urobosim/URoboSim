#pragma once

#include "ROSBridgeHandler.h"
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

public:
  UPROPERTY(EditAnywhere)
  FString ActionName;

protected:
  void Init(UObject *InOwner, const FString &InActionName);
  
  void Init(UObject *&InOwner);

protected:
  UPROPERTY(EditAnywhere)
  URSubscriber *GoalSubscriber;

  UPROPERTY(EditAnywhere)
  URSubscriber *CancelSubscriber;

  UPROPERTY(EditAnywhere)
  URPublisher *StatusPublisher;

  UPROPERTY(EditAnywhere)
  URPublisher *ResultPublisher;

  UPROPERTY(EditAnywhere)
  URPublisher *FeedbackPublisher;

  TSharedPtr<FROSBridgeHandler> Handler;
};
