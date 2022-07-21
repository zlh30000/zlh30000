#include <stdio.h>
#include <memory.h>
#include "code.h"

cUint8 g_array[1000];

int main(int argc, char** argv)
{
	Interface_Message stIMessage;
	memset(&stIMessage, 0, sizeof(Interface_Message));
	Interface_Message stOMessage;
	memset(&stOMessage, 0, sizeof(Interface_Message));
	
	cUint32 lengthsizeof = sizeof(Interface_Message);

	stIMessage.uEncodeDirectField = UAV_MSG_IE_POS_VEL_CMD | UAV_MSG_IE_VELOCITY_LIMIT;
	stIMessage.enUavCmd = UAV_COMMAND_MEMBER_START;
	stIMessage.stPosVelCmd.stPoint.x = 100;
	stIMessage.stPosVelCmd.stPoint.y = 80;
	stIMessage.stPosVelCmd.stPoint.z = 10;
	stIMessage.velocityLimit = 3;


	memset(g_array, 0, 1000);

	cUint32 length = fnCluster_codec_EncodeInterfaceMsg(g_array, &stIMessage);

	fnCluster_codec_DecodeInterfaceMsg(g_array, &stOMessage, length);

	return 0;
}


