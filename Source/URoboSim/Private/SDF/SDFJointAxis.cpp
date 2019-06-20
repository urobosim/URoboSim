// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "SDF/SDFJointAxis.h"
#include "SDF/SDFJoint.h"

USDFJointAxis::USDFJointAxis()
{
	Xyz = FVector(0.0, 0.0, 1.0);
}

void USDFJointAxis::SetLowerLimitFromSDF(float LowerLimit)
{
	USDFJoint* Owner = Cast<USDFJoint>(GetOuter());

	if(Owner->Type.Equals("revolute"))
	{
		Lower = FMath::RadiansToDegrees(LowerLimit);
	}
	else if(Owner->Type.Equals("prismatic"))
	{
		Lower = LowerLimit * 100;
	}
	else
	{
		Lower = LowerLimit;
	}

}

void USDFJointAxis::SetUpperLimitFromSDF(float UpperLimit)
{
	USDFJoint* Owner = Cast<USDFJoint>(GetOuter());

	if(Owner->Type.Equals("revolute"))
	{
		Upper = FMath::RadiansToDegrees(UpperLimit);
	}
	else if(Owner->Type.Equals("prismatic"))
	{
		Upper = UpperLimit * 100;
	}
	else
	{
		Upper = UpperLimit;
	}

}

