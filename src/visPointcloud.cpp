/*
 * @Author: tyz 1872516355@qq.com
 * @Date: 2026-04-11 01:49:40
 * @LastEditors: tyz 1872516355@qq.com
 * @LastEditTime: 2026-04-11 02:23:22
 * @FilePath: /Desktop/点云可视化/src/visPointcloud.cpp
 * @Description: 可视化点云V1.0
 */
#include "visPointcloud.h"
#include <yaml-cpp/yaml.h>
#include <algorithm>

#define visColoredPoints 1
#if visColoredPoints
#define DATA_TYPE pcl::PointXYZRGB
typedef pcl::PointCloud<DATA_TYPE> PointCloud;
#else
#define DATA_TYPE pcl::PointXYZ
typedef pcl::PointCloud<DATA_TYPE> PointCloud;
#endif

// 提取目录下各个文件路径
void extractFilePath(const std::string& t_FileDir, std::vector<std::string>& t_FilePath){
    for (const auto& entry : std::filesystem::directory_iterator(t_FileDir))
    {
        if (!entry.is_regular_file())
            continue;
        std::string ext = entry.path().extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext == ".ply" || ext == ".pcd")
        {
            std::string full_path = std::filesystem::absolute(entry.path()).string();
            t_FilePath.push_back(full_path);
        }
    }

    return ;
}

int main(){
    std::vector<std::string>filesPath;
    // 读取配置文件
    std::string fileDir = "";
    YAML::Node config = YAML::LoadFile("../config/config.yaml");
    if(config["fileDir"].IsDefined()){
        std::cout << "fileDir: " << config["fileDir"].as<std::string>() << std::endl;
        fileDir = config["fileDir"].as<std::string>();
    }else{
        std::cout << "fileDir is not defined in config.yaml" << std::endl;
        return -1;
    }
    // 读取文件路径
    extractFilePath(fileDir, filesPath);
    std::sort(filesPath.begin(), filesPath.end());
    // 显示点云
    pcl::visualization::PCLVisualizer viewer("Point Cloud Viewer");
    while(!viewer.wasStopped()){
        for(auto file: filesPath){
            PointCloud::Ptr cloud(new PointCloud);
            if(pcl::io::loadPLYFile<DATA_TYPE>(file, *cloud) == -1){
                PCL_ERROR("Couldn't read file %s \n", file.c_str());
                return -1;
            }
            if(viewer.contains("VISCLOUD")){
                viewer.removePointCloud("VISCLOUD");
            }
            viewer.addPointCloud(cloud, "VISCLOUD");
            viewer.spinOnce(10);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    system("pause");
    return 0;
}