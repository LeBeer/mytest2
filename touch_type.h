#ifndef TOUCH_TYPE
#define TOUCH_TYPE

#define BYTE(x) (BYTE)(x)

typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       ULONG;
typedef unsigned short      INT16U;
typedef unsigned char       INT8U;
typedef unsigned int        INT32U;

typedef struct _IROPT_TOUCH_AREA
{
    INT16U   MinX; //起始X位置
    INT16U   MinY; //起始Y位置
    INT16U   MaxX; //结束X位置
    INT16U   MaxY; //结束Y位置
}IROPT_TOUCH_AREA,*PIROPT_TOUCH_AREA;

typedef struct _IROPT_DEVICE_CONFIG
{
      INT8U MonitorID; //显示器ID号
      IROPT_TOUCH_AREA  DefaultTouchArea; //默认校正区域
      INT8U CalibratStatus; //校正的状态，0表示未校正过，1表示已经校正过
      IROPT_TOUCH_AREA ExtendTouchArea[3]; //校正区域1, 区域2, 区域3的范围
      INT8U Reserved[24]; //保留字节
}IROPT_DEVICE_CONFIG,*PIROPT_DEVICE_CONFIG;

typedef struct _IROPT_DRIVER_CONFIG
{
    BYTE          Mode; 		//触摸屏工作模式
                             //0x50  鼠标落笔模式 0x51:鼠标抬笔模式 0x52：鼠标缺省模式
                             //0x60  触摸落笔模式 0x61: 触摸抬笔模式 0x62：触摸缺省模式
    BYTE          EnableTouch; 		//启用、禁用触摸
                                 //0x00  表示禁用，表示启用
    BYTE          EnableRightClick; //启用、禁用右键功能
                                //:0x00  表示禁用，表示启用
    BYTE          CalibrateType; 	//设置校准类型(4/X)
                                 //0表示4点校正模式，1表示X点校正模式。
    BYTE          EnableDoubleClick; //启用、禁用双击功能
                                         //0表示禁用，1表示启用
    BYTE          HideCursor; 		//隐藏、显示光标
                                       //0 表示隐藏，1表示显示
    BYTE          EnableBeep; 		//启用、禁用触摸发声
                                       //0 表示禁用，1表示启用
    BYTE          BeepType; 		//发声类型
                                 //0 表示喇叭，1 表示声卡
    BYTE          BeepMode; 		//发声模式
                                 //0 表示落笔发声，1表示抬笔发声，2表示抬笔落笔都发声
    ULONG         BeepFrequency; 	//发声频率
                                 //单位：30-7000 Hz
    ULONG         BeepDuration; 	//发声音量
                                 //音量：00-100 （DB）
    ULONG         Smoothness; 		//平滑度设置

    ULONG         RightClickTime; 	//右键时间 //1000-8000(ms)
    ULONG         RightClickArea; 	//右键区域 //(单位：mm)
    ULONG         DoubleClickTime; 	//双击时间 //
    ULONG         DoubleClickArea; 	//双击区域 //
    BYTE			ByWindowVersion;	// 区分windows版本号，8.1做特殊处理
    BYTE			TouchTimeout;	// 最大超时（5-255）
    BYTE          Reserved[10];     //保留
}
IROPT_DRIVER_CONFIG,*PIROPT_DRIVER_CONFIG;

enum TOUCH_MOD
{
    PRESS = 0x00,
    RELEASE = 0x01,
    DEFAULT = 0x02,
    MOUSE = 0x50,
    TOUCH = 0x60
};

enum BEEPMOD
{
    DOWN,
    UP,
    ALL
};
enum BEEPTYPE
{
    BEEP,
    AUDIO
};

typedef struct _FIRMWARE_INFO
{
    INT16U Vendor_ID;
    INT16U Product_ID;
    INT32U Firmware;
    INT32U Hardware;
    INT32U Boot;
    INT32U Inside_Firmware;
}FIRMWARE_INFO,*PFIRMWARE_INFO;

typedef struct _SERIAL_NUMBER
{
    BYTE Product_Class;
    BYTE Product_Size;
    BYTE Glass_Thickness;
    BYTE Install_Type;
    BYTE Interface_Type;
    BYTE Product_Feature[3];
    BYTE Dcode[2];
    BYTE serial_numb[2];
    BYTE Identification;
    BYTE Reserved[4];
}SERIAL_NUMBER,*PSERIAL_NUMBER;

typedef struct _NO_DRIVER_CalibrateParameters
{
    INT16U	A0;       //16位有符号整数
    INT16U	B0;
    INT16U	C0;
    INT16U	A1;
    INT16U	B1;
    INT16U	C1;

}NO_DRIVER_CALIBRATEPARAMETERS, *pNO_DRIVER_CALIBRATEPARAMETERS;

typedef struct _TUC_CALIB_PARAS_IR_STD
{
    short A0;
    short B0;
    short C0;
    short A1;
    short B1;
    short C1;
    short A2;
    short B2;
}TUC_CALIB_PARAS_IR_STD, *PTUC_CALIB_PARAS_IR_STD;

typedef struct TvCpuID
{
    ULONG CPUID1;
    ULONG CPUID2;
    ULONG CPUID3;
}TVCPUID, *PTVCPUID;

typedef struct _CalibrateParameters
{
    float	A03;
    float	A12;
    float	A21;
    float	A30;
    float	A02;
    float	A11;
    float	A20;
    float	A01;//如果是X default == 0,如果是Y默认是1
    float	A10;//如果是X default == 1,如果是Y默认是0
    float	A00;
    BYTE	reserve[18];
}CalibrateParameters, *PCalibrateParameters;
typedef struct _CalibrateParametersSimple
{
    float A01;
    float A10;
    float A00;
}CalibrateParametersSimple, *PCalibrateParametersSimple;
typedef union _DEVICE_CALIB_PARAS
{
    CalibrateParameters			CPTraditional;
    CalibrateParametersSimple	CPSimple[4];
}
DEVICE_CALIB_PARAS, *PDEVICE_CALIB_PARAS;

