// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Physics/RModel.h"
#include "UObject/Interface.h"
// clang-format off
#include "RobotInterface.generated.h"
// clang-format on

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URobotInterface : public UInterface
{
    GENERATED_BODY()
};

class UROBOSIM_API IRobotInterface
{
  GENERATED_BODY()

public:
  ARModel *GetOwner() const { return Owner; }

  void SetOwner(UObject *InOwner);

private:
  ARModel *Owner;
};
