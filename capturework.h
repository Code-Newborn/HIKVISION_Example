#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QObject>
#include <MvCamera.h>
#include <QImage>
#include <QDebug>
#include <QThread>

/// @brief 抓图线程类
class CaptureWork : public QObject
{
    Q_OBJECT
public:
    explicit CaptureWork(QObject *parent = nullptr);
    ~CaptureWork();

    void getCameraPtr(MvCamera *cameraPtr); // 获取相机对象
    void getImagePtr(cv::Mat *imagePtr);    // 获取图像指针

    void stopWork();  // 终止子线程循环函数
    void startWork(); // 终止子线程循环函数

signals:
    void signal_messImage(QImage myImage);

    void workFinished(); //线程函数完成信号

public slots:
    void Work(); // 线程函数

private:
    MvCamera *cameraPtr; // 相机对象
    cv::Mat *imagePtr;   // 抓取图像

    cv::Mat SrcImage;
    cv::Mat GrayImage;
    cv::Mat ThresImage;

    QImage *capture_Qimage;
    bool IsCapturing;
};

#endif // CAPTURETHREAD_H
