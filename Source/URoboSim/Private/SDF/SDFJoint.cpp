// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "SDF/SDFJoint.h"

// Constructor
USDFJoint::USDFJoint()
{
	Axis = CreateDefaultSubobject<USDFJointAxis>(TEXT("Axis"));
}


