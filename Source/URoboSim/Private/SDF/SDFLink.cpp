// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "SDF/SDFLink.h"

// Constructor
USDFLink::USDFLink()
{
	bGravity = 1;
	bSelfCollide = 0;
	Inertial = CreateDefaultSubobject<USDFLinkInertial>(TEXT("Inertial"));
}
