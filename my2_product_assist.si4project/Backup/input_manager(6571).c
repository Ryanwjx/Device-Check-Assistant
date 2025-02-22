#include "input_manager.h"
#include "stdio.h"
#include "string.h"
#include <pthread.h>

static PInputDevice g_InputDevs;

#define INPUT_BUFFER_LEN 10
static InputEvent g_EventBuffer[INPUT_BUFFER_LEN];
static int g_iRead = 0;		//下一读索引号
static int g_iWrite = 0;	//下一写索引号

int EventBufferIsFull(void)
{
	return (g_iRead == (g_iWrite + 1) % INPUT_BUFFER_LEN); 	//读位置=写位置+1，分一般情况和循环归零情况
}

int EventBufferIsEmpty(void)
{
	return (g_iRead == g_iWrite); //读写位置相等
}

int GetEventBuffer(PInputEvent ptEvent)
{
	if(!EventBufferIsEmpty())
	{
		*ptEvent = g_EventBuffer[g_iRead++];

		if (g_iRead % INPUT_BUFFER_LEN == 0)
		{
			g_iRead = 0;
		}
		
		return 0;
	}
	return -1;
}

int PutEventBuffer(PInputEvent ptEvent)
{
	if(!EventBufferIsFull())
	{
		g_EventBuffer[g_iWrite++] = *ptEvent;

		if (g_iWrite % INPUT_BUFFER_LEN == 0)
		{
			g_iWrite = 0;
		}
		
		return 0;
	}
	return -1;
}

void RegisterInputDevice(PInputDevice       ptInputDev)
{
	ptInputDev->ptNext = g_InputDevs;
	g_InputDevs = ptInputDev;
}

int InputSystemRegister(void)
{
	extern void NetRegisterInputDevice(void);
	extern void TSRegisterInputDevice(void);
	TSRegisterInputDevice();
	NetRegisterInputDevice();
	
	return 0;
}

void *input_recv_thread_func(void *arg)
{
	PInputDevice ptDev = (PInputDevice)arg;
	int ret;
	InputEvent Event;
		
	while (1)
	{
		ret = ptDev->GetInputEvent(&Event);
			
		if (!ret)
		{
			PutEventBuffer(&Event);
		}
	}
}

int InputDeviceInit(void)
{
	PInputDevice ptTmp = g_InputDevs;
	int ret;
	pthread_t tid;
	
	while(ptTmp)
	{	
		ret = ptTmp->DeviceInit();
		if (!ret)
		{
			printf("%s DeviceInit ok\n",ptTmp->name);
		}
		
		ret = pthread_create(&tid, NULL, input_recv_thread_func, ptTmp);
		if (!ret)
		{
			printf("%s pthread_create ok\n",ptTmp->name);
		}

		ptTmp = ptTmp->ptNext;
	}
	
	return 0;
}

int InputDeviceExit(void)
{
	PInputDevice ptTmp = g_InputDevs;
	int ret;
	
	while(ptTmp)
	{
		ret = ptTmp->DeviceExit();
		if (!ret)
			printf("%s DeviceExit ok\n",ptTmp->name);

		ptTmp = ptTmp->ptNext;
	}
	
	return 0;
}

int GetInputEvent(PInputEvent ptInputEvent)
{
	return GetEventBuffer(ptInputEvent);
}



