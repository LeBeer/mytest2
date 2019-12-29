#include "osd_api.h"


COSD_API::COSD_API()
{
    m_curDevHandle = NULL;
    m_type = true;

    if(!init_hidraw())
    {
        printf("设备打开错误\n");
        m_flag = false;
        return;
    }
    if(m_type && !init_firmware_infomation())
    {
        printf("获取固件信息错误\n");
        m_flag = false;
        return;
    }
    m_flag = true;
}

BYTE* COSD_API::createBuffer(int size)
{
    m_buffer = (BYTE*)malloc(size);
    memset(m_buffer,0,size);
    return m_buffer;
}

void COSD_API::freeBuffer()
{
    free(m_buffer);
    m_buffer = NULL;
}

BYTE COSD_API::checksum(BYTE &pack_type, BYTE &cmd, BYTE datalen, BYTE *data)
{
    BYTE sum=0xAA+PACK_HEADER;
    sum+=pack_type;
    sum+=cmd;
    sum+=datalen;
    int i = 0;
    while (i<datalen){
        sum+=*(data+i++);
    }
    return sum;
}

bool COSD_API::prase_ack(BYTE *ack)
{
    if(ack&&(ack[0]==PACK_HEADER)&&ack[3]){
        if(ack[4+ack[3]] == checksum(ack[1],ack[2],ack[3],ack+4)){
            if(ack[1]&&0xf0!= NAK)
                return true;
        }
    }
    return false;
}

void COSD_API::prase_nak(BYTE *nak)
{
    if(nak&&(nak[0]==PACK_HEADER)&&nak[3]){
        if(nak[4+nak[3]] == checksum(nak[1],nak[2],nak[3],nak+4)){
            if(nak[1]&&0xf0== NAK)
                return;
        }
    }
}

bool COSD_API::init_hidraw()
{
    if (hid_init())
    {
        printf("fail to call hid_init()!\n");
        return false;
    }

    struct hid_device_info *t_devs, *t_cur_dev;
    t_devs = hid_enumerate(VID, PID);
    t_cur_dev = t_devs;
    while(t_cur_dev)
    {
        m_type = true;
        bool t_found = false;
        string tmp = t_cur_dev->path;
        printf("path(PID):%s\n", tmp.c_str());
        for(list<SHidHandle*>::iterator iter = m_devHandles.begin();
            iter != m_devHandles.end(); iter++)
        {
            if( (*iter)->devPath.find(tmp.substr(DEV_ID_START_POS,DEV_ID_LENGTH)) != 0 )
            {
                t_found = true;
                break;
            }
        }
        if(!t_found)
        {
            SHidHandle *t_handle = new SHidHandle;
            t_handle->devPath = tmp;
            t_handle->handle = NULL;
            memset(t_handle->h_buffer,0,GET_REPORT_SIZE);
            m_devHandles.push_back(t_handle);
        }
        t_cur_dev = t_cur_dev->next;
    }

    t_devs = hid_enumerate(VID, PID2);
    t_cur_dev = t_devs;
    while(t_cur_dev)
    {
        m_type = false;
        bool t_found = false;
        string tmp = t_cur_dev->path;
        printf("path(PID2):%s\n", tmp.c_str());
        for(list<SHidHandle*>::iterator iter = m_devHandles.begin();
            iter != m_devHandles.end(); iter++)
        {
            if( (*iter)->devPath.find(tmp.substr(DEV_ID_START_POS,DEV_ID_LENGTH)) != 0 )
            {
                t_found = true;
                break;
            }
        }
        if(!t_found)
        {
            SHidHandle *t_handle = new SHidHandle;
            t_handle->devPath = tmp;
            t_handle->handle = NULL;
            memset(t_handle->h_buffer,0,GET_REPORT_SIZE);
            m_devHandles.push_back(t_handle);
        }
        t_cur_dev = t_cur_dev->next;
    }
    hid_free_enumeration(t_devs);

    if(m_devHandles.empty())
    {
        printf("no device to open!\n");
        return false;
    }
    else
    {
        m_curDevHandle = m_devHandles.front();  //////////////////////////
        //m_curDevHandle = m_devHandles.back();
    }

   if( !open_cur_hiddev() )
   {
       return false;
   }

   return true;
}

