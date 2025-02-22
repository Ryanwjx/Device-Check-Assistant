#include "input_manager.h"
#include "stdio.h"
#include "string.h"
#include <pthread.h>

static struct list_head g_InputDevsList;

#define INPUT_BUFFER_LEN 10
static InputEvent g_EventBuffer[INPUT_BUFFER_LEN];
static int g_iRead = 0;		//下一读索引号
static int g_iWrite = 0;	//下一写索引号

static pthread_mutex_t g_tMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_tConVar = PTHREAD_COND_INITIALIZER;

//pthread_mutex_t g_MutexEventBuf;
//pthread_mutex_lock(&g_MutexEventBuf);
//pthread_mutex_unlock(&g_MutexEventBuf);


//int EventBufferIsFull(void)
//{
//	return (g_iRead == (g_iWrite + 1) % INPUT_BUFFER_LEN); 	//读位置=写位置+1，分一般情况和循环归零情况
//}
//
//int EventBufferIsEmpty(void)
//{
//	return (g_iRead == g_iWrite); //读写位置相等
//}
//
//int GetEventBuffer(PInputEvent ptEvent)
//{	
//	if(!EventBufferIsEmpty())
//	{
//		*ptEvent = g_EventBuffer[g_iRead++];
//
//		if (g_iRead % INPUT_BUFFER_LEN == 0)
//		{
//			g_iRead = 0;
//		}
//		return 0;
//	}
//	return -1;
//}
//
//int PutEventBuffer(PInputEvent ptEvent)
//{
//	if(!EventBufferIsFull())
//	{
//		g_EventBuffer[g_iWrite++] = *ptEvent;
//
//		if (g_iWrite % INPUT_BUFFER_LEN == 0)
//		{
//			g_iWrite = 0;
//		}
//		return 0;
//	}
//	return -1;
//}

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
	pthread_mutex_lock(&g_tMutex);
	pthread_cond_wait(&g_tConVar, &g_tMutex);  
	if(!EventBufferIsEmpty())
	{
		*ptEvent = g_EventBuffer[g_iRead++];

		if (g_iRead % INPUT_BUFFER_LEN == 0)
		{
			g_iRead = 0;
		}
		pthread_mutex_unlock(&g_tMutex);
		printf("really get event = %d\r\n",ptEvent->iType);
		return 0;
	}
	pthread_mutex_unlock(&g_tMutex);
	return -1;
}

int PutEventBuffer(PInputEvent ptEvent)
{
	pthread_mutex_lock(&g_tMutex);
	if(!EventBufferIsFull())
	{
		g_EventBuffer[g_iWrite++] = *ptEvent;

		if (g_iWrite % INPUT_BUFFER_LEN == 0)
		{
			g_iWrite = 0;
		}
		printf("input event = %d\r\n",ptEvent->iType);
		pthread_cond_signal(&g_tConVar);
		pthread_mutex_unlock(&g_tMutex);
		return 0;
	}
	pthread_mutex_unlock(&g_tMutex);
	return -1;
}


void RegisterInputDevice(PInputDevice       ptInputDev)
{
	list_add(&ptInputDev->InputDeviceList, &g_InputDevsList);
}

int InputSystemRegister(void)
{	
	INIT_LIST_HEAD(&g_InputDevsList);
	
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
	PInputDevice ptInputDevice;
	int ret;
	pthread_t tid;
	
	list_for_each_entry(ptInputDevice, &g_InputDevsList, InputDeviceList)
	{
		ret = ptInputDevice->DeviceInit();
		if (ret < 0)
		{	
			printf("DeviceInit err for %s \r\n",ptInputDevice->name);
			return -1;
		}
		ret = pthread_create(&tid, NULL, input_recv_thread_func, ptInputDevice);
		if (ret < 0)
		{
			printf("pthread_create err for %s \r\n",ptInputDevice->name);
			return -1;
		}
	}
	return 0;
}

int InputDeviceExit(void)
{
	PInputDevice ptInputDevice;
	int ret;
	
	list_for_each_entry(ptInputDevice, &g_InputDevsList, InputDeviceList)
	{
		ret = ptInputDevice->DeviceExit();
		if (ret < 0)
			return -1;
	}
	
	return 0;
}

int GetInputEvent(PInputEvent ptInputEvent)
{
	return GetEventBuffer(ptInputEvent);
}



