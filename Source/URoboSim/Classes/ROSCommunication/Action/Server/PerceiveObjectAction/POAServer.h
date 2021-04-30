#pragma once

#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "POAServer.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URPOAServer final : public URActionServer
{
  GENERATED_BODY()

public:
  URPOAServer();

protected:
  void CreateActionServer() override;
};
