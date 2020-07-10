// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "SDF/SDFCollision.h"

// Constructor
USDFCollision::USDFCollision()
{
	Geometry = CreateDefaultSubobject<USDFGeometry>(TEXT("Geometry"));
}
