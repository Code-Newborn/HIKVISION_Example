#ifndef MVCAMERA_H
#define MVCAMERA_H

#include <QQueue>
#include "HIKVISION/Includes/MvCameraControl.h"
#include "opencv2/opencv.hpp"

#ifndef MV_NULL
#define MV_NULL 0
#endif

class MvCamera
{
public:
  MvCamera(MV_CC_DEVICE_INFO *pstDevInfo);
  ~MvCamera();

  /// @brief      获取支持的传输层
  /// @msg        获取支持的传输层
  /// @return     {int} SDK版本号
  static int GetSDKVersion();

  /// @brief      枚举设备
  /// @msg        枚举设备
  /// @param      {unsigned int} nTLayerType 传输层类型：包括MV_GIGE_DEVICE、MV_1394_DEVICE、MV_USB_DEVICE、MV_CAMERALINK_DEVICE；MV_GIGE_DEVICE
  /// @param      {MV_CC_DEVICE_INFO_LIST} *pstDevList 设备列表
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  static int EnumDevices(unsigned int nTLayerType, MV_CC_DEVICE_INFO_LIST *pstDevList);

  /// @brief      判断相机是否可访问
  /// @msg
  /// @param      {MV_CC_DEVICE_INFO} *pstDevInfo 设备信息结构体
  /// @param      {unsigned int} nAccessMode 访问权限
  /// @return     {bool} 可达，返回true；不可达，返回false
  static bool IsDeviceAccessible(MV_CC_DEVICE_INFO *pstDevInfo, unsigned int nAccessMode);

  /// @brief      打开相机设备
  /// @msg
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int Open();

  /// @brief      关闭相机设备
  /// @msg
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int Close();

  /// @brief      判断设备是否连接
  /// @msg
  /// @return     {bool} 设备处于连接状态，返回true；没连接或失去连接，返回false
  bool IsDeviceConnected();

  /// @brief      开启抓图
  /// @msg
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int StartGrabbing();

  /// @brief      停止抓图
  /// @msg
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int StopGrabbing();

  /// @brief      使用内部缓存获取一帧图片（与MV_CC_Display不能同时使用）
  /// @msg
  /// @param      {MV_FRAME_OUT} *pFrame 图像数据和图像信息
  /// @param      {int} nMsec 等待超时时间，输入INFINITE时表示无限等待，直到收到一帧数据或者停止取流
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int GetImageBuffer(MV_FRAME_OUT *pFrame, int nMsec);

  /// @brief      释放图像缓存(此接口用于释放不再使用的图像缓存，与MV_CC_GetImageBuffer配套使用)
  /// @msg
  /// @param      {MV_FRAME_OUT} *pFrame 图像数据和图像信息
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int FreeImageBuffer(MV_FRAME_OUT *pFrame);

  /// @brief      显示一帧图像
  /// @msg
  /// @param      {MV_DISPLAY_FRAME_INFO} *pDisplayInfo 图像信息
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int DisplayOneFrame(MV_DISPLAY_FRAME_INFO *pDisplayInfo);

  /// @brief      设置SDK内部图像缓存节点个数，大于等于1，在抓图前调用
  /// @msg
  /// @param      {unsigned int} nNum 缓存节点个数
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int SetImageNodeNum(unsigned int nNum);

  /// @brief      获取设备信息，取流之前调用
  /// @msg
  /// @param      {MV_CC_DEVICE_INFO} *pstDevInfo 设备信息结构体
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int GetDeviceInfo(MV_CC_DEVICE_INFO *pstDevInfo);

  /// @brief      获取Integer属性值
  /// @msg        详细内容参考SDK安装目录下的MvCameraNode.xlsx 文件
  /// @param      {INchar} *strKey 属性键值，如获取宽度信息则为"Width"
  /// @param      {OUT MVCC_INTVALUE_EX} *pIntValue 返回给调用者有关设备属性结构体指针
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int GetIntValue(IN const char *strKey, OUT MVCC_INTVALUE_EX *pIntValue);

  /// @brief      设置Integer属性值
  /// @msg        详细内容参考SDK安装目录下的MvCameraNode.xlsx 文件
  /// @param      {INchar} *strKey 属性键值，如设置宽度信息则为"Width"
  /// @param      {IN int64_t} nValue 想要设置的设备的属性值
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int SetIntValue(IN const char *strKey, IN int64_t nValue);