bool COSD_API::init_data(BYTE cmd, unsigned char* data)
{
    if(m_curDevHandle->handle != NULL)
    {
        int r = hid_send_feature_report(m_curDevHandle->handle, get_cmd(cmd), GET_REPORT_SIZE);
        if (r < 0)
        {
            perror("hid_send_feature_report");
            fprintf(stderr, "init_data step 1 error %d\n", r);
            freeBuffer();
            return false;
        }else
                printf("hid_send_feature_report returned: %d\n", r);
#if 0
        int i = 0;
        while(i<r)
            qDebug("%02x",*(m_buffer+i++));
        qDebug()<<endl;
#endif
        freeBuffer();
        // createBuffer(64);
        createBuffer(GET_REPORT_SIZE);
        m_buffer[0] = 0x55;
        r = hid_get_feature_report(m_curDevHandle->handle, m_buffer, GET_REPORT_SIZE);
        int time = 10;
        while(!r&&time--)
            r = hid_get_feature_report(m_curDevHandle->handle, m_buffer, GET_REPORT_SIZE);
        if (r < 0){
            perror("hid_get_feature_report");
            fprintf(stderr, "init_data step 2 error %d\n", r);
            //qDebug("error : %d", GetLastError());
            freeBuffer();
            return false;
        }

        if(prase_ack(m_buffer)){
            int cmd1=*(m_buffer+2);
            if(cmd!=cmd1)
                return false;

            if(m_buffer[1] == (ACK|DATA)){
                //bzero(data,*(buffer+3));          //////////////////// test
                memset(data, '\0', sizeof(data));
                switch (*(m_buffer+2)) {
                case READ_FEATURETYPE:
                {
                    /*BYTE *buf= buffer;
                    memcpy(&(driver_conf),buf+5,9);
                    buf+=14;
                    driver_conf.BeepFrequency = ((*(buf+3)<<8|*(buf+2))<<8|*(buf+1))<<8|*(buf);
                    buf+=4;
                    driver_conf.BeepDuration = ((*(buf+3)<<8|*(buf+2))<<8|*(buf+1))<<8|*(buf);
                    buf+=4;
                    driver_conf.Smoothness = ((*(buf+3)<<8|*(buf+2))<<8|*(buf+1))<<8|*(buf);
                    buf+=4;
                    driver_conf.RightClickTime = ((*(buf+3)<<8|*(buf+2))<<8|*(buf+1))<<8|*(buf);
                    buf+=4;
                    driver_conf.RightClickArea = ((*(buf+3)<<8|*(buf+2))<<8|*(buf+1))<<8|*(buf);
                    buf+=4;
                    driver_conf.DoubleClickTime = ((*(buf+3)<<8|*(buf+2))<<8|*(buf+1))<<8|*(buf);
                    buf+=4;
                    driver_conf.DoubleClickArea = ((*(buf+3)<<8|*(buf+2))<<8|*(buf+1))<<8|*(buf);
                    buf+=4;
                    memcpy(&(driver_conf.ByWindowVersion),buf,12);
                    break;*/
                }
                case READ_PROPERITY:
                {
                    /*BYTE *buf= buffer;
                    device_conf.MonitorID = *(buf+5);
                    buf+=6;
                    device_conf.DefaultTouchArea.MinX = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    device_conf.DefaultTouchArea.MinY = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    device_conf.DefaultTouchArea.MaxX = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    device_conf.DefaultTouchArea.MaxY = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    device_conf.CalibratStatus = *buf;
                    buf+=1;
                    device_conf.ExtendTouchArea[0].MinX = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    device_conf.ExtendTouchArea[0].MinY = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    device_conf.ExtendTouchArea[0].MaxX = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    device_conf.ExtendTouchArea[0].MaxY = *(buf)|*(buf+1)<<8;
                    buf+=2;

                    device_conf.ExtendTouchArea[1].MinX = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    device_conf.ExtendTouchArea[1].MinY = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    device_conf.ExtendTouchArea[1].MaxX = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    device_conf.ExtendTouchArea[1].MaxY = *(buf)|*(buf+1)<<8;
                    buf+=2;

                    device_conf.ExtendTouchArea[2].MinX = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    device_conf.ExtendTouchArea[2].MinY = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    device_conf.ExtendTouchArea[2].MaxX = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    device_conf.ExtendTouchArea[2].MaxY = *(buf)|*(buf+1)<<8;
                    buf+=2;
                    memcpy(device_conf.Reserved,buf,24);
                    break;*/
                }
                case READ_CPUID:
                {
                    /*BYTE *buf= buffer+9;
                    long rombottom = ((*(buf+3)<<8|*(buf+2))<<8|*(buf+1))<<8|*(buf);
                    buf = buffer+5;
                    pageSize = *(buf+1)<<8|*(buf);
                    long romtop = (rombottom+pageSize*(*(buffer+7)));
                    m_cpuType  = *(buffer+8);
                    buf = buffer+13;
                    long rambottom = ((*(buf+3)<<8|*(buf+2))<<8|*(buf+1))<<8|*(buf);

                    m_Flash_Inform = "0x"+QString::number(rombottom,16)+"- "+"0x"+QString::number(romtop,16);
                    Flash_Size = (romtop - rombottom) ; //设备的用户空间

                    m_Ram_Inform = "0x"+QString::number(rambottom,16)+"- ";
                    buf = buffer+17;
                    cpuid.CPUID1 = ((*(buf+3)<<8|*(buf+2))<<8|*(buf+1))<<8|*(buf);
                    buf+=4;
                    cpuid.CPUID2 = ((*(buf+3)<<8|*(buf+2))<<8|*(buf+1))<<8|*(buf);
                    buf+=4;
                    cpuid.CPUID3 = ((*(buf+3)<<8|*(buf+2))<<8|*(buf+1))<<8|*(buf);
                    break;*/
                }
                default:
                    memcpy(data,m_buffer+5,*(m_buffer+3)-1);
                    break;
                }
                freeBuffer();
                return true;
            }else{
                freeBuffer();
                return false;
            }
        }else{
            prase_nak(m_buffer);
            freeBuffer();
            return false;
        }
    }
    return false;
}

