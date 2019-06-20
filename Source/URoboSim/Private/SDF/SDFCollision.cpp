// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "SDF/SDFCollision.h"

// Constructor
USDFCollision::USDFCollision()
{
	Geometry = CreateDefaultSubobject<USDFGeometry>(TEXT("Geometry"));
}


