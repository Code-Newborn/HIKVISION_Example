#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initSomeVars(); // 初始化
}

MainWindow::~MainWindow()
{
    delete ui;
    delete capImage;
    if (camera1)
    {
        camera1->StopGrabbing(); // 停止抓图
        IsDeviceGrabbing = false;
        IsShowing = false; // 停止刷新显示
        camera1->Close();
        IsDeviceOpen = false; // 设备关闭
        delete camera1;
        camera1 = NULL;
        qDebug("停止并销毁相机控制类对象");
    }
    Capture_ThreadWork->stopWork();
    if (Capture_Thread->isRunning())
    {
        Capture_Thread->quit();
        Capture_Thread->wait();
        delete Capture_Thread;
        qDebug("退出并销毁抓图线程");
    }
    qDebug("退出窗口");
}

void MainWindow::initSomeVars()
{
    IsDeviceOpen = false;     // 初始化设备关闭标志
    IsDeviceGrabbing = false; // 初始化设备停止采图标志
    IsShowing = false;
    m_pSaveImageBuf = NULL;

    //生成保存图片的文件夹
    QString imagePath = QDir::currentPath() + "/myImage/";
    QDir dstDir(imagePath);
    if (!dstDir.exists())
    {
        if (!dstDir.mkdir(imagePath))
        {
            qDebug() << "创建Image文件夹失败！";
            // return;
        }
    }

    capImage = new cv::Mat(); // 图像指针对象

    Capture_Thread = new QThread();         // 创建捕获线程
    Capture_ThreadWork = new CaptureWork(); // 创建捕获线程中工作的类对象

    // 工作的类对象移入线程中
    Capture_ThreadWork->moveToThread(Capture_Thread);

    connect(this, SIGNAL(startCaptureThread()), Capture_ThreadWork, SLOT(Work()));                        // 【信号】开启抓图线程 【槽函数】抓图线程中的抓图工作对象工作
    connect(Capture_ThreadWork, SIGNAL(signal_messImage(QImage)), this, SLOT(slot_displayImage(QImage))); // 【信号】抓取到图像 【槽函数】主窗口中显示

    Capture_Thread->start(); // 线程退出后需开启
}

void MainWindow::on_pushButton_findDev_clicked()
{
    ui->comboBox_deviceList->clear();
    memset(&camDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    int nRet = MvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &camDevList); // 检索设备
    if (nRet != MV_OK)
    {
        return;
    }

    // 组合下拉框中添加设备名
    for (unsigned int i = 0; i < camDevList.nDeviceNum; i++)
    {
        MV_CC_DEVICE_INFO *pDeviceInfo = camDevList.pDeviceInfo[i];
        QString strModelName = "";
        if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
        {
            strModelName = (char *)pDeviceInfo->SpecialInfo.stUsb3VInfo.chModelName;
        }
        else if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
        {
            strModelName = (char *)pDeviceInfo->SpecialInfo.stGigEInfo.chModelName;
        }
        else
        {
            QMessageBox::warning(this, "警告", "非GIGE和USB接口设备！");
            return;
        }
        qDebug() << "strModelName:" << strModelName;
        ui->comboBox_deviceList->addItem(strModelName);
    }
}

void MainWindow::on_pushButton_openDev_clicked()
{

    if (IsShowing == false)
    {
        IsShowing = true; // 恢复刷新显示
    }

    // 打开状态
    if (IsDeviceOpen == true)
    {
        return; // 设备已经打开并且正在采图
    }

    QString deviceModel = ui->comboBox_deviceList->currentText();
    if (deviceModel == "")
    {
        QMessageBox::warning(this, "警告", "请选择设备！");
        return;
    }

    int deviceIndex_inCombox = ui->comboBox_deviceList->currentIndex();   // 当前选择的索引
    camera1 = new MvCamera(camDevList.pDeviceInfo[deviceIndex_inCombox]); // 根据下拉框选择的相机，实例化相机操作类
    int nRet = camera1->Open();                                           // 打开设备
    if (MV_OK != nRet)                                                    // 设备打开失败
    {
        delete camera1;
        camera1 = NULL;
        IsDeviceOpen = false; // 设备关闭
        QMessageBox::warning(this, "警告", "打开设备失败！");
        qDebug("Open Device failed!, nRet[%#x]\r\n", nRet);
        return;
    }
    else
    {
        IsDeviceOpen = true; // 设备打开
        qDebug("Open Device Successfully!");
    }

    if (true == IsDeviceOpen) // 设置触发
    {
        qDebug("Set Trigger!");
        m_nTriggerMode = MV_TRIGGER_MODE_ON;
        m_nTriggerSource = MV_TRIGGER_SOURCE_SOFTWARE;
        m_fExposureTime = 5000.0;
        SetTriggerMode();
        SetTriggerSource();
        SetExposureTime();
    }

    if (camera1)
    {
        // 启动线程
        nRet = camera1->StartGrabbing(); // 开启抓图
        if (MV_OK != nRet)
        {
            IsDeviceGrabbing = false;
            qDebug("Start grabbing fail!, nRet[%#x]\r\n", nRet);
        }
        else
        {
            IsDeviceGrabbing = true;
            qDebug("Start grabbing successfully!");
        }

        Capture_ThreadWork->getCameraPtr(camera1); // 传递相机对象到抓图线程中
        Capture_ThreadWork->getImagePtr(capImage); // 传递图像指针到抓图线程中

        if (!IsDeviceOpen) // 设备未打开
        {
            Capture_ThreadWork->stopWork();
            QMessageBox::warning(this, "警告", "采集失败,请打开设备！");
            return;
        }
        Capture_Thread->start(); // 线程退出后需开启
        Capture_ThreadWork->startWork();
        emit startCaptureThread();
    }
}