  /// @brief      获取Enum型属性值
  /// @msg        详细内容参考SDK安装目录下的MvCameraNode.xlsx 文件
  /// @param      {INchar} *strKey 属性键值，如获取像素格式信息则为"PixelFormat"
  /// @param      {OUT MVCC_ENUMVALUE} *pEnumValue 返回给调用者有关设备属性结构体指针
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int GetEnumValue(IN const char *strKey, OUT MVCC_ENUMVALUE *pEnumValue);

  /// @brief      设置Enum型属性值
  /// @msg        详细内容参考SDK安装目录下的MvCameraNode.xlsx 文件
  /// @param      {INchar} *strKey 属性键值，如设置像素格式信息则为"PixelFormat"
  /// @param      {IN unsigned int} nValue 想要获取的设备的属性符号
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int SetEnumValue(IN const char *strKey, IN unsigned int nValue);

  /// @brief      获取Enum型属性值
  /// @msg        详细内容参考SDK安装目录下的MvCameraNode.xlsx 文件
  /// @param      {INchar} *strKey 属性键值，如获取像素格式信息则为"PixelFormat"
  /// @param      {IN MVCC_ENUMENTRY} *pstEnumEntry 想要获取的设备的属性符号
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int GetEnumEntrySymbolic(IN const char *strKey, IN MVCC_ENUMENTRY *pstEnumEntry);

  /// @brief      设置Enum型属性值
  /// @msg        详细内容参考SDK安装目录下的MvCameraNode.xlsx 文件
  /// @param      {INchar} *strKey 属性键值，如获取像素格式信息则为"PixelFormat"
  /// @param      {INchar} *sValue 想要设置的设备的属性字符串
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int SetEnumValueByString(IN const char *strKey, IN const char *sValue);

  /// @brief      获取Float属性值
  /// @msg        详细内容参考SDK安装目录下的MvCameraNode.xlsx 文件
  /// @param      {INchar} *strKey 属性键值
  /// @param      {OUT MVCC_FLOATVALUE} *pFloatValue 返回给调用者有关设备属性结构体指针
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int GetFloatValue(IN const char *strKey, OUT MVCC_FLOATVALUE *pFloatValue);

  /// @brief      设置Float属性值
  /// @msg        详细内容参考SDK安装目录下的MvCameraNode.xlsx 文件
  /// @param      {INchar} *strKey  属性键值
  /// @param      {IN float} fValue 想要设置的设备的属性值
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int SetFloatValue(IN const char *strKey, IN float fValue);

  /// @brief      获取Boolean属性值
  /// @msg        详细内容参考SDK安装目录下的MvCameraNode.xlsx 文件
  /// @param      {INchar} *strKey 属性键值
  /// @param      {OUT bool} *pbValue 返回给调用者有关设备属性值
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int GetBoolValue(IN const char *strKey, OUT bool *pbValue);

  /// @brief      设置Boolean属性值
  /// @msg        详细内容参考SDK安装目录下的MvCameraNode.xlsx 文件
  /// @param      {INchar} *strKey 属性键值
  /// @param      {IN bool} bValue 想要设置的设备的属性值
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int SetBoolValue(IN const char *strKey, IN bool bValue);

  /// @brief      获取String属性值
  /// @msg        详细内容参考SDK安装目录下的MvCameraNode.xlsx 文件
  /// @param      {INchar} *strKey 属性键值
  /// @param      {MVCC_STRINGVALUE} *pStringValue 返回给调用者有关设备属性结构体指针
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int GetStringValue(IN const char *strKey, MVCC_STRINGVALUE *pStringValue);

  /// @brief      设置String型属性值
  /// @msg        详细内容参考SDK安装目录下的MvCameraNode.xlsx 文件
  /// @param      {INchar} *strKey 属性键值
  /// @param      {INchar} *strValue 想要设置的设备的属性值
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int SetStringValue(IN const char *strKey, IN const char *strValue);

  /// @brief      设置Command型属性值
  /// @msg        连接设备之后调用该接口可以设置指定的Command类型节点。strKey取值可以参考XML节点参数类型列表，表格里面数据类型为“ICommand”的节点都可以通过该接口设置，strKey参数取值对应列表里面的“名称”一列。
  /// @param      {INchar} *strKey 属性键值
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int CommandExecute(IN const char *strKey);

  /// @brief      注册图像数据回调
  /// @msg        通过该接口可以设置图像数据回调函数，在MV_CC_CreateHandle之后即可调用。
  ///             调用MV_CC_RegisterImageCallBackEx设置图像数据回调函数，然后调用MV_CC_StartGrabbing开始采集，采集的图像数据在设置的回调函数中返回。
  /// @param cbOutput {*} 回调函数指针
  /// @param pUser    {*} 用户自定义变量
  /// @return         {int} 成功，返回MV_OK；错误，返回错误码
  int RegisterImageCallBack(
      void(__stdcall *cbOutput)(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo, void *pUser),
      void *pUser);

