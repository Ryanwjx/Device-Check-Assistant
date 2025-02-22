#ifndef _SIGNALSLOTCREATEWRAPPER_H
#define _SIGNALSLOTCREATEWRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void * CreateSignal0Wrapper(void);
void DeleteSignal0Wrapper(void * tSignal);
void ConnectSignal0SlotWrapper(void * Signal, void (*SlotFunc)(void));
void EmitSignal0Wrapper(void * Signal);

void * CreateSignal1Wrapper(void);
void DeleteSignal1Wrapper(void * tSignal);
void ConnectSignal1SlotWrapper(void * Signal, void (*SlotFunc)(void * Param));
void EmitSignal1Wrapper(void * Signal,void * data);

#ifdef __cplusplus
}
#endif //__cplusplus


#endif //_SIGNALSLOTCREATEWRAPPER_H