bool COSD_API::init_firmware_infomation()
{
    if(!setWorkMode(CHECK_MOD))
    {
        return false;
    }

    bool res = init_data(READ_FIRMWARE,(unsigned char*)&m_firmware);

    if(!setWorkMode(OUTPUT_MOD))
    {
        return false;
    }

    return res;
}

bool COSD_API::open_cur_hiddev()
{
    for(list<SHidHandle*>::iterator iter = m_devHandles.begin();
        iter != m_devHandles.end(); iter++)
    {
        if((*iter)->handle != NULL)
        {
            hid_close((*iter)->handle);
            (*iter)->handle = NULL;
            memset((*iter)->h_buffer,0,GET_REPORT_SIZE);
        }
    }

    m_curDevHandle->handle = hid_open_path(m_curDevHandle->devPath.c_str());
    //m_curDevHandle.handle = hid_open(VID, PID, NULL);
    printf("curr_path:%s\n", m_curDevHandle->devPath.c_str());
    if (m_curDevHandle->handle == NULL)
    {
        perror("open hidraw");
        fprintf(stderr, "hid_open \n");
        printf("error code :%d\n", errno);
        return false;
    }
    // current device, others should be null handle!!!
    return true;
}

bool COSD_API::setWorkMode(WORKMOD mode)
{
    BYTE value=mode;
    bool res =  false;
    int time = 10;
    while(!res&&time--)
        res =  set_conf(WRITE_WORKMOD,&value,1);
    return res;
}

bool COSD_API::set_conf(BYTE cmd, unsigned char *conf,BYTE len)
{
    if(m_curDevHandle->handle != NULL)
    {
        set_cmd(cmd,conf,len);
        int r = hid_send_feature_report(m_curDevHandle->handle, m_buffer, GET_REPORT_SIZE);
        if (r < 0)
        {
            perror("hid_send_feature_report");
            fprintf(stderr, "setconf step 1 error %d\n", r);
            printf("error code :%d\n", errno);
            //printf("error : %d\n", GetLastError());
            printf("cmd: %d\n", cmd);
            freeBuffer();
            return false;
        }else
            printf("hid_send_feature_report returned: %d\n", r);
#if 0
        if( cmd == WRITE_FEATURETYPE )
        {
            printf("set_conf()  WRITE_FEATURETYPE  send\n");
            int i = 0;
            while(i<r)
              printf("%02x\n",*(buffer+i++));
            //qDebug()<<r<<endl;
        }
        else if( cmd == WRITE_PROPERITY )
        {
            printf("set_conf()  WRITE_PROPERITY  send\n");
            int i = 0;
            while(i<r)
              printf("%02x\n",*(buffer+i++));
            //qDebug()<<r<<endl;
        }
        else if( cmd == RESET )
        {
            printf("set_conf()  RESET  send\n");
            int i = 0;
            while(i<r)
              printf("%02x\n",*(buffer+i++));
            //qDebug()<<r<<endl;
        }
        else if( cmd == WRITE_CALIB )
        {
            printf("set_conf()  WRITE_CALIB  send\n");
            int i = 0;
            while(i<r)
              printf("%02x\n",*(buffer+i++));
            //qDebug()<<r<<endl;
        }
        /*else if( cmd == WRITE_OUTMOD )
        {
            qDebug("set_conf()  WRITE_OUTMOD  send");
            int i = 0;
            while(i<r)
              qDebug("%02x",*(buffer+i++));
            qDebug()<<r<<endl;
        }
        else if( cmd == WRITE_WORKMOD )
        {
            qDebug("set_conf()  WRITE_WORKMOD  send");
            int i = 0;
            while(i<r)
              qDebug("%02x",*(buffer+i++));
            qDebug()<<r<<endl;
        }*/
#endif
        freeBuffer();
        //createBuffer(64);
        createBuffer(GET_REPORT_SIZE);
        m_buffer[0] = 0x55; /* Report Number */
        r = hid_get_feature_report(m_curDevHandle->handle, m_buffer, GET_REPORT_SIZE);
        int time = 10;
        while(!r&&time--)
            r = hid_get_feature_report(m_curDevHandle->handle, m_buffer, GET_REPORT_SIZE);
        if (r < 0){
            perror("hid_get_feature_report");
            fprintf(stderr, "set conf step 2 error %d with cmd %d\n", r, cmd);
            freeBuffer();
            //printf("error : %d\n", GetLastError());
            printf("cmd: %d\n", cmd);
            return  false ;
        }
#if 0
        if( cmd == WRITE_FEATURETYPE )
        {
            printf("set_conf()  WRITE_FEATURETYPE receive\n");
            int i = 0;
            while(i<r)
              printf("%02x\n",*(buffer+i++));
            //qDebug()<<r<<endl;
        }
        else if( cmd == WRITE_PROPERITY )
        {
            printf("set_conf()  WRITE_PROPERITY  receive\n");
            int i = 0;
            while(i<r)
              printf("%02x\n",*(buffer+i++));
            //qDebug()<<r<<endl;
        }
        else if( cmd == RESET )
        {
            printf("set_conf()  RESET receive\n");
            int i = 0;
            while(i<r)
              printf("%02x\n",*(buffer+i++));
            //qDebug()<<r<<endl;
        }
        else if( cmd == WRITE_CALIB )
        {
            printf("set_conf()  WRITE_CALIB receive\n");
            int i = 0;
            while(i<r)
              printf("%02x\n",*(buffer+i++));
            //qDebug()<<r<<endl;
        }
        /*else if( cmd == WRITE_OUTMOD )
        {
            qDebug("set_conf()  WRITE_OUTMOD receive");
            int i = 0;
            while(i<r)
              qDebug("%02x",*(buffer+i++));
            qDebug()<<r<<endl;
        }
        else if( cmd == WRITE_WORKMOD )
        {
            qDebug("set_conf()  WRITE_WORKMOD  receive");
            int i = 0;
            while(i<r)
              qDebug("%02x",*(buffer+i++));
            qDebug()<<r<<endl;
        }*/
#endif
        if(prase_ack(m_buffer))
        {
            if(m_buffer[1] == (ACK|SET))
               printf( "set conf success %d\n", r);
        }
        else
        {
            prase_nak(m_buffer);
            freeBuffer();
            return false;
        }
        freeBuffer();
        return true;
    }
    return false;
}

