#include "capturework.h"
#include <QTimer>
#include <QEventLoop>

CaptureWork::CaptureWork(QObject *parent) : QObject(parent)
{
    this->imagePtr = new cv::Mat();
    this->cameraPtr = NULL;
    this->capture_Qimage = new QImage();
}

CaptureWork::~CaptureWork()
{
    delete capture_Qimage; // 销毁抓图指针
    if (cameraPtr != NULL) // 销毁相机指针
    {
        delete cameraPtr;
    }
    if (imagePtr != NULL) // 销毁图像指针
    {
        delete imagePtr;
    }
}

void CaptureWork::getCameraPtr(MvCamera *cameraPtr)
{
    this->cameraPtr = cameraPtr;
}

void CaptureWork::getImagePtr(cv::Mat *imagePtr)
{
    this->imagePtr = imagePtr;
}

void CaptureWork::stopWork()
{
    IsCapturing = false;
}

void CaptureWork::startWork()
{
    IsCapturing = true;
}

void CaptureWork::Work()
{
    if (cameraPtr == NULL) // 线程中不存在相机对象
    {
        return;
    }
    if (imagePtr == NULL) // 线程中不存在图像指针
    {
        return;
    }

    while (IsCapturing) // 抓取指示
    {
        cameraPtr->CommandExecute("TriggerSoftware"); // 执行一次软件触发采图，主动取流
        cameraPtr->ReadBuffer(*imagePtr);             // 从相机读取一帧图像Mat格式
        if (imagePtr->channels() > 1)
        {
            *capture_Qimage = QImage((const unsigned char *)(imagePtr->data), imagePtr->cols, imagePtr->rows, QImage::Format_RGB888);
        }
        else
        {
            *capture_Qimage = QImage((const unsigned char *)(imagePtr->data), imagePtr->cols, imagePtr->rows, QImage::Format_Indexed8);
        }
        QSize test = (*capture_Qimage).size();
        qDebug() << "CaptureWork" << QThread::currentThreadId() << QThread::currentThread();
        emit signal_messImage(*capture_Qimage); // 读取到图像并转成Qimage格式
    }
    // emit workFinished();                                                                   // 发出退出工作循环信号
    qDebug() << "CurrentThread" << QThread::currentThreadId() << QThread::currentThread(); // 当前线程ID
}