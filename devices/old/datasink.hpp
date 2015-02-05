#ifndef __V_DATA_SINK_H_
#define __V_DATA_SINK_H_
#include "vpoco.hpp"

using namespace UtilityLib;
using namespace std;

typedef enum {
    CMN_CMD_TYPE_MIN = 0,
    CMN_CMD_TYPE_DATA,
    CMN_CMD_TYPE_MAX
} cmn_cmd_type;

typedef enum {
    CMN_CMD_STATUS_MIN = 0,

    CMN_CMD_STATUS_MAX
} cmn_cmd_status;

typedef enum {
    CMN_DATA_TYPE_MIN = 0,
    CMN_DATA_TYPE_H264,
    CMN_DATA_TYPE_MJPEG,
    CMN_DATA_TYPE_RGB24,

    CMN_DATA_TYPE_MAX
} cmn_data_type;

typedef struct __cmn_cmd{
    cmn_cmd_type type;
    cmn_cmd_status status;

    /* command seq*/
    s32 seq;

    cmn_data_type data_type;
    /* w & h cn are for video data */
    s32 w;
    s32 h;
    s32 cn;

    /* Data size & data  */
    s32 size;
    void *data;
} cmn_cmd;

class SinkNotification:public Notification
{
public:
	typedef AutoPtr<SinkNotification> LPtr;
    
public:
    SinkNotification()
    {
        std::cout<<"construct SinkNotification"<<std::endl;
        cmd.data = NULL;
        
    }
    ~SinkNotification()
    {
        std::cout<<"destruct SinkNotification"<<std::endl;
        if (cmd.data != NULL)
        {
            free(cmd.data);
        }
    }

public:
    s64 nId;
    cmn_cmd cmd;
};


class  BasicSink
{
public:
    virtual void handleSink(SinkNotification* pNf)
    {
        std::cout<<"handleBase: "<<pNf->name()<<std::endl;
        Debug::DebugPrint("ID %d\n", pNf->nId);
        pNf->release();
    }
};

#endif /* __V_DATA_SINK_H_ */