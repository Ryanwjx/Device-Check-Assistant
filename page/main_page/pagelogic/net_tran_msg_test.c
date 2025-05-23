#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/ioctl.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include <poll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

#define SERVER_PORT 8888


int main(int argc, char *argv[])
{
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;
	int iSendLen;
	
	if (argc != 3)
	{
		printf("Usage:%s <%s> <%s>\r\n", argv[0],"ip","option");
		return -1;
	}

	iSocketClient = socket(AF_INET, SOCK_DGRAM, 0);
	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
	if (0 == inet_aton(argv[1], &tSocketServerAddr.sin_addr))
	{
		printf("invalid server_ip\r\n");
		return -1;
	}
	memset(tSocketServerAddr.sin_zero, 0, 8);

	loop:
	iSendLen = sendto(iSocketClient, argv[2], strlen(argv[2]), 0,
		              (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (iSendLen < 0)
	{
		printf("sendto err\r\n");
		goto loop;
	}

	printf("Send Msg \"%s\" to %s\r\n",argv[2],argv[1]);

	close(iSocketClient);
	return 0;
}

