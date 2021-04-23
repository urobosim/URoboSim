#pragma once

#include "ROSCommunication/Actions/RActionServer.h"
#include "PR2HAServer.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URPointHeadActionServer : public URActionServer
{
  GENERATED_BODY()

public:

    URPointHeadActionServer();
};