typedef struct _TUC_CALIB_PARAS_HID
{
    float WarpMatData[9];

}TUC_CALIB_PARAS_HID, *PTUC_CALIB_PARAS_HID;

typedef union _TUC_CALIB_PARAS
{
    struct
    {
        DEVICE_CALIB_PARAS calibParaX;
        DEVICE_CALIB_PARAS calibParaY;
    }OptParas;
    TUC_CALIB_PARAS_IR_STD IRParas;
    TUC_CALIB_PARAS_HID HidParas;
}TUC_CALIB_PARAS, *PTUC_CALIB_PARAS;

typedef struct Point
{
    unsigned short x;
    unsigned short y;
    unsigned short z;
}PT;

#define EP_INTR			(1 | LIBUSB_ENDPOINT_IN)
#define EP_DATA			(2 | LIBUSB_ENDPOINT_IN)

#define USB_RQ			0x04
#define INTR_LENGTH		64

#define HID_GET_REPORT                0x01
#define HID_GET_IDLE                  0x02
#define HID_GET_PROTOCOL              0x03
#define HID_SET_REPORT                0x09
#define HID_SET_IDLE                  0x0A
#define HID_SET_PROTOCOL              0x0B
#define HID_REPORT_TYPE_INPUT         0x01
#define HID_REPORT_TYPE_OUTPUT        0x02
#define HID_REPORT_TYPE_FEATURE       0x03

#define REPORT_ID 0x55

#define CTRL_IN		(LIBUSB_ENDPOINT_IN|LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE)
#define CTRL_OUT	(LIBUSB_ENDPOINT_OUT|LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE)

//定义命令字节

#define READ_FIRMWARE BYTE(0x60)
#define READ_FIRMWARE2 BYTE(0x61)
#define WRITE_FIRMWARE BYTE(0x62)

#define READ_SERIAL_NUMBER BYTE(0x45)
#define WRITE_SERIAL_NUMBER BYTE(0x46)

#define READ_PROPERITY BYTE(0x68)
#define WRITE_PROPERITY BYTE(0x69)

#define READ_CALIB BYTE(0x70)
#define WRITE_CALIB BYTE(0x71)

#define READ_FEATURETYPE BYTE(0x78)
#define WRITE_FEATURETYPE BYTE(0x79)

#define RESET   BYTE(0xb0)
#define SELFCHECK BYTE(0xb1)
#define READ_WORKMOD BYTE(0xb2)
#define WRITE_WORKMOD BYTE(0xb3)
#define READ_OUTMOD BYTE(0xb4)
#define WRITE_OUTMOD BYTE(0xb5)

#define ENTER_USER_CODE BYTE(0xa0)
#define READ_CPUID BYTE(0xa4)
#define CRC_1 BYTE(0xa6)
#define ERASE_ROM BYTE(0xa7)
#define WRITE_ROM BYTE(0xa9)
#define WRITE_BUF BYTE(0xab)

//OSD
#define OSD_MAIN_OPEN BYTE(0x10)
#define OSD_MIAN_POSITION BYTE(0x11)
#define OSD_BRIGHTNESS_OPEN BYTE(0x12)
#define OSD_BRIGHTNESS_POSITION BYTE(0x13)
#define OSD_BRIGHTNESS_SET BYTE(0x14)
#define OSD_CONTRAST_OPEN BYTE(0x15)
#define OSD_CONTRAST_POSITION BYTE(0x16)
#define OSD_CONTRAST_SET BYTE(0x17)
#define OSD_AUTO_SET   BYTE(0x18)

//结束命令字节定义

#define MAX_RIGHT_DELAY 8000
#define MIN_RIGHT_DELAY 1000

#define MAX_TIMEOUT 255
#define MIN_TIMEOUT 5

#define MAX_BEEP_FREQ 7000
#define MIN_BEEP_FREQ 30

#define MAX_BEEP_DURATION 100
#define MIN_BEEP_DURATION 00

#define MAX_SMOOTH 25        // ypf
#define MIN_SMOOTH 0

#define TIMER_INTERVAL 150       // ypf
#define TIP_DURATION  5000       // ypf

#define MAX_BRIGHTNESS 100
#define MIN_BRIGHTNESS 0
#define MAX_CONTRAST 100
#define MIN_CONTRAST 0

#define XCALIB 0x01
#define P4CALIB 0x00


//定义包起始字节
#define PACK_HEADER 0x55
#define DEVICE_ID 0xA8
#define DEVICE_ID2 0xA9

#define TIMEOUT 1000
    //定义包类型，四位
enum PACK_TYPE{
    CMD = 0xf0,
    ACK = 0x70,
    NAK = 0xe0,
    DATA = 0x0f,
    SET = 0x07,
    GET = 0x0e
};
enum WORKMOD{
    OUTPUT_MOD = 0xaa,
    CHECK_MOD = 0xbb,
    UPDATE_MOD = 0xcc
};
enum OUT_MOD{
    CALIB_MOUSE = 0xc0,
    CALIB_MULTI = 0xc1,
    NOCALIB = 0xc2,
    NOCALIB_MULTI = 0xce
};

enum SELFCHECKOPTIONS{
    LED=0x01,
    GAIN = 0x02,
    CALIB = 0x03,
    RECOGNIZE = 0x04
};

#endif
