#include "readASCII.h"
#include <cmath>
#include <iostream>
#include <fstream>
// -------------------------------------------------------------------------------------------------------------------
// this function opens the STL file specified by the user and reads the contents

int read_binary_STL_file(std::string STL_filename, std::vector<triangle>& facet,
    double& x_min, double& x_max, double& y_min, double& y_max, double& z_min, double& z_max)
{
    // specify the location of STL files on this computer
    std::string STL_files_path = "../stl/";

    // declare an (input) file object
    std::ifstream binaryInputFile;

    // open the STL file by using the full path and the name
    // specify that the file is opened in "read-only" mode
    // specify that the file is opened in binary format
    binaryInputFile.open(STL_filename.c_str(), std::ifstream::in | std::ifstream::binary);

    // check whether the file was opened successfully
    // if yes then continue otherwise terminate program execution
    if (binaryInputFile.fail())
    {
        std::cout << "ERROR: Input STL file could not be opened!" << std::endl;
        return (1);
    }

    // position the pointer to byte number 80
    binaryInputFile.seekg(80);

    // read the number of facets (triangles) in the STL geometry
    int numberOfTriangles;
    binaryInputFile.read((char*)&numberOfTriangles, sizeof(int));

    // declare an object "tri" of type triangle (see main.h for the definition of the triangle class)
    triangle tri;

    // storage space for the "unused bytes" 
    char unused_bytes[2];

    // initialize parameters that will be used to store the minimum and maximum extents of the geometry
    // described by the STL file
    x_min = 1e+30, x_max = -1e+30;
    y_min = 1e+30, y_max = -1e+30;
    z_min = 1e+30, z_max = -1e+30;

    // temporary floating point variable
    float temp_float_var;

    for (int count = 0; count < numberOfTriangles; count++)
    {
        // read the three components of the normal vector
        binaryInputFile.read((char*)&temp_float_var, 4); tri.normal.x = (double)temp_float_var;
        binaryInputFile.read((char*)&temp_float_var, 4); tri.normal.y = (double)temp_float_var;
        binaryInputFile.read((char*)&temp_float_var, 4); tri.normal.z = (double)temp_float_var;

        // read the three coordinates of vertex 1 
        binaryInputFile.read((char*)&temp_float_var, 4); tri.point[0].x = (double)temp_float_var;
        binaryInputFile.read((char*)&temp_float_var, 4); tri.point[0].y = (double)temp_float_var;
        binaryInputFile.read((char*)&temp_float_var, 4); tri.point[0].z = (double)temp_float_var;

        // read the three coordinates of vertex 2 
        binaryInputFile.read((char*)&temp_float_var, 4); tri.point[1].x = (double)temp_float_var;
        binaryInputFile.read((char*)&temp_float_var, 4); tri.point[1].y = (double)temp_float_var;
        binaryInputFile.read((char*)&temp_float_var, 4); tri.point[1].z = (double)temp_float_var;

        // read the three coordinates of vertex 3
        binaryInputFile.read((char*)&temp_float_var, 4); tri.point[2].x = (double)temp_float_var;
        binaryInputFile.read((char*)&temp_float_var, 4); tri.point[2].y = (double)temp_float_var;
        binaryInputFile.read((char*)&temp_float_var, 4); tri.point[2].z = (double)temp_float_var;

        // read the 2 unused bytes
        binaryInputFile.read(unused_bytes, 2);

        // update geometry extents along X, Y and Z based on vertex 1 of this triangle
        if (tri.point[0].x < x_min) x_min = tri.point[0].x;
        if (tri.point[0].x > x_max) x_max = tri.point[0].x;
        if (tri.point[0].y < y_min) y_min = tri.point[0].y;
        if (tri.point[0].y > y_max) y_max = tri.point[0].y;
        if (tri.point[0].z < z_min) z_min = tri.point[0].z;
        if (tri.point[0].z > z_max) z_max = tri.point[0].z;

        // update geometry extents along X, Y and Z based on vertex 2 of this triangle
        if (tri.point[1].x < x_min) x_min = tri.point[1].x;
        if (tri.point[1].x > x_max) x_max = tri.point[1].x;
        if (tri.point[1].y < y_min) y_min = tri.point[1].y;
        if (tri.point[1].y > y_max) y_max = tri.point[1].y;
        if (tri.point[1].z < z_min) z_min = tri.point[1].z;
        if (tri.point[1].z > z_max) z_max = tri.point[1].z;

        // update geometry extents along X, Y and Z based on vertex 3 of this triangle
        if (tri.point[2].x < x_min) x_min = tri.point[2].x;
        if (tri.point[2].x > x_max) x_max = tri.point[2].x;
        if (tri.point[2].y < y_min) y_min = tri.point[2].y;
        if (tri.point[2].y > y_max) y_max = tri.point[2].y;
        if (tri.point[2].z < z_min) z_min = tri.point[2].z;
        if (tri.point[2].z > z_max) z_max = tri.point[2].z;

        // add data for this triangle to the "facet" vector
        facet.push_back(tri);
    }

    // explicitly close the connection to the input STL file
    binaryInputFile.close();

    return (0);  // all is well
}
int read_ascii_STL_file(std::string STL_filename, std::vector<triangle>& facet,
    double& x_min, double& x_max, double& y_min, double& y_max, double& z_min, double& z_max)
{
    // specify the location of STL files on this computer
    std::string STL_files_path = "../stl/";

    // declare a (input) file object
    std::ifstream asciiInputFile;

    // open the STL file by using the full path and the name
    // specify that the file is opened in "read-only" mode
    asciiInputFile.open(STL_filename.c_str(), std::ifstream::in);

    // check whether the file was opened successfully
    // if yes then continue otherwise terminate program execution
    if (asciiInputFile.fail())
    {
        std::cout << "ERROR: Input STL file could not be opened!" << std::endl;
        return(1); // error
    }

    // read in the contents line by line until the file ends

    // initialize counter for counting the number of lines in this file
    int triangle_number = 0;

    // declare an object "tri" of type triangle (see above for the definition of the triangle struct)
    triangle tri;

    // declare some string objects
    std::string junk;
    std::string string1, string2;

    // read in the first line (until the /n delimiter) and store it in the string object "line"
    getline(asciiInputFile, junk);

    // initialize parameters that will be used to store the minimum and maximum extents of the geometry
    // described by the STL file
    x_min = 1e+30, x_max = -1e+30;
    y_min = 1e+30, y_max = -1e+30;
    z_min = 1e+30, z_max = -1e+30;

    // begin loop to read the rest of the file until the file ends
    while (true)
    {
        // read the components of the normal vector
        asciiInputFile >> string1 >> string2 >> tri.normal.x >> tri.normal.y >> tri.normal.z;        //  1
        // continue reading this line until the \n delimiter
        getline(asciiInputFile, junk);                                                                //  1

        // read the next line until the \n delimiter
        getline(asciiInputFile, junk);                                                                //  2

        // read the (x,y,z) coordinates of vertex 1            
        asciiInputFile >> string1 >> tri.point[0].x >> tri.point[0].y >> tri.point[0].z;             //  3
        getline(asciiInputFile, junk);                                                                //  3

        // read the (x,y,z) coordinates of vertex 2            
        asciiInputFile >> string1 >> tri.point[1].x >> tri.point[1].y >> tri.point[1].z;             //  4
        getline(asciiInputFile, junk);                                                                //  4

        // read the (x,y,z) coordinates of vertex 3            
        asciiInputFile >> string1 >> tri.point[2].x >> tri.point[2].y >> tri.point[2].z;             //  5
        getline(asciiInputFile, junk);                                                                //  5

        // read some more junk
        getline(asciiInputFile, junk);                                                                //  6
        getline(asciiInputFile, junk);                                                                //  7

        // update geometry extents along X, Y and Z based on vertex 1 of this triangle
        if (tri.point[0].x < x_min) x_min = tri.point[0].x;
        if (tri.point[0].x > x_max) x_max = tri.point[0].x;
        if (tri.point[0].y < y_min) y_min = tri.point[0].y;
        if (tri.point[0].y > y_max) y_max = tri.point[0].y;
        if (tri.point[0].z < z_min) z_min = tri.point[0].z;
        if (tri.point[0].z > z_max) z_max = tri.point[0].z;

        // update geometry extents along X, Y and Z based on vertex 2 of this triangle
        if (tri.point[1].x < x_min) x_min = tri.point[1].x;
        if (tri.point[1].x > x_max) x_max = tri.point[1].x;
        if (tri.point[1].y < y_min) y_min = tri.point[1].y;
        if (tri.point[1].y > y_max) y_max = tri.point[1].y;
        if (tri.point[1].z < z_min) z_min = tri.point[1].z;
        if (tri.point[1].z > z_max) z_max = tri.point[1].z;

        // update geometry extents along X, Y and Z based on vertex 3 of this triangle
        if (tri.point[2].x < x_min) x_min = tri.point[2].x;
        if (tri.point[2].x > x_max) x_max = tri.point[2].x;
        if (tri.point[2].y < y_min) y_min = tri.point[2].y;
        if (tri.point[2].y > y_max) y_max = tri.point[2].y;
        if (tri.point[2].z < z_min) z_min = tri.point[2].z;
        if (tri.point[2].z > z_max) z_max = tri.point[2].z;

        // break out of the while loop if "end-of-file" becomes true
        if (asciiInputFile.eof()) break;

        // increment the triangle number
        triangle_number++;

        // add data for this triangle to the "facet" vector
        facet.push_back(tri);
    }
    // end while loop

    // explicitly close the output file
    asciiInputFile.close();

    return (0);   // all is well
}

