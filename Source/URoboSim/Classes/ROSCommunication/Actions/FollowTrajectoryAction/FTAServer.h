#pragma once

#include "ROSCommunication/Actions/RActionServer.h"
#include "FTAServer.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URFollowTrajectoryActionServer : public URActionServer
{
  GENERATED_BODY()
public:
    URFollowTrajectoryActionServer();
};