bool COSD_API::set_conf(BYTE cmd, unsigned char* conf, BYTE len, SHidHandle *hidHandle)
{
    if(hidHandle->handle != NULL)
    {
        set_cmd(cmd,conf,len,hidHandle->h_buffer);
        int r = hid_send_feature_report(hidHandle->handle, hidHandle->h_buffer, GET_REPORT_SIZE);
        if (r < 0)
        {
            perror("hid_send_feature_report");
            fprintf(stderr, "setconf step 1 error %d\n", r);
            printf("error code :%d\n", errno);
            //printf("error : %d\n", GetLastError());
            printf("cmd: %d\n", cmd);
            //freeBuffer();
            return false;
        }else
            printf("hid_send_feature_report returned: %d\n", r);

        //freeBuffer();
        //createBuffer(64);
        //createBuffer(GET_REPORT_SIZE);
        memset(hidHandle->h_buffer,0,GET_REPORT_SIZE);
        hidHandle->h_buffer[0] = 0x55; /* Report Number */
        r = hid_get_feature_report(hidHandle->handle, hidHandle->h_buffer, GET_REPORT_SIZE);
        int time = 10;
        while(!r&&time--)
            r = hid_get_feature_report(hidHandle->handle, hidHandle->h_buffer, GET_REPORT_SIZE);
        if (r < 0){
            perror("hid_get_feature_report");
            fprintf(stderr, "set conf step 2 error %d with cmd %d\n", r, cmd);
            //freeBuffer();
            //printf("error : %d\n", GetLastError());
            printf("cmd: %d\n", cmd);
            return  false ;
        }
        if(prase_ack(hidHandle->h_buffer))
        {
            if(hidHandle->h_buffer[1] == (ACK|SET))
               printf( "set conf success %d\n", r);
        }
        else
        {
            prase_nak(hidHandle->h_buffer);
            //freeBuffer();
            return false;
        }
        //freeBuffer();
        return true;
    }
    return false;
}

BYTE* COSD_API::set_cmd(BYTE &cmd,BYTE* data,BYTE &datalen)
{
    BYTE* buf = createBuffer(datalen+6);
    buf[0] = PACK_HEADER;
    buf[1] = CMD|SET;
    buf[2] = cmd;
    buf[3] = 1+datalen;
    buf[4] = DEVICE_ID;

    memcpy(buf+5,data,datalen);

    buf[datalen+5] = checksum(buf[1],buf[2],buf[3],buf+4);
    return buf;
}

BYTE* COSD_API::set_cmd(BYTE &cmd,BYTE* data,BYTE &datalen,BYTE* buf)
{
    //BYTE* buf = createBuffer(datalen+6);
    buf[0] = PACK_HEADER;
    buf[1] = CMD|SET;
    buf[2] = cmd;
    buf[3] = 1+datalen;
    buf[4] = DEVICE_ID;

    memcpy(buf+5,data,datalen);

    buf[datalen+5] = checksum(buf[1],buf[2],buf[3],buf+4);
    return buf;
}

BYTE* COSD_API::get_cmd(BYTE &cmd)
{
    BYTE* buf = createBuffer(6);
    buf[0] = PACK_HEADER;
    buf[1] = CMD|GET;
    buf[2] = cmd;
    buf[3] = 1;
    buf[4] = DEVICE_ID;
    if(cmd == READ_PROPERITY)
        buf[4] = DEVICE_ID2;
    buf[5] = checksum(buf[1],buf[2],buf[3],buf+4);
    return buf;
}