int getGeometryInput(const std::string& STL_filename,
    std::vector<triangle>& facet,
    double& x_min, double& x_max,
    double& y_min, double& y_max,
    double& z_min, double& z_max)
{

    bool isA = false;

    // END algorithm from ParaView to detect whether the STL file is ASCII or BINARY
    if (isA)
        std::cout << "Input Geometry File = " << STL_filename << " (is Ascii)" << std::endl;
    else
        std::cout << "Input Geometry File = " << STL_filename << " (is bin)" << std::endl;


    // function call to open the ascii file and read the content
    if (isA)
    {
        // call the function to read the ASCII file
        // if all goes well, this function will return a value of 0
        int error = read_ascii_STL_file(STL_filename, facet, x_min, x_max, y_min, y_max, z_min, z_max);

        // if something goes wrong, the above function will return a value of 1
        if (error == 1)
        {
            return(1);       // exit returning an error
        }
    }
    else
    {
        // call the function to read the BINARY file
        // if all goes well, this function will return a value of 0
        int error = read_binary_STL_file(STL_filename, facet, x_min, x_max, y_min, y_max, z_min, z_max);

        // if something goes wrong, the above function will return a value of 1
        if (error == 1)
        {
            return(1);       // exit returning an error
        }
    }
    // print the number of triangles
    std::cout << "The number of triangles in the STL file = " << facet.size() << std::endl;

    // print the domain extent along X, Y and Z
    std::cout << std::endl;
    std::cout << "X range : " << x_min << " to " << x_max << " (delta = " << x_max - x_min << ")" << std::endl;
    std::cout << "Y range : " << y_min << " to " << y_max << " (delta = " << y_max - y_min << ")" << std::endl;
    std::cout << "Z range : " << z_min << " to " << z_max << " (delta = " << z_max - z_min << ")" << std::endl;
    std::cout << std::endl;

    return (0);  // all is well
}

