#include "MvCamera.h"
#include "QDebug"

MvCamera::MvCamera(MV_CC_DEVICE_INFO *pstDevInfo)
{
    this->m_hDevHandle = MV_NULL;
    this->pstDeviceInfo = pstDevInfo;
}

MvCamera::~MvCamera()
{
    if (m_hDevHandle)
    {
        MV_CC_DestroyHandle(m_hDevHandle);// 销毁相机句柄
        m_hDevHandle = MV_NULL;
    }
}

int MvCamera::GetSDKVersion()
{
    return MV_CC_GetSDKVersion();
}

int MvCamera::EnumDevices(unsigned int nTLayerType, MV_CC_DEVICE_INFO_LIST *pstDevList)
{
    return MV_CC_EnumDevices(nTLayerType, pstDevList);
}

bool MvCamera::IsDeviceAccessible(MV_CC_DEVICE_INFO *pstDevInfo, unsigned int nAccessMode)
{
    return MV_CC_IsDeviceAccessible(pstDevInfo, nAccessMode);
}

int MvCamera::Open()
{
    if (MV_NULL == pstDeviceInfo)
    {
        return MV_E_PARAMETER;
    }
    if (m_hDevHandle)
    {
        return MV_E_CALLORDER;
    }
    int nRet = MV_CC_CreateHandle(&this->m_hDevHandle, pstDeviceInfo);
    if (MV_OK != nRet)
    {
        qDebug() << "Create Handle Failed!";
        return nRet;
    }
    nRet = MV_CC_OpenDevice(m_hDevHandle);
    if (MV_OK != nRet)
    {
        qDebug("Open Device Handle Failed! nRet[%#x]\r\n", nRet);
        return nRet;
    }
    return nRet;
}

int MvCamera::Close()
{
    if (MV_NULL == m_hDevHandle)
    {
        return MV_E_HANDLE;
    }
    MV_CC_CloseDevice(m_hDevHandle);              // 关闭设备
    int nRet = MV_CC_DestroyHandle(m_hDevHandle); // 销毁设备操作句柄
    m_hDevHandle = MV_NULL;
    return nRet;
}

bool MvCamera::IsDeviceConnected()
{
    return MV_CC_IsDeviceConnected(m_hDevHandle);
}

int MvCamera::StartGrabbing() { return MV_CC_StartGrabbing(m_hDevHandle); }

int MvCamera::StopGrabbing() { return MV_CC_StopGrabbing(m_hDevHandle); }

int MvCamera::GetImageBuffer(MV_FRAME_OUT *pFrame, int nMsec)
{
    return MV_CC_GetImageBuffer(m_hDevHandle, pFrame, nMsec);
}

int MvCamera::FreeImageBuffer(MV_FRAME_OUT *pFrame)
{
    return MV_CC_FreeImageBuffer(m_hDevHandle, pFrame);
}

int MvCamera::DisplayOneFrame(MV_DISPLAY_FRAME_INFO *pDisplayInfo)
{
    return MV_CC_DisplayOneFrame(m_hDevHandle, pDisplayInfo);
}

int MvCamera::SetImageNodeNum(unsigned int nNum)
{
    return MV_CC_SetImageNodeNum(m_hDevHandle, nNum);
}

int MvCamera::GetDeviceInfo(MV_CC_DEVICE_INFO *pstDevInfo)
{
    return MV_CC_GetDeviceInfo(m_hDevHandle, pstDevInfo);
}

int MvCamera::GetIntValue(IN const char *strKey, OUT MVCC_INTVALUE_EX *pIntValue)
{
    return MV_CC_GetIntValueEx(m_hDevHandle, strKey, pIntValue);
}

int MvCamera::SetIntValue(IN const char *strKey, IN int64_t nValue)
{
    return MV_CC_SetIntValueEx(m_hDevHandle, strKey, nValue);
}

int MvCamera::GetEnumValue(IN const char *strKey, OUT MVCC_ENUMVALUE *pEnumValue)
{
    return MV_CC_GetEnumValue(m_hDevHandle, strKey, pEnumValue);
}

int MvCamera::SetEnumValue(IN const char *strKey, IN unsigned int nValue)
{
    return MV_CC_SetEnumValue(m_hDevHandle, strKey, nValue);
}

int MvCamera::SetEnumValueByString(IN const char *strKey, IN const char *sValue)
{
    return MV_CC_SetEnumValueByString(m_hDevHandle, strKey, sValue);
}

