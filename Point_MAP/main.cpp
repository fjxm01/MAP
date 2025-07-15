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
#include "map/PG.h"
#include "map/MAPC.h"
#include "viz/PC_viz.h"

const float VOXEL_LEAF_SIZE = 0.005f;
const int POINTS_PER_GROUP = 500;

const float POINT_MAP_THRESHOLD = 0.1f;

int main(int argc, char **argv)
{
    std::filesystem::path folderPath = std::filesystem::current_path() / "data" / "PCD";
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

    for (int i = 0; i < numFiles; ++i)
    {
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
        if (pcl::io::loadPCDFile<pcl::PointXYZ>(Files[i], *cloud) == -1)
        {
            PCL_ERROR("Couldn't read file %s \n", Files[i].c_str());
            return -1;
        }

        //-------------------------
        //* Step1 - Group 생성
        //-------------------------
        GroupCreator groupCreator(VOXEL_LEAF_SIZE, POINTS_PER_GROUP);
        GroupResult result = groupCreator.fastPG(cloud);
        initialClouds[i] = result.PCflt;
        initialGroupAssignments[i] = result.PGa;
        const int numGroups = result.PGnum;

        //--------------------------
        //* Step2 - MAP 계산
        //--------------------------
        std::vector<float> firstMAP(numGroups), lastMAP(numGroups), avgMAP(numGroups), minMAP(numGroups), maxMAP(numGroups);
        MAPCalculator calculator;
        for (int g = 0; g < numGroups; ++g)
        {
            MAPResult pointRes = calculator.MAPrun(result.PCflt, result.PGa, g);
            firstMAP[g] = pointRes.firstMAP;
            lastMAP[g] = pointRes.lastMAP;
            avgMAP[g] = pointRes.meanMAP;
            minMAP[g] = pointRes.minMAP;
            maxMAP[g] = pointRes.maxMAP;
        }
        Base3DViz viz;
        viz.MAP(initialClouds[i], initialGroupAssignments[i], firstMAP, POINT_MAP_THRESHOLD);
        viz.MAP(initialClouds[i], initialGroupAssignments[i], lastMAP, POINT_MAP_THRESHOLD);
        viz.MAP(initialClouds[i], initialGroupAssignments[i], avgMAP, POINT_MAP_THRESHOLD);
        viz.MAP(initialClouds[i], initialGroupAssignments[i], minMAP, POINT_MAP_THRESHOLD);
        viz.MAP(initialClouds[i], initialGroupAssignments[i], maxMAP, POINT_MAP_THRESHOLD);
    }

    return 0;
}
