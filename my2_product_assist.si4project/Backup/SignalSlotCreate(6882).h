#ifndef _SIGNALSLOTCREATE_H
#define _SIGNALSLOTCREATE_H

void * CreateSignal0(void);
void DeleteSignal0(void * tSignal);
void ConnectSignal0Slot(void * Signal, void (*SlotFunc)(void));
void EmitSignal0(void * Signal);

#endif //_SIGNALSLOTCREATE_H