int MvCamera::GetEnumEntrySymbolic(IN const char *strKey, IN MVCC_ENUMENTRY *pstEnumEntry)
{
    return MV_CC_GetEnumEntrySymbolic(m_hDevHandle, strKey, pstEnumEntry);
}

int MvCamera::GetFloatValue(IN const char *strKey, OUT MVCC_FLOATVALUE *pFloatValue)
{
    return MV_CC_GetFloatValue(m_hDevHandle, strKey, pFloatValue);
}

int MvCamera::SetFloatValue(IN const char *strKey, IN float fValue)
{
    return MV_CC_SetFloatValue(m_hDevHandle, strKey, fValue);
}

int MvCamera::GetBoolValue(IN const char *strKey, OUT bool *pbValue)
{
    return MV_CC_GetBoolValue(m_hDevHandle, strKey, pbValue);
}

int MvCamera::SetBoolValue(IN const char *strKey, IN bool bValue)
{
    return MV_CC_SetBoolValue(m_hDevHandle, strKey, bValue);
}

int MvCamera::GetStringValue(IN const char *strKey, MVCC_STRINGVALUE *pStringValue)
{
    return MV_CC_GetStringValue(m_hDevHandle, strKey, pStringValue);
}

int MvCamera::SetStringValue(IN const char *strKey, IN const char *strValue)
{
    return MV_CC_SetStringValue(m_hDevHandle, strKey, strValue);
}

int MvCamera::CommandExecute(IN const char *strKey)
{
    return MV_CC_SetCommandValue(m_hDevHandle, strKey);
}

int MvCamera::RegisterImageCallBack(
    void(__stdcall *cbOutput)(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo, void *pUser),
    void *pUser)
{
    return MV_CC_RegisterImageCallBackEx(m_hDevHandle, cbOutput, pUser);
}

int MvCamera::RegisterExceptionCallBack(
    void(__stdcall *cbException)(unsigned int nMsgType, void *pUser),
    void *pUser)
{
    return MV_CC_RegisterExceptionCallBack(m_hDevHandle, cbException, pUser);
}

int MvCamera::RegisterEventCallBack(
    const char *pEventName,
    void(__stdcall *cbEvent)(MV_EVENT_OUT_INFO *pEventInfo, void *pUser),
    void *pUser)
{
    return MV_CC_RegisterEventCallBackEx(m_hDevHandle, pEventName, cbEvent, pUser);
}

int MvCamera::ConvertPixelType(MV_CC_PIXEL_CONVERT_PARAM *pstCvtParam)
{
    return MV_CC_ConvertPixelType(m_hDevHandle, pstCvtParam);
}

int MvCamera::SaveImage(MV_SAVE_IMAGE_PARAM_EX *pstParam)
{
    return MV_CC_SaveImageEx2(m_hDevHandle, pstParam);
}

int MvCamera::SaveImageToFile(MV_SAVE_IMG_TO_FILE_PARAM *pstSaveFileParam)
{
    return MV_CC_SaveImageToFile(m_hDevHandle, pstSaveFileParam);
}

int MvCamera::DrawCircle(MVCC_CIRCLE_INFO *pCircleInfo)
{
    return MV_CC_DrawCircle(m_hDevHandle, pCircleInfo);
}

int MvCamera::DrawLines(MVCC_LINES_INFO *pLinesInfo)
{
    return MV_CC_DrawLines(m_hDevHandle, pLinesInfo);
}

bool MvCamera::IsMono(MvGvspPixelType enType)
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

bool MvCamera::IsColor(MvGvspPixelType enType)
{
    switch (enType)
    {
    case PixelType_Gvsp_BGR8_Packed:
    case PixelType_Gvsp_YUV422_Packed:
    case PixelType_Gvsp_YUV422_YUYV_Packed:
    case PixelType_Gvsp_BayerGR8:
    case PixelType_Gvsp_BayerRG8:
    case PixelType_Gvsp_BayerGB8:
    case PixelType_Gvsp_BayerBG8:
    case PixelType_Gvsp_BayerGB10:
    case PixelType_Gvsp_BayerGB10_Packed:
    case PixelType_Gvsp_BayerBG10:
    case PixelType_Gvsp_BayerBG10_Packed:
    case PixelType_Gvsp_BayerRG10:
    case PixelType_Gvsp_BayerRG10_Packed:
    case PixelType_Gvsp_BayerGR10:
    case PixelType_Gvsp_BayerGR10_Packed:
    case PixelType_Gvsp_BayerGB12:
    case PixelType_Gvsp_BayerGB12_Packed:
    case PixelType_Gvsp_BayerBG12:
    case PixelType_Gvsp_BayerBG12_Packed:
    case PixelType_Gvsp_BayerRG12:
    case PixelType_Gvsp_BayerRG12_Packed:
    case PixelType_Gvsp_BayerGR12:
    case PixelType_Gvsp_BayerGR12_Packed:
        return true;
    default:
        return false;
    }
}

