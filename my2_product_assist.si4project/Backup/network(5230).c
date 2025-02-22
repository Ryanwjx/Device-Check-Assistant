#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#include "input_manager.h"
#include "stdio.h"

#define SERVER_PORT 8888

static int g_iSocketServer;

int NetDeviceInit(void)
{	
	int iRet;
	struct sockaddr_in tSocketServerAddr;
	int reuse = 1;
	
	g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
	if (g_iSocketServer == -1)
	{
		printf("socket error!\n");
		return -1;
	}
	if (setsockopt(g_iSocketServer, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
	{
		perror("setsockopet error\n");
		return -1;
	}
	
	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	memset(tSocketServerAddr.sin_zero, 0, 8);

	iRet = bind(g_iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (iRet == -1)
	{
		perror("Bind failed");
		return -1;
	}

	printf("Net Ready!\n");
	return 0;
}

int NetDeviceExit(void)
{
	close(g_iSocketServer); 
	return 0;
}

int NetGetInputEvent(PInputEvent ptInputEvent)
{
	struct sockaddr_in tSocketClientAddr;
	int iRecvLen;
	char aRecvBuf[1000];
	
	unsigned int iAddrLen = sizeof(struct sockaddr);
	
	iRecvLen = recvfrom(g_iSocketServer, aRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
	if (iRecvLen > 0)
	{
		aRecvBuf[iRecvLen] = '\0';
		printf("Get Msg From %s : %s\n", inet_ntoa(tSocketClientAddr.sin_addr), aRecvBuf);
		ptInputEvent->iType 	= INPUT_TYPE_NET;
		gettimeofday(&ptInputEvent->iTime, NULL);
		strncpy(ptInputEvent->str, aRecvBuf, 1000);
		ptInputEvent->str[999] = '\0';
		return 0;
	}
	else
		return -1;

}


static InputDevice g_NetWork = {
	.name 			= "network",
	.GetInputEvent 	= NetGetInputEvent,
	.DeviceInit		= NetDeviceInit,
	.DeviceExit		= NetDeviceExit,
};

void NetRegisterInputDevice(void)
{
	RegisterInputDevice(&g_NetWork);
}


