#ifndef _SIGNALSLOTWRAPPER_H
#define _SIGNALSLOTWRAPPER_H

#include "Delegate.h"
#include <set>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void * CreateSignal0(void);
void DeleteSignal0(void * tSignal);
void ConnectSignal0Slot(void * Signal, void (*SlotFunc)(void));
void EmitSignal0(void * Signal);

void * CreateSignal1(void);
void DeleteSignal1(void * tSignal);
void ConnectSignal1Slot(void * Signal, void (*SlotFunc)(void));
void EmitSignal1(void * Signal);


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus



#endif //_SIGNALSLOTWRAPPER_H
