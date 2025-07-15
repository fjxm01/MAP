#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/common/common.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <vector>
#include <Eigen/Dense>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include "Jmath.h"
#include "PG.h"
#include "map/MAPC.h"
#include "viz/graph.h"

const float VOXEL_LEAF_SIZE = 0.01f;
const int POINTS_PER_GROUP = 500;

const float NORMAL_MAP_THRESHOLD = 1.5f;

int main(int argc, char **argv)
{
    std::filesystem::path exeDir = std::filesystem::current_path();
    std::filesystem::path folderPath = exeDir / "data" / "PCD";

    std::vector<std::string> Files;

    for (const auto &entry : std::filesystem::directory_iterator(folderPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".pcd")
        {
            Files.push_back(entry.path().string());
        }
    }

    const int numFiles = Files.size();
    std::vector<std::vector<int>> initialGroupAssignments(numFiles);
    std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr> initialClouds(numFiles);

    for (int i = 0; i < Files.size(); ++i)
    {
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
        if (pcl::io::loadPCDFile<pcl::PointXYZ>(Files[i], *cloud) == -1)
        {
            PCL_ERROR("Couldn't read file %s \n", Files[i].c_str());
            return (-1);
        }
        //--------------------------------------
        //* Step 1 - Group 생성
        //--------------------------------------
        GroupCreator groupCreator(VOXEL_LEAF_SIZE, POINTS_PER_GROUP);
        GroupResult result = groupCreator.fastPG(cloud);
        initialClouds[i] = result.PCflt;
        initialGroupAssignments[i] = result.PGa;
        const int numGroups = result.PGnum;

        //----------------------------------------
        //* Step 2 - normal vector MAP 계산
        //----------------------------------------
        std::vector<float> firstMAP(numGroups), lastMAP(numGroups), avgMAP(numGroups), minMAP(numGroups), maxMAP(numGroups);
        MAPCalculator calculator;
        for (int g = 0; g < numGroups; ++g)
        {
            MAPResult normalRes = calculator.MAPrunNormals(result.PCflt, result.PGa, g);
            firstMAP[g] = normalRes.firstMAP;
            lastMAP[g] = normalRes.lastMAP;
            avgMAP[g] = normalRes.meanMAP;
            minMAP[g] = normalRes.minMAP;
            maxMAP[g] = normalRes.maxMAP;
        }
        Base3DViz viz;
        viz.MAP(initialClouds[i], initialGroupAssignments[i], firstMAP, NORMAL_MAP_THRESHOLD);
        viz.MAP(initialClouds[i], initialGroupAssignments[i], lastMAP, NORMAL_MAP_THRESHOLD);
        viz.MAP(initialClouds[i], initialGroupAssignments[i], avgMAP, NORMAL_MAP_THRESHOLD);
        viz.MAP(initialClouds[i], initialGroupAssignments[i], minMAP, NORMAL_MAP_THRESHOLD);
        viz.MAP(initialClouds[i], initialGroupAssignments[i], maxMAP, NORMAL_MAP_THRESHOLD);
    }
    return 0;
}