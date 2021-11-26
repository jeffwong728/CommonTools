#ifndef ICAMERA_H
#define ICAMERA_H

#include "icamera_global.h"
#include <QObject>
#include <QImage.h>

class ICAMERA_LIBRARY_EXPORT ICamera : public QObject {
  Q_OBJECT
public:
  virtual ~ICamera() {}

  // Slot which should cause emission of `image_ready` signal.
  virtual void snap() = 0;

signals:
  // Signal to be emitted in getName()
  void image_ready(QImage);
};

#define CameraInterfaceIID "testqt_camera_interface"
Q_DECLARE_INTERFACE(ICamera, CameraInterfaceIID)

#endif // ICAMERA_H
