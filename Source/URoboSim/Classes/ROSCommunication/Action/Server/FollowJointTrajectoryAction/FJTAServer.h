#pragma once

#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "FJTAServer.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URFJTAServer : public URActionServer
{
  GENERATED_BODY()

public:
  URFJTAServer();
};
