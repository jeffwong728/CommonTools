#include "videofilemanager.h"
#include "videofile.h"
#include "opencv2/opencv.hpp"
#include <qdebug.h>
#include <QThread>

constexpr int k_num_video_file_cameras = 5;
struct VideoFileCameraManagerImpl
{
    std::vector<std::unique_ptr<VideoFile>> cameras;
};

VideoFileCameraManager::VideoFileCameraManager()
    : impl(std::make_unique<VideoFileCameraManagerImpl>())
{
    for (int cc = 0; cc < k_num_video_file_cameras; ++cc)
    {
        impl->cameras.push_back(std::make_unique<VideoFile>());
    }
}

VideoFileCameraManager::~VideoFileCameraManager()
{
}

int VideoFileCameraManager::GetNumCameras()
{
    return k_num_video_file_cameras;
}

ICamera* VideoFileCameraManager::GetCamera(const int index)
{
    if (index >=0 && index < k_num_video_file_cameras)
    {
        return impl->cameras[index].get();
    }
    else
    {
        return nullptr;
    }
}
