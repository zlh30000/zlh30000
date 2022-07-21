/***************************************************************************************************************************
* codec.c
*
* Author: Zhou
*
* Update Time: 2021.08.20
***************************************************************************************************************************/
#include <stdio.h>
#include "code.h"

void fnCluster_codec_StreamToEightByte(cUint8* pInMsgStream, cUint64* puDestVal)
{
    cUint64 uIntTempToFloat = 0;
    uIntTempToFloat = pInMsgStream[0];
    uIntTempToFloat = (uIntTempToFloat << 8) | pInMsgStream[1];
    uIntTempToFloat = (uIntTempToFloat << 8) | pInMsgStream[2];
    uIntTempToFloat = (uIntTempToFloat << 8) | pInMsgStream[3];
    uIntTempToFloat = (uIntTempToFloat << 8) | pInMsgStream[4];
    uIntTempToFloat = (uIntTempToFloat << 8) | pInMsgStream[5];
    uIntTempToFloat = (uIntTempToFloat << 8) | pInMsgStream[6];
    uIntTempToFloat = (uIntTempToFloat << 8) | pInMsgStream[7];
    *puDestVal = uIntTempToFloat;
}

void fnCluster_codec_StreamToFourByte(cUint8* pInMsgStream, cUint32* puDestVal)
{
    cUint32 uIntTempToFloat = 0;
    uIntTempToFloat = pInMsgStream[0];
    uIntTempToFloat = (uIntTempToFloat << 8) | pInMsgStream[1];
    uIntTempToFloat = (uIntTempToFloat << 8) | pInMsgStream[2];
    uIntTempToFloat = (uIntTempToFloat << 8) | pInMsgStream[3];
    *puDestVal = uIntTempToFloat;
}

void fnCluster_codec_EightByteToStream(cUint8* pOutMsgStream, cUint64* puSourceVal)
{
    cUint64 uFloatTempToInt = *puSourceVal;
    pOutMsgStream[0] = (cUint8)(uFloatTempToInt >> 56);
    pOutMsgStream[1] = (cUint8)(uFloatTempToInt >> 48);
    pOutMsgStream[2] = (cUint8)(uFloatTempToInt >> 40);
    pOutMsgStream[3] = (cUint8)(uFloatTempToInt >> 32);
    pOutMsgStream[4] = (cUint8)(uFloatTempToInt >> 24);
    pOutMsgStream[5] = (cUint8)(uFloatTempToInt >> 16);
    pOutMsgStream[6] = (cUint8)(uFloatTempToInt >> 8);
    pOutMsgStream[7] = (cUint8)(uFloatTempToInt);
}

void fnCluster_codec_FourByteToStream(cUint8* pOutMsgStream, cUint32* puSourceVal)
{
    cUint32 uFloatTempToInt = *puSourceVal;
    pOutMsgStream[0] = (cUint8)(uFloatTempToInt >> 24);
    pOutMsgStream[1] = (cUint8)(uFloatTempToInt >> 16);
    pOutMsgStream[2] = (cUint8)(uFloatTempToInt >> 8);
    pOutMsgStream[3] = (cUint8)(uFloatTempToInt);
}

