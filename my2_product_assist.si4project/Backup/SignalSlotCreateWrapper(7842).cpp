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

