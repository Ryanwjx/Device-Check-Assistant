#include <stdio.h>
#include "SignalSlotCreateWrapper.h"
#include "SignalSlotCreate.h"

void * CreateSignal0Wrapper(void)
{
	return CreateSignal0();
}

void DeleteSignal0Wrapper(void * tSignal)
{
	DeleteSignal0(tSignal);
}

void ConnectSignal0SlotWrapper(void * Signal, void (*SlotFunc)(void))
{
	ConnectSignal0Slot(Signal, SlotFunc);
}

void EmitSignal0Wrapper(void * Signal)
{
	EmitSignal0(Signal);
}

void * CreateSignal1Wrapper(void)
{
	return CreateSignal1();
}

void DeleteSignal1Wrapper(void * tSignal)
{
	DeleteSignal1(tSignal);
}

void ConnectSignal1SlotWrapper(void * Signal, void (*SlotFunc)(void *))
{
	ConnectSignal1Slot(Signal, SlotFunc);
}

void EmitSignal1Wrapper(void * Signal,void * data)
{
	EmitSignal1(Signal,data);
}

