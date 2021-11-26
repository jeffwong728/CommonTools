#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include "videofile_global.h"
#include "icamera.h"
#include <QObject>
#include <QImage.h>

class VIDEOFILE_LIBRARY_EXPORT VideoFile : public ICamera {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "testqt_camera_interface")
  Q_INTERFACES(ICamera)

public:
  // Override abstract function to emit the `name` signal
  void snap() override { emit image_ready(QImage()); }
};

#endif // VIDEOFILE_H
