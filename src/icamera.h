#ifndef ICAMERA_H
#define ICAMERA_H

#include "icamera_global.h"
#include <QObject>
#include <QImage.h>

class ICAMERA_LIBRARY_EXPORT ICamera : public QObject {
  Q_OBJECT
public:
  virtual ~ICamera() = default;

public slots:
  virtual bool Open(const QString &src) = 0;
  virtual bool Close() = 0;
  virtual QImage Snap() = 0;
  virtual void OneShot() = 0;
  virtual void Trigger(int msec) = 0;
  virtual void Stop() = 0;

signals:
  // Signal to be emitted in getName()
  void image_ready(QImage);
};

#define CameraInterfaceIID "testqt_camera_interface"
Q_DECLARE_INTERFACE(ICamera, CameraInterfaceIID)

class ICAMERA_LIBRARY_EXPORT ICameraManager : public QObject
{
    Q_OBJECT;
public:
    virtual ~ICameraManager() = default;

public slots:
    virtual int GetNumCameras() = 0;
    virtual ICamera* GetCamera(const int index) = 0;

signals:
    void camera_ready(ICamera *);
    void camera_found(ICamera *);
};

#define CameraManagerInterface_iid "org.qt-project.Qt.Examples.CameraManagerInterface"

Q_DECLARE_INTERFACE(ICameraManager, CameraManagerInterface_iid)

#endif // ICAMERA_H