// OSD
bool COSD_API::OSD_open(BYTE cmd)
{
    BYTE* buf = createBuffer(13);
    buf[0] = 0x55;
    buf[1] = CMD;
    buf[2] = cmd;       //0x10 0x18
    buf[3] = 0x08;
    buf[4] = 0xA8;
    buf[5] = 0x07;
    buf[6] = 0x12;
    buf[7] = cmd;       //0x10 0x18
    buf[8] = 0x00;
    buf[9] = 0x01;
    buf[10] = 0x00;
    buf[11] = 0x100 - buf[5] - buf[6] - buf[7] - buf[9];
    buf[12] = 0xAA + buf[0] + buf[1] + buf[2] + buf[3] + buf[4]
            + buf[5] + buf[6] + buf[7] + buf[9] + 0xD6;

    printf("send\n");
    int i=0;
    while(i<13)
        printf("%02x\n",*(buf+i++));
    //qDebug()<<13<<endl;

    if(m_curDevHandle->handle != NULL)
    {
        int r = hid_send_feature_report(m_curDevHandle->handle, buf, GET_REPORT_SIZE);
        if (r < 0)
        {
            perror("hid_send_feature_report");
            fprintf(stderr, "OSD_open step 1 error %d\n", r);
            printf("error code :%d\n", errno);
            //printf("error : %d\n", GetLastError());
            printf("cmd: %d\n", cmd);
            freeBuffer();
            return false;
        }
        else
            printf("hid_send_feature_report returned: r %d\n", r);

        freeBuffer();
        buf = createBuffer(GET_REPORT_SIZE);
        buf[0] = 0x55; /* Report Number */
        if(m_type)
        {
            printf("feature report\n");
            r = hid_get_feature_report(m_curDevHandle->handle, buf, GET_REPORT_SIZE);
        }
        else
        {
            printf("nomal report\n");
            r = hid_read_timeout(m_curDevHandle->handle, buf, GET_REPORT_SIZE, 100);
        }
        if (r < 0)
        {
            perror("hid_get_feature_report");
            fprintf(stderr, "OSD_open step 2 error %d\n", r);
            printf("error code :%d\n", errno);
            //printf("error : %d\n", GetLastError());
            printf("cmd: %d\n", cmd);
            freeBuffer();
            return false;
        }
        else
        {
            printf("hid_get_feature_report returned: r %d\n", r);
            printf("receive\n");
            int i=0;
            while(i<r)
                printf("%02x\n",*(buf+i++));
            //qDebug()<<r<<endl;
        }
    }
    else
    {
        freeBuffer();
        return false;
    }

    freeBuffer();
    return true;
}

bool COSD_API::OSD_close(BYTE cmd)
{
    BYTE* buf = createBuffer(13);
    buf[0] = 0x55;
    buf[1] = CMD;
    buf[2] = cmd;
    buf[3] = 0x08;
    buf[4] = 0xA8;
    buf[5] = 0x07;
    buf[6] = 0x12;
    buf[7] = cmd;
    buf[8] = 0x00;
    //buf[9] = 0x01;
    buf[9] = 0x00;      //close
    buf[10] = 0x00;
    buf[11] = 0x100 - buf[5] - buf[6] - buf[7] - buf[9];
    buf[12] = 0xAA + buf[0] + buf[1] + buf[2] + buf[3] + buf[4]
            + buf[5] + buf[6] + buf[7] + buf[9] + 0xD6;

    printf("send\n");
    int i=0;
    while(i<13)
        printf("%02x\n",*(buf+i++));
    //qDebug()<<13<<endl;

    if(m_curDevHandle->handle != NULL)
    {
        int r = hid_send_feature_report(m_curDevHandle->handle, buf, GET_REPORT_SIZE);
        if (r < 0)
        {
            perror("hid_send_feature_report");
            fprintf(stderr, "OSD_close step 1 error %d\n", r);
            printf("error code :%d\n", errno);
            //printf("error : %d\n", GetLastError());
            printf("cmd: %d\n", cmd);
            freeBuffer();
            return false;
        }
        else
            printf("hid_send_feature_report returned: r %d\n", r);

        freeBuffer();
        buf = createBuffer(GET_REPORT_SIZE);
        buf[0] = 0x55; /* Report Number */
        if(m_type)
        {
            r = hid_get_feature_report(m_curDevHandle->handle, buf, GET_REPORT_SIZE);
        }
        else
        {
            r = hid_read_timeout(m_curDevHandle->handle, buf, GET_REPORT_SIZE, 100);
        }
        if (r < 0)
        {
            perror("hid_get_feature_report");
            fprintf(stderr, "OSD_close step 2 error %d\n", r);
            printf("error code :%d\n", errno);
            //printf("error : %d\n", GetLastError());
            printf("cmd: %d\n", cmd);
            freeBuffer();
            return false;
        }
        else
        {
            printf("hid_get_feature_report returned: r %d\n", r);
            printf("receive\n");
            int i=0;
            while(i<r)
                printf("%02x\n",*(buf+i++));
            //qDebug()<<r<<endl;
        }
    }
    else
    {
        freeBuffer();
        return false;
    }

    freeBuffer();
    return true;
}

