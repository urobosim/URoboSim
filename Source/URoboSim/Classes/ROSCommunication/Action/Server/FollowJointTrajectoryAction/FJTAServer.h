#pragma once

#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "FJTAServer.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTAServerParameter final : public URActionServerParameter
{
  GENERATED_BODY()

public:
  URFJTAServerParameter()
  {
    ActionName = TEXT("/whole_body_controller/body/follow_joint_trajectory");
    ControllerName = TEXT("JointController");
    FrameId = TEXT("odom");
    JointParamTopic = TEXT("/whole_body_controller/body/joints");
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 

  UPROPERTY(EditAnywhere)
  FString JointParamTopic; 
};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URFJTAServer final : public URActionServer
{
  GENERATED_BODY()

public:
  URFJTAServer();

public:
  void SetActionServerParameters(URActionServerParameter *&ActionServerParameters) override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 

  UPROPERTY(EditAnywhere)
  FString JointParamTopic; 

protected:
  void CreateActionServer() override;
};
