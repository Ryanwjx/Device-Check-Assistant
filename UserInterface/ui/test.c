#include "SignalSlotCreateWrapper.h"
#include "stdio.h"

void PrintFunc(void)
{
	printf("PrintFunc is Called!\r\n");
}

int main()
{
	void * Signal0 = CreateSignal0Wrapper();

	ConnectSignal0SlotWrapper(Signal0, &PrintFunc);

	EmitSignal0Wrapper(Signal0);

	DeleteSignal0Wrapper(Signal0);

	return 0;
}