int MvCamera::ReadBuffer(cv::Mat &image)
{
    cv::Mat *getImage = new cv::Mat();
    unsigned int nRecvBufSize = 0;
    MVCC_INTVALUE stParam;
    memset(&stParam, 0, sizeof(MVCC_INTVALUE));
    int tempValue = MV_CC_GetIntValue(m_hDevHandle, "PayloadSize", &stParam);
    if (tempValue != 0)
    {
        return -1;
    }
    nRecvBufSize = stParam.nCurValue;
    unsigned char *pDate;
    pDate = (unsigned char *)malloc(nRecvBufSize);

    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    tempValue = MV_CC_GetOneFrameTimeout(m_hDevHandle, pDate, nRecvBufSize,
                                         &stImageInfo, 700);
    if (tempValue != 0)
    {
        return -1;
    }
    m_nBufSizeForSaveImage = stImageInfo.nWidth * stImageInfo.nHeight * 3 + 2048;
    unsigned char *m_pBufForSaveImage;
    m_pBufForSaveImage = (unsigned char *)malloc(m_nBufSizeForSaveImage);

    bool isMono;
    switch (stImageInfo.enPixelType)
    {
    case PixelType_Gvsp_Mono8:
    case PixelType_Gvsp_Mono10:
    case PixelType_Gvsp_Mono10_Packed:
    case PixelType_Gvsp_Mono12:
    case PixelType_Gvsp_Mono12_Packed:
        isMono = true;
        break;
    default:
        isMono = false;
        break;
    }
    if (isMono)
    {
        *getImage =
            cv::Mat(stImageInfo.nHeight, stImageInfo.nWidth, CV_8UC1, pDate);
        // imwrite("d:\\测试opencv_Mono.tif", image);
    }
    else
    {
        //转换图像格式为BGR8
        MV_CC_PIXEL_CONVERT_PARAM stConvertParam = {0};
        memset(&stConvertParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));
        stConvertParam.nWidth = stImageInfo.nWidth;   // ch:图像宽 | en:image width
        stConvertParam.nHeight = stImageInfo.nHeight; // ch:图像高 | en:image height
        // stConvertParam.pSrcData = m_pBufForDriver; //ch:输入数据缓存 | en:input
        // data buffer
        stConvertParam.pSrcData = pDate; // ch:输入数据缓存 | en:input data buffer
        stConvertParam.nSrcDataLen =
            stImageInfo.nFrameLen; // ch:输入数据大小 | en:input data size
        stConvertParam.enSrcPixelType =
            stImageInfo.enPixelType; // ch:输入像素格式 | en:input pixel format
        stConvertParam.enDstPixelType =
            PixelType_Gvsp_BGR8_Packed; // ch:输出像素格式 | en:output pixel format
                                        // 适用于OPENCV的图像格式
        // stConvertParam.enDstPixelType = PixelType_Gvsp_RGB8_Packed;
        // //ch:输出像素格式 | en:output pixel format
        stConvertParam.pDstBuffer =
            m_pBufForSaveImage; // ch:输出数据缓存 | en:output data buffer
        stConvertParam.nDstBufferSize =
            m_nBufSizeForSaveImage; // ch:输出缓存大小 | en:output buffer size
        MV_CC_ConvertPixelType(m_hDevHandle, &stConvertParam);

        *getImage = cv::Mat(stImageInfo.nHeight, stImageInfo.nWidth, CV_8UC3,
                            m_pBufForSaveImage);
        // imwrite("d:\\测试opencv_Color.tif", image);
    }
    (*getImage).copyTo(image);
    (*getImage).release();
    free(pDate);
    free(m_pBufForSaveImage);
    return 0;
}