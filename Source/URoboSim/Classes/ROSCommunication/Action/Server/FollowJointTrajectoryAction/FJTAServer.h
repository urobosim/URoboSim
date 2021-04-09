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

protected:
  void Init() override;
};
