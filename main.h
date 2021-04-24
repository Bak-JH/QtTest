#pragma once
#include <vector>
#include "structs_and_classes.h"


// input geometry

std::vector<triangle> facet;        // define a vector object (container) called "facet" of type "triangle"
double x_min, x_max, delta_x;       // geometry extents along X
double y_min, y_max, delta_y;       // geometry extents along Y
double z_min, z_max, delta_z;       // geometry extents along Z
