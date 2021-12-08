#ifndef VIDEOFILEMANAGER_H
#define VIDEOFILEMANAGER_H

#include "videofile_global.h"
#include "icamera.h"
#include <QObject>
#include <memory>
struct VideoFileCameraManagerImpl;

class VIDEOFILE_LIBRARY_EXPORT VideoFileCameraManager : public ICameraManager {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID CameraManagerInterface_iid)
    Q_INTERFACES(ICameraManager)
public:
    VideoFileCameraManager();
    ~VideoFileCameraManager();

public:
    // Override abstract function to emit the `name` signal
    int GetNumCameras() override;
    ICamera* GetCamera(const int index) override;

private:
    std::unique_ptr<VideoFileCameraManagerImpl> impl;
};

#endif // VIDEOFILEMANAGER_H
