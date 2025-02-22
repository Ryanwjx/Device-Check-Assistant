#ifndef _SIGNALSLOTCREATE_H
#define _SIGNALSLOTCREATE_H

void * CreateSignal0(void);
void DeleteSignal0(void * tSignal);
void ConnectSignal0Slot(void * Signal, void (*SlotFunc)(void));
void EmitSignal0(void * Signal);

void * CreateSignal1(void);
void DeleteSignal1(void * tSignal);
void ConnectSignal1Slot(void * Signal, void (*SlotFunc)(void * Param));
void EmitSignal1(void * Signal,void * data);

#endif //_SIGNALSLOTCREATE_H