cUint32 fnCluster_codec_EncodeInterfaceMsg(cUint8* pOutMsgStream, Interface_Message* pstInterfaceMsg)
{
    cUint32 uSubIndex = 0;
    cUint32 uStreamIndex = 2;

    if ((NULL == pOutMsgStream) || (NULL == pstInterfaceMsg))
    {
        CLUSTER_PRINT("Input param error!");
        return 0;
    }

    pOutMsgStream[uStreamIndex++] = (cUint8)(pstInterfaceMsg->enUavCmd);

    if (0 != (pstInterfaceMsg->uEncodeDirectField & UAV_MSG_IE_INITIAL_POS))
    {
        pOutMsgStream[uStreamIndex++] = (cUint8)(UAV_OPTIONAL_IE_INITIAL_POS);
        for (int i = 0; i < 3; i++)
        {
            fnCluster_codec_EightByteToStream(&pOutMsgStream[uStreamIndex], (cUint64*)&pstInterfaceMsg->stInitialPos.aArray[i]);
            uStreamIndex += 8;
        }
    }

    if (0 != (pstInterfaceMsg->uEncodeDirectField & UAV_MSG_IE_OFFSET_POS))
    {
        pOutMsgStream[uStreamIndex++] = (cUint8)(UAV_OPTIONAL_IE_OFFSET_POS);
        for (int i = 0; i < 3; i++)
        {
            fnCluster_codec_EightByteToStream(&pOutMsgStream[uStreamIndex], (cUint64*)&pstInterfaceMsg->stOffsetPos.aArray[i]);
            uStreamIndex += 8;
        }
    }

    if (0 != (pstInterfaceMsg->uEncodeDirectField & UAV_MSG_IE_GLOBAL_POS))
    {
        pOutMsgStream[uStreamIndex++] = (cUint8)(UAV_OPTIONAL_IE_GLOBAL_POS);
        for (int i = 0; i < 3; i++)
        {
            fnCluster_codec_EightByteToStream(&pOutMsgStream[uStreamIndex], (cUint64*)&pstInterfaceMsg->stGlobalPos.aArray[i]);
            uStreamIndex += 8;
        }
    }

    if (0 != (pstInterfaceMsg->uEncodeDirectField & UAV_MSG_IE_VELOCITY))
    {
        pOutMsgStream[uStreamIndex++] = (cUint8)(UAV_OPTIONAL_IE_MEMBER_VEL);
        for (int i = 0; i < 3; i++)
        {
            fnCluster_codec_EightByteToStream(&pOutMsgStream[uStreamIndex], (cUint64*)&pstInterfaceMsg->stVelocity.aArray[i]);
            uStreamIndex += 8;
        }
    }

    if (0 != (pstInterfaceMsg->uEncodeDirectField & UAV_MSG_IE_UAV_STATE))
    {
        pOutMsgStream[uStreamIndex++] = (cUint8)(UAV_OPTIONAL_IE_UAV_STATE);
        pOutMsgStream[uStreamIndex++] = (cUint8)(pstInterfaceMsg->enUavState);
    }

    if (0 != (pstInterfaceMsg->uEncodeDirectField & UAV_MSG_IE_WORK_STATE))
    {
        pOutMsgStream[uStreamIndex++] = (cUint8)(UAV_OPTIONAL_IE_WORK_STATE);
        pOutMsgStream[uStreamIndex++] = (cUint8)(pstInterfaceMsg->enWorkState);
    }

    if (0 != (pstInterfaceMsg->uEncodeDirectField & UAV_MSG_IE_POS_VEL_CMD))
    {
        pOutMsgStream[uStreamIndex++] = (cUint8)(UAV_OPTIONAL_IE_POS_VEL_CMD);
        for (int i = 0; i < 3; i++)
        {
            fnCluster_codec_EightByteToStream(&pOutMsgStream[uStreamIndex], (cUint64*)&pstInterfaceMsg->stPosVelCmd.aArray[i]);
            uStreamIndex += 8;
        }
    }

    if (0 != (pstInterfaceMsg->uEncodeDirectField & UAV_MSG_IE_TARGET_POS))
    {
        pOutMsgStream[uStreamIndex++] = (cUint8)(UAV_OPTIONAL_IE_TARGET_INFO);
        pOutMsgStream[uStreamIndex++] = (cUint8)(pstInterfaceMsg->stTargetInfo.uPathNum >> 8);
        pOutMsgStream[uStreamIndex++] = (cUint8)(pstInterfaceMsg->stTargetInfo.uPathNum);
        uSubIndex = 0;
        uStreamIndex += 2;
        for (cUint32 i = 0; i < pstInterfaceMsg->stTargetInfo.uPathNum; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                fnCluster_codec_EightByteToStream(&pOutMsgStream[uStreamIndex + uSubIndex], (cUint64*)&pstInterfaceMsg->stTargetInfo.aPathPointBuf[i].aArray[j]);
                uSubIndex += 8;
            }
        }
        pOutMsgStream[uStreamIndex - 2] = (cUint8)(uSubIndex >> 8);
        pOutMsgStream[uStreamIndex - 1] = (cUint8)(uSubIndex);
        uStreamIndex += uSubIndex;
    }

    if (0 != (pstInterfaceMsg->uEncodeDirectField & UAV_MSG_IE_VELOCITY_LIMIT))
    {
        pOutMsgStream[uStreamIndex++] = (cUint8)(UAV_OPTIONAL_IE_VELOCITY_LIMIT);
        fnCluster_codec_FourByteToStream(&pOutMsgStream[uStreamIndex], (cUint32*)&pstInterfaceMsg->velocityLimit);
        uStreamIndex += 4;
    }

    if (0 != (pstInterfaceMsg->uEncodeDirectField & UAV_MSG_IE_STRING))
    {
        pOutMsgStream[uStreamIndex++] = (cUint8)(UAV_OPTIONAL_IE_STRING);
        pOutMsgStream[uStreamIndex++] = (cUint8)(pstInterfaceMsg->stringLen >> 8);
        pOutMsgStream[uStreamIndex++] = (cUint8)(pstInterfaceMsg->stringLen);
        for (cUint32 i = 0; i < pstInterfaceMsg->stringLen; i++)
        {
            pOutMsgStream[uStreamIndex++] = (cUint8)pstInterfaceMsg->stringMsg[i];
        }
    }

    pOutMsgStream[0] = (cUint8)((uStreamIndex - 2) >> 8);
    pOutMsgStream[1] = (cUint8)(uStreamIndex - 2);

    return uStreamIndex;
}

