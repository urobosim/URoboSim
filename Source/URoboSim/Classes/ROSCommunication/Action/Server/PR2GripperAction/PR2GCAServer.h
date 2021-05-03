#pragma once

#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "PR2GCAServer.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URPR2GCAServerParameter : public URActionServerParameter
{
  GENERATED_BODY()

public:
  URPR2GCAServerParameter()
  {
    ActionName = TEXT("l_gripper_controller/gripper_action");
    ControllerName = TEXT("GripperController");
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 
};

UCLASS()
class UROBOSIM_API URPR2GCAServer : public URActionServer
{
  GENERATED_BODY()

public:
  URPR2GCAServer();

public:
  void SetActionServerParameters(URActionServerParameter *&ActionServerParameters) override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 

protected:
  void CreateActionServer() override;
};
