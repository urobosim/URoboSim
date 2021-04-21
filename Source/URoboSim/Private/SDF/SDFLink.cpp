// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "SDF/SDFLink.h"

// Constructor
USDFLink::USDFLink()
{
	bGravity = false;
	bSelfCollide = false;
	Inertial = CreateDefaultSubobject<USDFLinkInertial>(TEXT("Inertial"));
}
