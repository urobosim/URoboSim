#pragma once

#include "ROSCommunication/Action/Server/RActionServer.h"
#include "Controller/ControllerType/SpecialController/RCameraController.h"
// clang-format off
#include "POAServer.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URPOAServerParameter : public URActionServerParameter
{
  GENERATED_BODY()

public:
  URPOAServerParameter()
  {
    ActionName = TEXT("/perceive_object");
    ControllerName = TEXT("CameraController");
  }
};

UCLASS()
class UROBOSIM_API URPOAServer final : public URActionServer
{
  GENERATED_BODY()

public:
  URPOAServer();

protected:
  void CreateActionServer() override;
};
