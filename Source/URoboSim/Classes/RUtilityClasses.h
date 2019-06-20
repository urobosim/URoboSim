#pragma once

#include "CoreMinimal.h"

struct FJointState
{
public:
	TArray<FString> JointNames;
	TArray<float> JointPositions;
	TArray<float> JointVelocities;
};
