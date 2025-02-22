#include "SignalSlotCreateWrapper.h"
#include "stdio.h"

void PrintFunc(void)
{
	printf("PrintFunc is Called!\r\n");
}

void main()
{
	void * Signal0 = CreateSignal0Wrapper();

	ConnectSignal0SlotWrapper(Signal0, &PrintFunc);

	EmitSignal0(Signal0);

	DeleteSignal0(Signal0);
}
