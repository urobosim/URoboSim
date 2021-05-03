#pragma once

#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "PR2HAServer.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URPR2HAServerParameter : public URActionServerParameter
{
  GENERATED_BODY()

public:
  URPR2HAServerParameter()
  {
    ActionName = TEXT("/head_traj_controller/point_head_action");
    ControllerName = TEXT("HeadController");
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 
};

UCLASS()
class UROBOSIM_API URPR2HAServer : public URActionServer
{
  GENERATED_BODY()

public:
  URPR2HAServer();

public:
  void SetActionServerParameters(URActionServerParameter *&ActionServerParameters) override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 

protected:
  void CreateActionServer() override;
};