void createVertexArray(std::vector<triangle>& facet, float* vertices)
{
    const int NUM_FACETS = facet.size();
    int count = 0;
    for (int facet_index = 0; facet_index < NUM_FACETS; facet_index++)
    {
        // vertex 0
        {
            // x-y-z coordinates of the point
            vertices[count ++] = facet[facet_index].point[0].x;
            vertices[count ++] = facet[facet_index].point[0].y;
            vertices[count ++] = facet[facet_index].point[0].z;
        }

        // vertex 1
        {
            // x-y-z coordinates of the point
            vertices[count ++] = facet[facet_index].point[1].x;
            vertices[count ++] = facet[facet_index].point[1].y;
            vertices[count ++] = facet[facet_index].point[1].z;
        }

        // vertex 2
        {
            // x-y-z coordinates of the point
            vertices[count ++] = facet[facet_index].point[2].x;
            vertices[count ++] = facet[facet_index].point[2].y;
            vertices[count ++] = facet[facet_index].point[2].z;
        }
    }
}

void createVertexArrayB(const std::vector<triangle>& facet, float* vertices)
{
    const int NUM_FACETS = facet.size();
    int count = 0;
    for (int facet_index = 0; facet_index < NUM_FACETS; facet_index++)
    {
        // vertex 0
        {
            // x-y-z coordinates of the point
            vertices[count + 0] = facet[facet_index].point[0].x;
            vertices[count + 1] = facet[facet_index].point[0].y;
            vertices[count + 2] = facet[facet_index].point[0].z;
            // x-y-z coordinates of the normal
            vertices[count + 3] = facet[facet_index].normal.x;
            vertices[count + 4] = facet[facet_index].normal.y;
            vertices[count + 5] = facet[facet_index].normal.z;
            // (R,G,B,A) color values 
            vertices[count + 6] = fabs(facet[facet_index].normal.x);
            vertices[count + 7] = fabs(facet[facet_index].normal.y);
            vertices[count + 8] = fabs(facet[facet_index].normal.z);
            vertices[count + 9] = 1.0;
        }

        // vertex 1
        {
            // x-y-z coordinates of the point
            vertices[count + 10] = facet[facet_index].point[1].x;
            vertices[count + 11] = facet[facet_index].point[1].y;
            vertices[count + 12] = facet[facet_index].point[1].z;
            // x-y-z coordinates of the normal
            vertices[count + 13] = facet[facet_index].normal.x;
            vertices[count + 14] = facet[facet_index].normal.y;
            vertices[count + 15] = facet[facet_index].normal.z;
            // (R,G,B,A) color values 
            vertices[count + 16] = fabs(facet[facet_index].normal.x);
            vertices[count + 17] = fabs(facet[facet_index].normal.y);
            vertices[count + 18] = fabs(facet[facet_index].normal.z);
            vertices[count + 19] = 1.0;
        }

        // vertex 2
        {
            // x-y-z coordinates of the point
            vertices[count + 20] = facet[facet_index].point[2].x;
            vertices[count + 21] = facet[facet_index].point[2].y;
            vertices[count + 22] = facet[facet_index].point[2].z;
            // x-y-z coordinates of the normal
            vertices[count + 23] = facet[facet_index].normal.x;
            vertices[count + 24] = facet[facet_index].normal.y;
            vertices[count + 25] = facet[facet_index].normal.z;
            // (R,G,B,A) color values 
            vertices[count + 26] = fabs(facet[facet_index].normal.x);
            vertices[count + 27] = fabs(facet[facet_index].normal.y);
            vertices[count + 28] = fabs(facet[facet_index].normal.z);
            vertices[count + 29] = 1.0;
        }

        count += 30;
    }
}


