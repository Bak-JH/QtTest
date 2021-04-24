#pragma once
#include <string>
#include <vector>
#include "structs_and_classes.h"

int read_ascii_STL_file(std::string STL_filename, std::vector<triangle>& facet,
    double& x_min, double& x_max, double& y_min, double& y_max, double& z_min, double& z_max);

int getGeometryInput(const std::string& STL_filename,
    std::vector<triangle>& facet,
    double& x_min, double& x_max,
    double& y_min, double& y_max,
    double& z_min, double& z_max);

void createVertexArray(std::vector<triangle>& facet, float* vertices);
void createVertexArrayB(std::vector<triangle>& facet, float* vertices);