void MainWindow::slot_displayImage(QImage myImage)
{
    qDebug() << "MainWindow" << QThread::currentThreadId() << QThread::currentThread();

    if (IsShowing) //显示图像
    {
        QSize label_size = ui->label_show->size();
        myImage = (myImage).scaledToHeight(ui->label_show->contentsRect().height(), Qt::TransformationMode::SmoothTransformation);
        ui->label_show->setPixmap(QPixmap::fromImage(myImage));
    }
}

void MainWindow::SetExposureTime(void)
{
    int nRet = MV_OK;
    if (camera1)
    {
        nRet = camera1->SetFloatValue("ExposureTime", m_fExposureTime);
        if (MV_OK != nRet)
        {
            qDebug("Set Trigger source fail!, TriggerSource[%.2f], nRet[%#x]\r\n",
                   m_fExposureTime, nRet);
        }
    }
}

// ch:设置触发源 | en:Set trigger source
void MainWindow::SetTriggerSource(void)
{
    int nRet = MV_OK;

    if (camera1)
    {
        nRet = camera1->SetEnumValue("TriggerSource", m_nTriggerSource);
        if (MV_OK != nRet)
        {
            qDebug("Set Trigger source fail!, TriggerSource[%d], nRet[%#x]\r\n",
                   m_nTriggerSource, nRet);
        }
    }
}

// ch:设置触发模式 | en:Set trigger mode
void MainWindow::SetTriggerMode(void)
{
    int nRet = MV_OK;
    if (camera1)
    {
        nRet = camera1->SetEnumValue("TriggerMode", m_nTriggerMode);
        if (MV_OK != nRet)
        {
            qDebug("Set Trigger mode fail!, TriggerMode[%d], nRet[%#x]\r\n",
                   m_nTriggerMode, nRet);
        }
    }
}

bool MainWindow::IsMono(MvGvspPixelType enType)
{
    switch (enType)
    {
    case PixelType_Gvsp_Mono8:
    case PixelType_Gvsp_Mono10:
    case PixelType_Gvsp_Mono10_Packed:
    case PixelType_Gvsp_Mono12:
    case PixelType_Gvsp_Mono12_Packed:
        return true;
    default:
        return false;
    }
}

void MainWindow::stopCaptureThread()
{
    // if (Capture_Thread->isRunning())
    // {
    //     Capture_Thread->quit();
    //     Capture_Thread->wait();
    // }
    // qDebug("线程退出");
}

void MainWindow::on_pushButton_closeDev_clicked()
{
    Capture_ThreadWork->stopWork();

    if (camera1) // 关闭相机 销毁相机操作对象
    {
        camera1->StopGrabbing();
        IsDeviceGrabbing = false; // 停止抓图
        IsShowing = false;        // 停止刷新显示
        camera1->Close();
        IsDeviceOpen = false; // 设备关闭
        delete camera1;
        camera1 = NULL;
        qDebug("停止并销毁相机控制类对象");
    }

    // 退出抓图线程
    if (Capture_Thread->isRunning())
    {
        Capture_Thread->quit();
        Capture_Thread->wait();
        ui->label_show->clear(); //清除图片
        qDebug("退出抓图线程");
    }
}

void MainWindow::on_pushButton_captureImg_clicked() // 单次取图刷新
{
    IsShowing = false; // 取消显示刷新
    if (IsDeviceOpen == true && IsDeviceGrabbing == true)
    {
        if (capImage->channels() > 1)
        {
            qImage_show = QImage((const unsigned char *)(capImage->data), capImage->cols, capImage->rows, QImage::Format_RGB888);
        }
        else
        {
            qImage_show = QImage((const unsigned char *)(capImage->data), capImage->cols, capImage->rows, QImage::Format_Indexed8);
        }

        qImage_show = (qImage_show).scaledToHeight(ui->label_show->contentsRect().height(), Qt::TransformationMode::SmoothTransformation);
        ui->label_show->setPixmap(QPixmap::fromImage(qImage_show));
    }
    else
    {
        ui->label_show->clear();
    }
}

void MainWindow::on_pushButton_exit_clicked()
{
    on_pushButton_closeDev_clicked(); // 关闭设备
    QApplication::quit();
}
