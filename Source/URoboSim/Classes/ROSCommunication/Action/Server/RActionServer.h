#pragma once

#include "Controller/RControllerComponent.h"
#include "ROSBridgeHandler.h"
#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
// clang-format off
#include "RActionServer.generated.h"
// clang-format on

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URActionServerParameter : public UObject
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  FString ActionName;
};

UCLASS()
class UROBOSIM_API URActionSubscriber : public URSubscriber
{
  GENERATED_BODY()

public:
  void SetController(URController *&InController) { Controller = InController; }

protected:
  URController *Controller;
};

UCLASS() class UROBOSIM_API URActionPublisher : public URPublisher
{
  GENERATED_BODY()

public:
  void SetController(URController *&InController) { Controller = InController; }

protected:
  URController *Controller;
};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew) class UROBOSIM_API URActionServer : public UObject
{
  GENERATED_BODY()

public:
  URActionServer(){};

public:
  void Init(const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InActionName = "");

  void Init(const FString &WebsocketIPAddr, const uint32 &WebsocketPort, const FString &InActionName = "");

  void DeInit();

  void Tick();

  virtual void SetActionServerParameters(URActionServerParameter *&ActionServerParameters);

public:
  UPROPERTY(EditAnywhere)
  FString ControllerName;

protected:
  void Init(const FString &InActionName);

  virtual void Init();

protected:
  UPROPERTY(EditAnywhere)
  FString ActionName;

  UPROPERTY(VisibleAnywhere)
  URActionSubscriber *GoalSubscriber;

  UPROPERTY(VisibleAnywhere)
  URActionSubscriber *CancelSubscriber;

  UPROPERTY(VisibleAnywhere)
  URActionPublisher *StatusPublisher;

  UPROPERTY(VisibleAnywhere)
  URActionPublisher *ResultPublisher;

  UPROPERTY(VisibleAnywhere)
  URActionPublisher *FeedbackPublisher;

  TSharedPtr<FROSBridgeHandler> Handler;
};