  /// @brief      注册异常消息回调，在打开设备之后调用
  /// @msg        该接口需要在MV_CC_OpenDevice打开设备之后调用。设备异常断开连接后可以在回调里面获取到异常消息，GigE设备掉线之后需要先调用MV_CC_CloseDevice接口关闭设备，再调用MV_CC_OpenDevice接口重新打开设备。
  /// @param cbException {*} 异常回调函数指针
  /// @param pUser       {*} 用户自定义变量
  /// @return            {int} 成功，返回MV_OK；错误，返回错误码
  int RegisterExceptionCallBack(
      void(__stdcall *cbException)(unsigned int nMsgType, void *pUser),
      void *pUser);

  /// @brief      注册单个事件回调，在打开设备之后调用
  /// @msg        通过该接口设置事件回调，可以在回调函数里面获取采集、曝光等事件信息。
  /// @param pEventName  {char*} 事件名称
  /// @param cbEvent     {*} 事件回调函数指针
  /// @param pUser       {*} 用户自定义变量
  /// @return            {int} 成功，返回MV_OK；错误，返回错误码
  int RegisterEventCallBack(
      const char *pEventName,
      void(__stdcall *cbEvent)(MV_EVENT_OUT_INFO *pEventInfo, void *pUser),
      void *pUser);

  /// @brief      像素格式转换
  /// @msg        通过将接口可以将从设备采集到的原始图像数据转换成用户所需的像素格式并存放在指定内存中。
  ///             该接口调用无接口顺序要求，有图像源数据就可以进行转换，可以先调用MV_CC_GetOneFrameTimeout或者MV_CC_RegisterImageCallBackEx设置回调函数，获取一帧图像数据，然后再通过该接口转换格式。
  ///             如果设备当前采集图像是JPEG压缩的格式，则不支持调用该接口进行转换。
  /// @param      {MV_CC_PIXEL_CONVERT_PARAM} *pstCvtParam 像素格式转换参数结构体
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int ConvertPixelType(MV_CC_PIXEL_CONVERT_PARAM *pstCvtParam);

  /// @brief      保存图片，支持Bmp和Jpeg.
  /// @msg        通过将接口可以将从设备采集到的原始图像数据转换成JPEG或者BMP等格式并存放在指定内存中，然后用户可以将转换之后的数据直接保存成图片文件。
  ///             该接口调用无接口顺序要求，有图像源数据就可以进行转换，可以先调用MV_CC_GetOneFrameTimeout或者MV_CC_RegisterImageCallBackEx设置回调函数，获取一帧图像数据，然后再通过该接口转换格式。
  /// @param      {MV_SAVE_IMAGE_PARAM_EX} *pstParam 保存图片参数结构体
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int SaveImage(MV_SAVE_IMAGE_PARAM_EX *pstParam);

  /// @brief      保存图像到文件
  /// @msg        该接口支持BMP/JPEG/PNG/TIFF
  /// @param      {MV_SAVE_IMG_TO_FILE_PARAM} *pstParam 保存图片文件参数结构体
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int SaveImageToFile(MV_SAVE_IMG_TO_FILE_PARAM *pstParam);

  /// @brief      在图像上绘制圆形辅助线
  /// @msg
  /// @param      {MVCC_CIRCLE_INFO} *pCircleInfo 圆形辅助线的信息
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int DrawCircle(MVCC_CIRCLE_INFO *pCircleInfo);

  /// @brief      在图像上绘制线条
  /// @msg
  /// @param      {MVCC_LINES_INFO} *pLinesInfo 线条辅助线信息
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int DrawLines(MVCC_LINES_INFO *pLinesInfo);

  /// @brief      读取图像缓存
  /// @msg
  /// @param      {Mat} &image 读取到的Mat格式图像数据
  /// @return     {int} 成功，返回MV_OK；错误，返回错误码
  int ReadBuffer(cv::Mat &image);

  bool IsMono(MvGvspPixelType enType);
  bool IsColor(MvGvspPixelType enType);

public:
  void *m_hDevHandle;         // 相机句柄
  cv::Mat currentImg;         // 相机当前图像
  QQueue<cv::Mat> imageQueue; // 帧缓冲区队列
  int bufferSize;

private:
  unsigned int m_nBufSizeForSaveImage; // 用于保存图像的缓存大小
  MV_CC_DEVICE_INFO *pstDeviceInfo;    // 设备信息
};

#endif // MVCAMERA_H
