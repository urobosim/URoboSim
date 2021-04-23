#pragma once

#include "ROSCommunication/Action/Server/RActionServer.h"
#include "POAServer.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URPerceiveObjectActionServer : public URActionServer
{
  GENERATED_BODY()

public:

    URPerceiveObjectActionServer();

};
