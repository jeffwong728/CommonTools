#include "videofile.h"

VideoFile::VideoFile()
{
}

VideoFile::~VideoFile()
{
    Stop();
    thread.requestInterruption();
    thread.quit();
    thread.wait();
}

bool VideoFile::Open(const QString& camSrc)
{
    setObjectName(camSrc);
    filename = camSrc.toStdString();
    if (!thread.isRunning())
    {
        moveToThread(&thread);
        timer.moveToThread(&thread);
        connect(&thread, &QThread::started, this, &VideoFile::thread_started);
        connect(&thread, &QThread::finished, this, &VideoFile::thread_finished);
        connect(&timer, &QTimer::timeout, this, &VideoFile::snap_image);
        thread.start();
    }
    return true;
}

bool VideoFile::Close()
{
    Stop();
    if (thread.isRunning())
    {
        thread.requestInterruption();
        thread.quit();
        thread.wait();
    }
    return true;
}

void VideoFile::OneShot()
{
    if (!QMetaObject::invokeMethod(this, &VideoFile::OneShotAsync, Qt::AutoConnection))
    {
        qDebug() << this << "OneShot failed";
    }
}

void VideoFile::Trigger(int msec)
{
    if (!QMetaObject::invokeMethod(this, "TriggerAsync", Qt::AutoConnection, Q_ARG(int, msec)))
    {
        qDebug() << this << "Trigger failed";
    }
}

void VideoFile::Stop()
{
    if (!QMetaObject::invokeMethod(this, &VideoFile::StopAsync, Qt::AutoConnection))
    {
        qDebug() << this << "Stop failed";
    }
}

QImage VideoFile::SnapAsync()
{
    if (!vc.isOpened())
    {
        vc.open(filename);
    }

    if (vc.isOpened())
    {
        vc >> frame;
        if (frame.empty())
        {
            vc.set(cv::CAP_PROP_POS_FRAMES, 0);
            vc >> frame;
        }
        if (!frame.empty())
        {
            if (frame.type() == CV_8UC3)
            {
                QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
                image.rgbSwap();
                return image;
            }
            else if (frame.type() == CV_8UC4)
            {
                QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_ARGB32);
                return image;
            }
            else
            {
                qDebug() << "ERROR: Mat could not be converted to QImage.";
            }
        }
    }

    return QImage();
}

void VideoFile::OneShotAsync()
{
    QImage image = SnapAsync();
    if (!image.isNull())
    {
        emit image_ready(image);
    }
}

void VideoFile::TriggerAsync(int msec)
{
    timer.start(msec);
}

void VideoFile::StopAsync()
{
    timer.stop();
}

void VideoFile::thread_started()
{
    qDebug() << this << "thread started.";
}

void VideoFile::thread_finished()
{
    qDebug() << this << "thread finished.";
    timer.stop();
    vc.release();
}

void VideoFile::snap_image()
{
    QImage image = SnapAsync();
    if (!image.isNull())
    {
        emit image_ready(image);
    }
}

QImage VideoFile::Snap()
{
    QImage image;
    if (!QMetaObject::invokeMethod(this, &VideoFile::SnapAsync, Qt::BlockingQueuedConnection, &image))
    {
        qDebug() << this << "Snap failed";
    }
    return image;
}