bool fnCluster_codec_DecodeInterfaceMsg(cUint8* pInMsgStream, Interface_Message* pstInterfaceMsg, cUint32 uInMsgLen)
{
    cUint32 uMsgLen = 0;
    cUint32 uSubIndex = 0;
    cUint32 uSubMsgLen = 0;
    cUint32 uStreamIndex = 0;

    if ((NULL == pInMsgStream) || (NULL == pstInterfaceMsg))
    {
        CLUSTER_PRINT("Input param error!");
        return false;
    }

    uMsgLen = (cUint32)((pInMsgStream[uStreamIndex] << 8) | pInMsgStream[uStreamIndex + 1]) & 0xFFFF;
    uStreamIndex += 2;
    uMsgLen += uStreamIndex;

    pstInterfaceMsg->enUavCmd = (Uav_Command)pInMsgStream[uStreamIndex++];

    pstInterfaceMsg->uEncodeDirectField = 0;

    if ((uStreamIndex < uMsgLen) && ((cUint8)(UAV_OPTIONAL_IE_INITIAL_POS) == pInMsgStream[uStreamIndex]))
    {
        uStreamIndex++;
        pstInterfaceMsg->uEncodeDirectField |= UAV_MSG_IE_INITIAL_POS;

        if ((uStreamIndex + 8 * 3) <= uMsgLen)
        {
            for (int i = 0; i < 3; i++)
            {
                fnCluster_codec_StreamToEightByte(&pInMsgStream[uStreamIndex], (cUint64*)&pstInterfaceMsg->stInitialPos.aArray[i]);
                uStreamIndex += 8;
            }
        }
        else
        {
            return false;
        }
    }

    if ((uStreamIndex < uMsgLen) && ((cUint8)(UAV_OPTIONAL_IE_OFFSET_POS) == pInMsgStream[uStreamIndex]))
    {
        uStreamIndex++;
        pstInterfaceMsg->uEncodeDirectField |= UAV_MSG_IE_OFFSET_POS;

        if ((uStreamIndex + 8 * 3) <= uMsgLen)
        {
            for (int i = 0; i < 3; i++)
            {
                fnCluster_codec_StreamToEightByte(&pInMsgStream[uStreamIndex], (cUint64*)&pstInterfaceMsg->stOffsetPos.aArray[i]);
                uStreamIndex += 8;
            }
        }
        else
        {
            return false;
        }
    }

    if ((uStreamIndex < uMsgLen) && ((cUint8)(UAV_OPTIONAL_IE_GLOBAL_POS) == pInMsgStream[uStreamIndex]))
    {
        uStreamIndex++;
        pstInterfaceMsg->uEncodeDirectField |= UAV_MSG_IE_GLOBAL_POS;

        if ((uStreamIndex + 8 * 3) <= uMsgLen)
        {
            for (int i = 0; i < 3; i++)
            {
                fnCluster_codec_StreamToEightByte(&pInMsgStream[uStreamIndex], (cUint64*)&pstInterfaceMsg->stGlobalPos.aArray[i]);
                uStreamIndex += 8;
            }
        }
        else
        {
            return false;
        }
    }

    if ((uStreamIndex < uMsgLen) && ((cUint8)(UAV_OPTIONAL_IE_MEMBER_VEL) == pInMsgStream[uStreamIndex]))
    {
        uStreamIndex++;
        pstInterfaceMsg->uEncodeDirectField |= UAV_MSG_IE_VELOCITY;

        if ((uStreamIndex + 8 * 3) <= uMsgLen)
        {
            for (int i = 0; i < 3; i++)
            {
                fnCluster_codec_StreamToEightByte(&pInMsgStream[uStreamIndex], (cUint64*)&pstInterfaceMsg->stVelocity.aArray[i]);
                uStreamIndex += 8;
            }
        }
        else
        {
            return false;
        }
    }

    if ((uStreamIndex + 1 < uMsgLen) && ((cUint8)(UAV_OPTIONAL_IE_UAV_STATE) == pInMsgStream[uStreamIndex]))
    {
        uStreamIndex++;
        pstInterfaceMsg->uEncodeDirectField |= UAV_MSG_IE_UAV_STATE;

        pstInterfaceMsg->enUavState = (Uav_State)pInMsgStream[uStreamIndex++];

    }

    if ((uStreamIndex + 1 < uMsgLen) && ((cUint8)(UAV_OPTIONAL_IE_WORK_STATE) == pInMsgStream[uStreamIndex]))
    {
        uStreamIndex++;
        pstInterfaceMsg->uEncodeDirectField |= UAV_MSG_IE_WORK_STATE;

        pstInterfaceMsg->enWorkState = (Work_State)pInMsgStream[uStreamIndex++];

    }

    if ((uStreamIndex < uMsgLen) && ((cUint8)(UAV_OPTIONAL_IE_POS_VEL_CMD) == pInMsgStream[uStreamIndex]))
    {
        uStreamIndex++;
        pstInterfaceMsg->uEncodeDirectField |= UAV_MSG_IE_POS_VEL_CMD;

        if ((uStreamIndex + 8 * 3) <= uMsgLen)
        {
            for (int i = 0; i < 3; i++)
            {
                fnCluster_codec_StreamToEightByte(&pInMsgStream[uStreamIndex], (cUint64*)&pstInterfaceMsg->stPosVelCmd.aArray[i]);
                uStreamIndex += 8;
            }
        }
        else
        {
            return false;
        }
    }

    if ((uStreamIndex < uMsgLen) && ((cUint8)(UAV_OPTIONAL_IE_TARGET_INFO) == pInMsgStream[uStreamIndex]))
    {
        uStreamIndex++;
        pstInterfaceMsg->uEncodeDirectField |= UAV_MSG_IE_TARGET_POS;
        pstInterfaceMsg->stTargetInfo.uPathNum = (cUint32)((pInMsgStream[uStreamIndex] << 8) | pInMsgStream[uStreamIndex + 1]) & 0xFFFF;
        uStreamIndex += 2;
        uSubMsgLen = (cUint32)((pInMsgStream[uStreamIndex] << 8) | pInMsgStream[uStreamIndex + 1]) & 0xFFFF;
        uStreamIndex += 2;

        if ((uSubMsgLen + uStreamIndex) > uMsgLen)
        {
            return false;
        }

        uSubIndex = 0;
        for (int i = 0; uSubMsgLen > uSubIndex; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                fnCluster_codec_StreamToEightByte(&pInMsgStream[uStreamIndex + uSubIndex], (cUint64*)&pstInterfaceMsg->stTargetInfo.aPathPointBuf[i].aArray[j]);
                uSubIndex += 8;
            }
        }
        //assert(uSubMsgLen == uSubIndex);
        uStreamIndex += uSubIndex;
    }

    if ((uStreamIndex + 4 < uMsgLen) && ((cUint8)(UAV_OPTIONAL_IE_VELOCITY_LIMIT) == pInMsgStream[uStreamIndex]))
    {
        uStreamIndex++;
        pstInterfaceMsg->uEncodeDirectField |= UAV_MSG_IE_VELOCITY_LIMIT;

        fnCluster_codec_StreamToFourByte(&pInMsgStream[uStreamIndex], (cUint32*)&pstInterfaceMsg->velocityLimit);
        uStreamIndex += 4;

    }

    if ((uStreamIndex < uMsgLen) && ((cUint8)(UAV_OPTIONAL_IE_STRING) == pInMsgStream[uStreamIndex]))
    {
        uStreamIndex++;
        pstInterfaceMsg->uEncodeDirectField |= UAV_MSG_IE_STRING;
        pstInterfaceMsg->stringLen = (cUint32)((pInMsgStream[uStreamIndex] << 8) | pInMsgStream[uStreamIndex + 1]) & 0xFFFF;
        uStreamIndex += 2;

        if ((pstInterfaceMsg->stringLen + uStreamIndex) > uMsgLen)
        {
            return false;
        }

        for (cUint32 i = 0; i < pstInterfaceMsg->stringLen; i++)
        {
            pstInterfaceMsg->stringMsg[i] = (cUint8)pInMsgStream[uStreamIndex++];
        }
    }

    if (uMsgLen < uStreamIndex)/*解码过程只读取不写入，即使越界不会破环系统，所以溢出判断放到最后*/
    {
        return false;
    }

    return true;
}