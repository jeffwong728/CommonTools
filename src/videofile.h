#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include "videofile_global.h"
#include "icamera.h"
#include "opencv2/opencv.hpp"
#include <QDebug>
#include <QTimer>
#include <QThread>

class VideoFile : public ICamera {
    Q_OBJECT
public:
    VideoFile();
    ~VideoFile();

public:
    bool Open(const QString& src) override;
    bool Close() override;
    QImage Snap() override;
    void OneShot() override;
    void Trigger(int msec) override;
    void Stop() override;

private slots:
    QImage SnapAsync();
    void OneShotAsync();
    void TriggerAsync(int msec);
    void StopAsync();

private:
    void thread_started();
    void thread_finished();
    void snap_image();

private:
    cv::String filename;
    cv::VideoCapture vc;
    cv::Mat frame;
    QThread thread;
    QTimer timer;
};

#endif // VIDEOFILE_H
