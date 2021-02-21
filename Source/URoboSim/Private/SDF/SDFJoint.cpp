// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "SDF/SDFJoint.h"

// Constructor
USDFJoint::USDFJoint()
{
	Axis = CreateDefaultSubobject<USDFJointAxis>(TEXT("Axis"));
}
