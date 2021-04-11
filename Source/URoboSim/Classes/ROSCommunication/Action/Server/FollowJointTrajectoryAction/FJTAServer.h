#pragma once

#include "ROSCommunication/Action/Server/RActionServer.h"
#include "FJTAServerParameter.h"
// clang-format off
#include "FJTAServer.generated.h"
// clang-format on

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
  FString JointParamPath; 

protected:
  void Init() override;
};
