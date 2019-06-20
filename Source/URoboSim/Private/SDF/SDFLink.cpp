// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "SDF/SDFLink.h"

// Constructor
USDFLink::USDFLink()
{
	bGravity = 1;
	bSelfCollide = 0;
	Inertial = CreateDefaultSubobject<USDFLinkInertial>(TEXT("Inertial"));
}


