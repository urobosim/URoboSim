// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "ROSCommunication/RROSCommunication.h"
#include "RPluginComponent.h"
// clang-format off
#include "RROSCommunicationComponent.generated.h"
// clang-format on

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URROSCommunicationComponent final : public URPluginComponent
{
  GENERATED_BODY()

public:
  URROSCommunicationComponent(const FString &InWebsocketIPAddr, const uint32 &InWebsocketPort);

  URROSCommunicationComponent();

protected:
  void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

public:
  void AddPublisher(URPublisher *&InPublisher) { ROSCommunication.AddPublisher(InPublisher); }

  void AddSubscriber(URSubscriber *&InSubscriber) { ROSCommunication.AddSubscriber(InSubscriber); }

  void AddServiceClient(URServiceClient *&InServiceClient) { ROSCommunication.AddServiceClient(InServiceClient); }

  void AddActionServer(URActionServer *&InActionServer) { ROSCommunication.AddActionServer(InActionServer); }

protected:
  void Init() override;

private:
  UPROPERTY(EditAnywhere)
  FRROSCommunicationContainer ROSCommunication;
};