bool COSD_API::OSD_readPosition(BYTE cmd, BYTE value, BYTE* rect, int num)
{
    BYTE* buf = createBuffer(13);
    buf[0] = 0x55;
    buf[1] = CMD;
    buf[2] = cmd;
    buf[3] = 0x08;
    buf[4] = 0xA8;
    buf[5] = 0x07;
    buf[6] = 0x12;
    buf[7] = cmd;
    buf[8] = 0x00;
    buf[9] = value;
    buf[10] = 0x00;
    buf[11] = 0x100 - buf[5] - buf[6] - buf[7] - buf[9];
    buf[12] = 0xAA + buf[0] + buf[1] + buf[2] + buf[3] + buf[4]
            + buf[5] + buf[6] + buf[7] + buf[9] + 0xD6;
    printf("send\n");
    int i=0;
    while(i<13)
        printf("%02x\n",*(buf+i++));

    if(m_curDevHandle->handle != NULL)
    {
        int r = hid_send_feature_report(m_curDevHandle->handle, buf, GET_REPORT_SIZE);
        if (r < 0)
        {
            perror("hid_send_feature_report");
            fprintf(stderr, "OSD_readPosition step 1 error %d\n", r);
            printf("error code :%d\n", errno);
            //printf("error : %d\n", GetLastError());
            printf("cmd: %d\n", cmd);
            freeBuffer();
            return false;
        }
        else
            printf("hid_send_feature_report returned: r %d\n", r);

        freeBuffer();
        buf = createBuffer(GET_REPORT_SIZE);
        buf[0] = 0x55; /* Report Number */
        for(int j = 0; j < num; ++j)
        {
            if(m_type)
            {
                r = hid_get_feature_report(m_curDevHandle->handle, buf, GET_REPORT_SIZE);
            }
            else
            {
                r = hid_read_timeout(m_curDevHandle->handle, buf, GET_REPORT_SIZE, 300);
            }
            if (r < 0)
            {
                perror("hid_send_feature_report");
                fprintf(stderr, "OSD_readPosition step 2 error %d\n", r);
                printf("error code :%d\n", errno);
                //printf("error : %d\n", GetLastError());
                printf("cmd: %d\n", cmd);
                freeBuffer();
                return false;
            }
            else
            {
                printf("hid_get_feature_report returned: r %d\n", r);
                printf("receive\n");
                int i=0;
                while(i<r)
                    printf("%02x\n",*(buf+i++));
            }
            //rect[j] = buf[6];
            //rect[j] *= 256;
            rect[j] = buf[9];
        }
    }
    else
    {
        freeBuffer();
        return false;
    }

    freeBuffer();
    return true;
}

bool COSD_API::OSD_readAllPosition(BYTE cmd)
{
    BYTE rect[RECTANGLE_NUM];
    int num = 0;
    if(m_status == MAIN)
    {
        num = POSISION_VALUE_MAX;
    }
    else if(m_status == BRIGHTNESS || m_status == CONTRAST)
    {
        num = POSISION_VALUE_MAX-1;
    }
    else
    {
        return false;
    }

    for(int i = 0; i < num; ++i)
    {
        if( !OSD_readPosition(cmd, i, rect, RECTANGLE_NUM) )
        {
            return false;
        }

        m_areas[i].CenterX = rect[0];
        m_areas[i].CenterY = rect[1];
        m_areas[i].Width = rect[2];
        m_areas[i].Height = rect[3];
    }

    //
    /*for(int i = 0; i < pthread_id.size(); ++i)
    {
        printf("pthread id : %d\n", pthread_id.at(i));
    }
    //
    pthread_t id;
    if(0 == pthread_create(&id,NULL,&USB_RW::OSD_run,NULL))
    {
        pthread_id.append(id);
    }*/

    return true;
}

void COSD_API::OSD_run(void *par)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

    int num = 0;
    if(m_status == MAIN)
    {
        num = POSISION_VALUE_MAX;
    }
    else if(m_status == BRIGHTNESS || m_status == CONTRAST)
    {
        num = POSISION_VALUE_MAX-1;
    }
    else
    {
        return;
    }

    //
    while(true)
    {
        PT pm = getCoordinate(m_curDevHandle);

        bool flag = false;
        int j = 0;
        for(; j < num; ++j)
        {
            if( EnterRectangleArea(pm, m_areas[j]) )            //
            {
                // open or status
                if( m_status == MAIN )
                {
                    flag = true;
                    switch (j)
                    {
                        // brightness
                        case 0:
                            if( !OSD_close(OSD_MAIN_OPEN) )     printf("failed to call OSD_close()\n");
                            if( !OSD_open(OSD_BRIGHTNESS_OPEN) )     printf("failed to call OSD_open()\n");
                            if( !OSD_readAllPosition(OSD_BRIGHTNESS_POSITION) )     printf("failed to call OSD_readAllPosition()\n");
                            break;
                        // contrast
                        case 1:
                            if( !OSD_close(OSD_MAIN_OPEN) )     printf("failed to call OSD_close()\n");
                            if( !OSD_open(OSD_CONTRAST_OPEN) )     printf("failed to call OSD_open()\n");
                            if( !OSD_readAllPosition(OSD_CONTRAST_POSITION) )     printf("failed to call OSD_readAllPosition()\n");
                            break;
                        // quit
                        case 5:
                            if( !OSD_close(OSD_MAIN_OPEN) )     printf("failed to call OSD_close()\n");
                            break;
                        default:
                            flag = false;
                            break;
                    }
                }
                else if( m_status == BRIGHTNESS )
                {
                    flag = true;
                    switch (j)
                    {
                        // quit
                        case 4:
                            if( !OSD_close(OSD_BRIGHTNESS_OPEN) )     printf("failed to call OSD_close()\n");
                            if( !OSD_open(OSD_MAIN_OPEN) )     printf("failed to call OSD_open()\n");
                            if( !OSD_readAllPosition(OSD_MIAN_POSITION) )     printf("failed to call OSD_readAllPosition()\n");
                            break;
                        default:
                            flag = false;
                            break;
                    }
                }
                else if( m_status == CONTRAST )
                {
                    flag = true;
                    switch (j)
                    {
                        // quit
                        case 4:
                            if( !OSD_close(OSD_CONTRAST_OPEN) )     printf("failed to call OSD_close()\n");
                            if( !OSD_open(OSD_MAIN_OPEN) )     printf("failed to call OSD_open()\n");
                            if( !OSD_readAllPosition(OSD_MIAN_POSITION) )     printf("failed to call OSD_readAllPosition()\n");
                            break;
                        default:
                            flag = false;
                            break;
                    }
                }

                //break;
            }
        }
        if(flag)    break;
    }

    printf("OSD_run() calling end!\n");
}

