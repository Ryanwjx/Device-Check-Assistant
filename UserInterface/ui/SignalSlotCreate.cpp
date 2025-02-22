#include <stdio.h>
#include "SignalSlotCreate.h"
#include "Delegate.h"
#include "Signal.h"

using namespace Gallant;

void * CreateSignal0(void)
{
	return new Signal0<void>();
}

void DeleteSignal0(void * tSignal)
{
	return delete (Signal0<void>*)tSignal;
}

void ConnectSignal0Slot(void * Signal, void (*SlotFunc)(void))
{
	Signal0<void>* ptSignal = (Signal0<void>*)Signal;
	ptSignal->Connect( SlotFunc );
}

void EmitSignal0(void * Signal)
{
	Signal0<void>* ptSignal = (Signal0<void>*)Signal;
	(*ptSignal)();
}

void * CreateSignal1(void)
{
	return new Signal1<void *>();
}

void DeleteSignal1(void * tSignal)
{
	return delete (Signal1<void *>*)tSignal;
}

void ConnectSignal1Slot(void * Signal, void (*SlotFunc)(void *))
{
	Signal1<void *>* ptSignal = (Signal1<void *>*)Signal;
	ptSignal->Connect( SlotFunc );
}

void EmitSignal1(void * Signal,void * data)
{
	Signal1<void *>* ptSignal = (Signal1<void *>*)Signal;
	(*ptSignal)(data);
}

