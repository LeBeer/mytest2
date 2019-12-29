#ifndef OSD_API_H
#define OSD_API_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include <unistd.h>
#include <string>
#include <list>

#include "touch_type.h"
#include "hidapi.h"

using namespace std;

#define VID 0xc251
#define PID 0x00a1
#define PID2 0x00a2
#define DEV_ID_START_POS 34
#define DEV_ID_LENGTH 8

#define SEND_REPORT_SIZE 6
#define GET_REPORT_SIZE 64

// for OSD
#define RECTANGLE_NUM 4
#define POSISION_VALUE_MAX 6
enum MENU_ITEM
{
    NONE = 0,
    MAIN = 1,
    BRIGHTNESS = 2,
    CONTRAST = 3
};
typedef struct _RECTANGLE_AREA
{
    BYTE                ID;
    BYTE                CenterX;
    BYTE                CenterY;
    BYTE                Height;
    BYTE                Width;
}RECTANGLE_AREA, *PRECTANGLE_AREA;
// end

// for new hidapi
typedef struct _tagHidHandleStruct
{
    string devPath;
    hid_device *handle;
    BYTE h_buffer[64];
}SHidHandle, *PSHidHandle;

class COSD_API
{

public:
    COSD_API();

    // hid device
    bool open_cur_hiddev();
    bool init_hidraw();
    bool init_result() { return m_flag; }
    bool init_data(BYTE cmd,unsigned char* data);
    bool init_firmware_infomation();

    // OSD for windows of main, brightness and contrast
    bool EnterRectangleArea(PT pm, RECTANGLE_AREA rect);
    bool OSD_open(BYTE cmd);
    bool OSD_close(BYTE cmd);
    bool OSD_readPosition(BYTE cmd, BYTE value, BYTE* rect, int num);
    bool OSD_readAllPosition(BYTE cmd);
    bool OSD_setValue(BYTE cmd, BYTE value);
    void OSD_run(void *par);
    bool OSD_tmp_test(BYTE cmd, BYTE value);

    BYTE* createBuffer(int size);
    void  freeBuffer();
    bool setWorkMode(WORKMOD mode);
    bool set_conf(BYTE cmd, unsigned char *conf,BYTE len);
    bool set_conf(BYTE cmd, unsigned char* conf, BYTE len, SHidHandle *hidHandle);
    BYTE* set_cmd(BYTE &cmd,BYTE* data,BYTE &datalen);
    BYTE* set_cmd(BYTE &cmd,BYTE* data,BYTE &datalen,BYTE* buffer);
    BYTE* get_cmd(BYTE &cmd);
    bool prase_ack(BYTE* ack);
    void prase_nak(BYTE* nak);
    BYTE checksum(BYTE &pack_type,BYTE &cmd,BYTE datalen,BYTE*data);
    PT getCoordinate(SHidHandle *hidHandle);
    bool getTypePID() { return m_type; }
    INT32U getFirmwareNum() { return m_firmware.Firmware; }

private:
    bool                    m_flag;
    bool                    m_type;             // PID true ;  PID2 false
    BYTE*                   m_buffer;
    MENU_ITEM               m_status;
    RECTANGLE_AREA          m_areas[POSISION_VALUE_MAX];
    list<SHidHandle*>       m_devHandles;
    SHidHandle*             m_curDevHandle;
    FIRMWARE_INFO           m_firmware;
};

#endif // OSD_API_H
