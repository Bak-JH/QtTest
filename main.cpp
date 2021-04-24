#include <QtCore/QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <fstream>

#include "structs_and_classes.h"
#include "readASCII.h"

using namespace boost::interprocess;

struct SlicerArgs
{
    float delta;
    size_t sampleXY;
    size_t sampleZ;
    float minHeight;
    float maxHeight;
    float pixelWidth;
    size_t imgX;
    size_t imgY;
    size_t vtcCount;
    SlicerArgs(float delta, size_t sampleXY, size_t sampleZ, float minHeight, float maxHeight, float pixelWidth, size_t imgX, size_t imgY, size_t vtcCount) 
        :delta(delta), sampleXY(sampleXY), sampleZ(sampleZ), minHeight(minHeight), maxHeight(maxHeight), pixelWidth(pixelWidth), imgX(imgX), imgY(imgY) , vtcCount(vtcCount)
    {}
};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    SlicerArgs arg = {0.1f, 1, 0, 0.1f, 10, 0.5f, 2560, 1620, 108};


    std::vector<triangle> facet;        // define a vector object (container) called "facet" of type "triangle"
    double x_min, x_max, delta_x;       // geometry extents along X
    double y_min, y_max, delta_y;       // geometry extents along Y
    double z_min, z_max, delta_z;       // geometry extents along Z


    // check if all command line arguments were supplied
    //--------------------------------------------------
    // read STL geometry (triangle vertices and normals)
    //--------------------------------------------------
    int error = getGeometryInput(argv[1], facet,
        x_min, x_max,
        y_min, y_max,
        z_min, z_max);

    //if (error == 1) return(1); 

     //-------------------------------------------------------
    // calculate translation parameters to move the center of 
    // the geometry approximately to the origin
    //-------------------------------------------------------
    float x_center = (x_min + x_max) / 2.0;
    float y_center = (y_min + y_max) / 2.0;
    float z_center = (z_min + z_max) / 2.0;

    float move_x = 0.0 - x_center;   // translation along X
    float move_y = 0.0 - y_center;   // translation along Y
    float move_z = 0.0 - z_center;   // translation along Z

    // calculate scaling factors along X, Y and Z
    float scale_x = (x_max - x_min) / 2.0;
    float scale_y = (y_max - y_min) / 2.0;
    float scale_z = (z_max - z_min) / 2.0;

    float scale_0 = scale_x;
    if (scale_y > scale_0) scale_0 = scale_y;
    if (scale_z > scale_0) scale_0 = scale_z;

    //------------------------------------------
    // create a vertex array based on facet data
    //------------------------------------------
    auto ptSize = facet.size() * 3 * 3;
    float* vertices = new float[ptSize];

    createVertexArray(facet, vertices);

    QProcess* process = new QProcess();
    process->waitForStarted(-1);
    process->waitForFinished(-1);

    struct shm_remove
    {
        shm_remove() { shared_memory_object::remove("SlicerSharedMemory"); }
        ~shm_remove() { shared_memory_object::remove("SlicerSharedMemory"); }
    } remover;

    //Create a shared memory object.
    managed_shared_memory shm(create_only, "SlicerSharedMemory", facet.size() * 3 * 3 * 4 * 8);


    shm.destroy<SlicerArgs>("args");
    shm.destroy<float*>("vtcs");

    float test[3] = { 0.0f, 0.1f, 0.2f };
    auto myclass = shm.construct<SlicerArgs>("args")[9](1.0f, 1, 1, 0.1, 20, 4, 2560, 1620, 108);

    auto  array_it = shm.construct_it<float>
        ("vtcs")   //name of the object
        [ptSize]                        //number of elements
          (&vertices[0]);    //Iterator for the 2nd ctor argument


    auto vtcSeg = shm.find<float>("vtcs");

    QString agentName = "/GLApp0.exe";
    QString agentPath = QCoreApplication::applicationDirPath() + agentName;
    QStringList args = QStringList();
    args = QStringList({ "-Command", QString("Start-Process %1").arg(agentPath) });
    process->start("powershell", args);
    auto err0 = process->error();
    auto err = process->errorString();
    //QObject::connect(process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), &a, &QCoreApplication::exit);
 /*   QString program = "D:/Qt/5.12.4/Examples/Qt-5.12.4/widgets/analogclock";
    QStringList arguments;
    arguments << "-style" << "fusion";

    QProcess* myProcess = new QProcess();
    myProcess->start(program, arguments);
    qDebug() << "done";
    myProcess->terminate();*/

    return a.exec();
}
