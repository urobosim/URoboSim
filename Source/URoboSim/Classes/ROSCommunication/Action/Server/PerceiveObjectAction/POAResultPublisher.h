
#pragma once

#include "Controller/ControllerType/SpecialController/RCameraController.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
#include "ROSCommunication/Publisher/RTFPublisher.h"
// clang-format off
#include "POAResultPublisher.generated.h"
// clang-format on

DECLARE_MULTICAST_DELEGATE_OneParam(FObjectDetection, UObject * /*PerceivedObject*/);

UCLASS()
class UROBOSIM_API URPOAResultPublisher final : public URActionPublisher
{
  GENERATED_BODY()

public:
  URPOAResultPublisher();

public:
  void Publish() override;

protected:
  void Init() override;
  
private:
  URCameraController *CameraController;

  URTFPublisher *TFPublisher;

  FObjectDetection OnObjectDetected;
};
