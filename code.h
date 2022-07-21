#ifndef __CLUSTER_COMMON_H__
#define __CLUSTER_COMMON_H__

#define CLUSTER_UT_TEST
#define CLUSTER_WIN32_SWITCH

#define NULL 0

#define ROS_LOOP_FREQUENCY 50

#define CLUSTER_MAX_NODE_NUM              10
#define CLUSTER_MAX_TARGET_NUM          10
#define CLUSTER_MAX_PATH_NUM               10
#define CLUSTER_MAX_MSG_CONTAINER_NUM   10

#define CLUSTER_INVALID_ID                          0xFFFF
#define CLUSTER_RECV_ID_ALL                     CLUSTER_MAX_NODE_NUM
#define CLUSTER_QGC_ID                                 0
#define CLUSTER_STATE_LOCK_TIME          300

#ifdef CLUSTER_WIN32_SWITCH
#define CLUSTER_PRINT(REPORT_STRING, ...) printf("%s->Line[%u]:" REPORT_STRING "\n",__FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define CLUSTER_PRINT(REPORT_STRING, arg...) printf("%s->Line[%u]:" REPORT_STRING "\n",__FUNCTION__, __LINE__, ##arg)
#endif
#define CLUSTER_WAIT_ANY_LETTER() do{char aString[10]; int i = scanf("%s",aString);}while(0)

/*前缀字母c：标识此类型名仅在集群代码中有效，其他开发环境应使用对应的类型*/
typedef unsigned char cUint8;
typedef unsigned short cUint16;
typedef unsigned int cUint32;
typedef unsigned long long cUint64;

typedef char cInt8;
typedef short cInt16;
typedef int cInt32;
typedef long long cInt64;

typedef float cFloat32;
typedef double cFloat64;
/*前缀字母c：标识此类型名仅在集群代码中有效，其他开发环境应使用对应的类型*/

enum Uav_Optional_IE
{
    UAV_OPTIONAL_IE_INITIAL_POS,
    UAV_OPTIONAL_IE_INITIAL_POS_GPS,
    UAV_OPTIONAL_IE_OFFSET_POS,
    UAV_OPTIONAL_IE_OFFSET_POS_GPS,
    UAV_OPTIONAL_IE_GLOBAL_POS,
    UAV_OPTIONAL_IE_GLOBAL_POS_GPS,
    UAV_OPTIONAL_IE_MEMBER_VEL,
    UAV_OPTIONAL_IE_TARGET_INFO,
    UAV_OPTIONAL_IE_UAV_STATE,
    UAV_OPTIONAL_IE_WORK_STATE,
    UAV_OPTIONAL_IE_POS_VEL_CMD,
    UAV_OPTIONAL_IE_VELOCITY_LIMIT,
    UAV_OPTIONAL_IE_STRING,
};

enum Uav_Command
{
    /*Cluster to Member*/
    UAV_COMMAND_MEMBER_AUTOWORK,
    UAV_COMMAND_MEMBER_START,
    UAV_COMMAND_MEMBER_END,
    UAV_COMMAND_MISSION_START,
    UAV_COMMAND_MISSION_END,
    UAV_COMMAND_WAY_POINT,
    UAV_COMMAND_VELOCITY_CTRL,
    UAV_COMMAND_TARGET_ASSIGN,

    /*Member to Cluster*/
    UAV_COMMAND_MEMBER_AUTOWORK_RESP,
    UAV_COMMAND_MEMBER_START_RESP,
    UAV_COMMAND_MEMBER_END_RESP,
    UAV_COMMAND_MISSION_START_RESP,
    UAV_COMMAND_MISSION_END_RESP,
    UAV_COMMAND_WAY_POINT_RESP,
    UAV_COMMAND_VELOCITY_CTRL_RESP,
    UAV_COMMAND_TARGET_ASSIGN_RESP,
    /*Not need response*/
    UAV_COMMAND_STATE_REPORT,
};

enum Uav_State
{
    UAV_STATE_NONE,
    UAV_STATE_TAKEOFF,
    UAV_STATE_STANDBY,
    UAV_STATE_WAY_POINT,
    UAV_STATE_AUTOWORK,
    UAV_STATE_LANDING,
};

enum Work_State
{
    WORK_STATE_NONE,
    WORK_STATE_REVOKABLE,
    WORK_STATE_UNREVOKABLE,
    WORK_STATE_SUCCESS,
    WORK_STATE_FAILURE,
    WORK_STATE_NO_TARGET,
    WORK_STATE_INVALID,
};

union Cluster_Point {
    cFloat64 aArray[3];
    struct
    {
        cFloat64 x;
        cFloat64 y;
        cFloat64 z;
    }stPoint;
};

struct Cluster_Path_Buf {
    cUint32 uPathNum;
    Cluster_Point aPathPointBuf[CLUSTER_MAX_PATH_NUM];
};

#define UAV_MSG_IE_UAV_STATE                    0x0001
#define UAV_MSG_IE_WORK_STATE                   0x0002
#define UAV_MSG_IE_INITIAL_POS                  0x0004
#define UAV_MSG_IE_OFFSET_POS                   0x0008
#define UAV_MSG_IE_GLOBAL_POS                   0x0010
#define UAV_MSG_IE_VELOCITY                     0x0020
#define UAV_MSG_IE_POS_VEL_CMD                  0x0040
#define UAV_MSG_IE_TARGET_POS                   0x0080
#define UAV_MSG_IE_VELOCITY_LIMIT               0x0100
#define UAV_MSG_IE_STRING                       0x0200

struct Interface_Message
{
    cUint32 uEncodeDirectField;
    Uav_Command enUavCmd;
    Uav_State enUavState;
    Work_State enWorkState;
    Cluster_Point stInitialPos;
    Cluster_Point stOffsetPos;
    Cluster_Point stGlobalPos;
    Cluster_Point stVelocity;
    Cluster_Point stPosVelCmd;
    Cluster_Path_Buf stTargetInfo;
    cFloat32 velocityLimit;
    cUint32 stringLen;
    cInt8 stringMsg[1024];
};
bool fnCluster_codec_DecodeInterfaceMsg(cUint8* pInMsgStream, Interface_Message* pstInterfaceMsg, cUint32 uInMsgLen);
cUint32 fnCluster_codec_EncodeInterfaceMsg(cUint8* pOutMsgStream, Interface_Message* pstInterfaceMsg);

#endif /*__CLUSTER_COMMON_H__*/

