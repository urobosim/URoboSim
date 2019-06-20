// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "SDF/SDFVisual.h"

// Constructor
USDFVisual::USDFVisual()
{
	Geometry = CreateDefaultSubobject<USDFGeometry>(TEXT("Geometry"));
}


