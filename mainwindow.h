#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <MvCamera.h>
#include <QDebug>
#include <QDir>
#include <QMainWindow>
#include <QMessageBox>
#include <QThread>
#include <capturework.h>
#include <windows.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int m_nCurCameraIndex; // 当前设备索引号

    bool IsMono(MvGvspPixelType enType); // 是否是灰度图

signals:
    void startCaptureThread();

private slots:
    void slot_displayImage(QImage myImage);

    void stopCaptureThread();

    void on_pushButton_findDev_clicked();

    void on_pushButton_openDev_clicked();

    void on_pushButton_closeDev_clicked();

    void on_pushButton_captureImg_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::MainWindow *ui;

    MvCamera *camera1 = NULL; // 相机对象

    bool IsDeviceOpen;     // 设备是否打开标志
    bool IsDeviceGrabbing; // 设备是否处于采图状态
    bool IsShowing;        // 设备是否处于显示状态
    int m_nTriggerMode;    // 触发模式
    int m_nTriggerSource;  // 触发源
    float m_fExposureTime;

    MV_CC_DEVICE_INFO_LIST camDevList;      // 在线设备列表
    QThread *Capture_Thread = NULL;         // 采图线程
    QThread *Process_Thread = NULL;         // 处理线程
    CaptureWork *Capture_ThreadWork = NULL; // 采图线程中工作的类对象
    cv::Mat *camImage = NULL;               // 采回的图像

    CRITICAL_SECTION m_hSaveImageMux; // 用于保存图片时的互斥量
    unsigned char *m_pSaveImageBuf;   // 用于保存图片的缓存
    unsigned int m_nSaveImageBufSize; // 保存图片的缓存大小
    HWND m_hwndDisplay;               // ch:显示句柄 | en:Display window
    MV_FRAME_OUT_INFO_EX m_stImageInfo;
    void *m_hGrabThread; // 抓图线程

    cv::Mat SrcImage;
    cv::Mat GrayImage;
    cv::Mat ThresImage;

    cv::Mat *capImage = NULL; // 采回的图像
    QImage qImage_show;

    void SetTriggerSource(void); //设置触发模式
    void SetTriggerMode(void);
    void SetExposureTime(void);

    void initSomeVars();
};
#endif // MAINWINDOW_H
