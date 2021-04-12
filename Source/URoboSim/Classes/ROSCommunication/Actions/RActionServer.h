#pragma once

#include "ROSBridgeHandler.h"
#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "RActionServerParameter.h"
#include "RActionServer.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URActionServer : public UObject
{
  GENERATED_BODY()
public:

  URActionServer(){};

  virtual void Init(TSharedPtr<FROSBridgeHandler> InHandler, UObject* InOwner, FString InActionName= "");
  virtual void Init(FString InHostIp, uint32 InPort, UObject* InOwner, FString InActionName= "");

  virtual void SetActionServerParameters(URActionServerParameter *&ActionServerParameters);

  virtual void Tick();

  UPROPERTY(EditAnywhere)
    FString ControllerName;
protected:

  UPROPERTY(EditAnywhere)
    URPublisher* FeedbackPublisher;

  UPROPERTY(EditAnywhere)
    URPublisher* StatusPublisher;

  UPROPERTY(EditAnywhere)
    URPublisher* ResultPublisher;

  UPROPERTY(EditAnywhere)
    URSubscriber* GoalSubscriber;

  UPROPERTY(EditAnywhere)
    URSubscriber* CancelSubscriber;

  TSharedPtr<FROSBridgeHandler> Handler;

  virtual void Init();

  UPROPERTY()
    UObject* Owner;

  UPROPERTY(EditAnywhere)
    FString ActionName;
};
