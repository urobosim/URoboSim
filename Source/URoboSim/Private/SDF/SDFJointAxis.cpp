// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "SDF/SDFJointAxis.h"
#include "SDF/SDFJoint.h"

USDFJointAxis::USDFJointAxis()
{
  Xyz = FVector(0.0, 0.0, 1.0);
}

void USDFJointAxis::SetLowerLimitFromSDF(float LowerLimit)
{
  // USDFJoint* Owner = Cast<USDFJoint>(GetOuter());
  Lower = LowerLimit;
}

void USDFJointAxis::SetUpperLimitFromSDF(float UpperLimit)
{
  // USDFJoint* Owner = Cast<USDFJoint>(GetOuter());
  Upper = UpperLimit;
}