bool COSD_API::EnterRectangleArea(PT pm, RECTANGLE_AREA rect)
{
    if( pm.x >= rect.CenterX - rect.Width/2
            && pm.x <= rect.CenterX + rect.Width/2
            && pm.y >= rect.CenterY - rect.Height/2
            &&pm.y <= rect.CenterY + rect.Height/2 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool COSD_API::OSD_setValue(BYTE cmd, BYTE value)
{
    BYTE* buf = createBuffer(13);
    buf[0] = 0x55;
    buf[1] = CMD;
    buf[2] = cmd;
    buf[3] = 0x08;
    buf[4] = 0xA8;
    buf[5] = 0x07;
    buf[6] = 0x12;
    buf[7] = cmd;
    buf[8] = 0x00;
    buf[9] = 0x02;      //absolut
    buf[10] = value;
    buf[11] = 0x100 - buf[5] - buf[6] - buf[7] - buf[8] - buf[9] - buf[10];
    buf[12] = 0xAA + buf[0] + buf[1] + buf[2] + buf[3] + buf[4]
            + buf[5] + buf[6] + buf[7] + buf[8] + buf[9] + buf[10] + 0xD6;

    printf("send\n");
    int i=0;
    while(i<13)
        printf("%02x\n",*(buf+i++));
    //qDebug()<<13<<endl;

    if(m_curDevHandle->handle != NULL)
    {
        int r = hid_send_feature_report(m_curDevHandle->handle, buf, GET_REPORT_SIZE);
        if (r < 0)
        {
            perror("hid_send_feature_report");
            fprintf(stderr, "OSD_setValue step 1 error %d\n", r);
            printf("error code :%d\n", errno);
            //printf("error : %d\n", GetLastError());
            printf("cmd: %d\n", cmd);
            freeBuffer();
            return false;
        }
        else
            printf("hid_send_feature_report returned: r %d\n", r);

        freeBuffer();
        buf = createBuffer(GET_REPORT_SIZE);
        buf[0] = 0x55; /* Report Number */
        if(m_type)
        {
            r = hid_get_feature_report(m_curDevHandle->handle, buf, GET_REPORT_SIZE);
        }
        else
        {
            r = hid_read_timeout(m_curDevHandle->handle, buf, GET_REPORT_SIZE, 100);
        }
        if (r < 0)
        {
            perror("hid_get_feature_report");
            fprintf(stderr, "OSD_setValue step 2 error %d\n", r);
            printf("error code :%d\n", errno);
            //printf("error : %d\n", GetLastError());
            printf("cmd: %d\n", cmd);
            freeBuffer();
            return false;
        }
        else
        {
            printf("hid_get_feature_report returned: r %d\n", r);
            printf("receive\n");
            int i=0;
            while(i<r)
                printf("%02x\n",*(buf+i++));
            //qDebug()<<r<<endl;
        }
    }
    else
    {
        freeBuffer();
        return false;
    }

    freeBuffer();
    return true;
}

bool COSD_API::OSD_tmp_test(BYTE cmd, BYTE value)
{
    BYTE* buf = createBuffer(13);
    buf[0] = 0x55;
    buf[1] = CMD;
    buf[2] = cmd;
    buf[3] = 0x08;
    buf[4] = 0xA8;
    buf[5] = 0x07;
    buf[6] = 0x12;
    buf[7] = cmd;
    buf[8] = 0x00;
    buf[9] = value;
    buf[10] = 0x00;
    buf[11] = 0x100 - buf[5] - buf[6] - buf[7] - buf[9];
    buf[12] = 0xAA + buf[0] + buf[1] + buf[2] + buf[3] + buf[4]
            + buf[5] + buf[6] + buf[7] + buf[9] + 0xD6;

    printf("send\n");
    int i=0;
    while(i<13)
        printf("%02x\n",*(buf+i++));
    //qDebug()<<13<<endl;

    if(m_curDevHandle->handle != NULL)
    {
        int r = hid_send_feature_report(m_curDevHandle->handle, buf, GET_REPORT_SIZE);
        if (r < 0)
        {
            perror("hid_send_feature_report");
            fprintf(stderr, "OSD_tmp_test step 1 error %d\n", r);
            printf("error code :%d\n", errno);
            //printf("error : %d\n", GetLastError());
            printf("cmd: %d\n", cmd);
            freeBuffer();
            return false;
        }
        else
            printf("hid_send_feature_report returned: r %d\n", r);

        freeBuffer();
        buf = createBuffer(GET_REPORT_SIZE);
        buf[0] = 0x55; /* Report Number */
        if(m_type)
        {
            r = hid_get_feature_report(m_curDevHandle->handle, buf, GET_REPORT_SIZE);
        }
        else
        {
            r = hid_read_timeout(m_curDevHandle->handle, buf, GET_REPORT_SIZE, 100);
        }
        if (r < 0)
        {
            perror("hid_get_feature_report");
            fprintf(stderr, "OSD_tmp_test step 2 error %d\n", r);
            printf("error code :%d\n", errno);
            //printf("error : %d\n", GetLastError());
            printf("cmd: %d\n", cmd);
            freeBuffer();
            return false;
        }
        else
        {
            printf("hid_get_feature_report returned: r %d\n", r);
            printf("receive\n");
            int i=0;
            while(i<r)
                printf("%02x\n",*(buf+i++));
            //qDebug()<<r<<endl;
        }
    }
    else
    {
        freeBuffer();
        return false;
    }

    freeBuffer();
    return true;
}

PT COSD_API::getCoordinate(SHidHandle *hidHandle)
{
    PT pPoint;
    pPoint.x = 0;
    pPoint.y = 0;
    pPoint.z = 0;
    if(hidHandle->handle == NULL)
    {
        return pPoint;
    }

    //setWorkMode(CHECK_MOD,handle);
    //setOutputMod(NOCALIB_MULTI,handle);

    bool res = false;
    BYTE value = CHECK_MOD;
    int time = 10;
    while(!res&&time--)
        res =  set_conf(WRITE_WORKMOD,&value,1,hidHandle);
    if(!res)    printf("fail to call set_conf(WRITE_WORKMOD,&value,1,hidHandle)\n");

    res = false;
    value = NOCALIB_MULTI;
    res =  set_conf(WRITE_OUTMOD,&value,1,hidHandle);
    if(!res)    printf("fail to call set_conf(WRITE_OUTMOD,&value,1,hidHandle)\n");

    //
        //BYTE* buf = createBuffer(GET_REPORT_SIZE);
    BYTE* buf = hidHandle->h_buffer;
    buf[0] = PACK_HEADER;
    buf[1] = CMD|GET;
    buf[2] = 0xb1;
    buf[3] = 0x02;
    buf[4] = DEVICE_ID;
    buf[5] = 0x03;
    buf[6] = checksum(buf[1],buf[2],buf[3],buf+4);
    int r = hid_send_feature_report(hidHandle->handle, buf, GET_REPORT_SIZE);
    if (r < 0){
        perror("hid_send_feature_report");
        fprintf(stderr, "init_data step 1 error %d\n", r);
        //freeBuffer();
        //printf("error : %d\n", GetLastError());
        return pPoint;
    }
//  else
//      printf("ioctl HIDIOCGFEATURE returned: %d\n", r);

    //freeBuffer();
    //createBuffer(GET_REPORT_SIZE);
    memset(hidHandle->h_buffer,0,GET_REPORT_SIZE);
    buf[0] = 0x55; /* Report Number */
    r = hid_get_feature_report(hidHandle->handle, buf, GET_REPORT_SIZE);
    if (r < 0){
        perror("hid_get_feature_report");
        //freeBuffer();
        //printf("error : %d\n", GetLastError());
        return pPoint;
    }
#if 0
        int i = 0;
        while(i<r)
            printf("%02x\n",*(buf+i++));
        //qDebug()<<endl;
#endif
    if(buf[2] == 0xb1 && buf[5] != 0xFF && buf[6] != 0xFF&& buf[5] != 0x00 && buf[6] != 0x00){
        int tmp;
        tmp = buf[6];
        tmp *= 256;
        tmp += buf[5];
        pPoint.x = (short)tmp;

        tmp = buf[8];
        tmp *= 256;
        tmp += buf[7];
        pPoint.y = (short)tmp;
    }
    //freeBuffer();


    //setOutputMod(CALIB_MULTI,handle);
    //setWorkMode(OUTPUT_MOD,handle);

    res = false;
    value = CALIB_MULTI;
    res =  set_conf(WRITE_OUTMOD,&value,1,hidHandle);
    if(!res)    printf("fail to call set_conf(WRITE_OUTMOD,&value,1,hidHandle)\n");

    res = false;
    value = OUTPUT_MOD;
    time = 10;
    while(!res&&time--)
        res =  set_conf(WRITE_WORKMOD,&value,1,hidHandle);
    if(!res)    printf("fail to call set_conf(WRITE_WORKMOD,&value,1,hidHandle)\n");

    return pPoint;
}
