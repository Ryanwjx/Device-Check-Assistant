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


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <string.h>


#define SERVER_PORT 8888

int uart_init(int fd)
{
	struct termios options;
	
	if  (tcgetattr(fd, &options) < 0) {
        return -1;
    }
	// 修改控制模式，保证程序不会占用串口
    options.c_cflag  |=  CLOCAL;

    // 修改控制模式，能够从串口读取数据
    options.c_cflag  |=  CREAD;

    // 不使用流控制
    options.c_cflag &= ~CRTSCTS;

    // 设置数据位
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // 设置奇偶校验位
    options.c_cflag &= ~PARENB;
    options.c_iflag &= ~INPCK; 

    // 设置停止位
    options.c_cflag &= ~CSTOPB;

    // 设置最少字符和等待时间
    options.c_cc[VMIN] = 1;     // 读数据的最小字节数
    options.c_cc[VTIME]  = 0;   //等待第1个数据，单位是10s
    
    // 修改输出模式，原始数据输出
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // 设置波特率
    cfsetispeed(&options, B115200); 
    cfsetospeed(&options, B115200);

    // 清空终端未完成的数据
    tcflush(fd, TCIFLUSH);

    // 设置新属性
    if(tcsetattr(fd, TCSANOW, &options) < 0) {
        return -1;
    }

	return 0;
}

int main(int argc, char *argv[])
{
	int fd;
	char ch[100];
	int ret = 0;
	char data[100];
	memset(data,0,sizeof(data));
	memset(ch,0,sizeof(ch));
	
	while (1) 
	{
		fd = open("/dev/ttymxc2", O_RDWR);
		if(fd < 0) {
			perror("can't open /dev/ttymxc2\r\n");
			goto err;
		}
		else
		{
			fcntl(fd, F_SETFL, 0);
		}
		
		ret = uart_init(fd);
		if (fd < 0)
		{
			printf("uart_init err \r\n");
			close(fd);
			goto err;
		}

		while (1)
		{	
			printf("Testing Uart ...\r\n");
			sprintf(data,"Testing Uart Ready\r\n");
			ret = write(fd, data, sizeof(data));
			if (ret < 0)
			{
				perror("write err\r\n");
				goto errin;
			}
			
			ret = read(fd, ch, sizeof(ch));
			if (ret < 0)
			{
				perror("read err\r\n");
				goto errin;
			}
			
			printf("get uart msg:%s\r\n",ch);
			
			if(strcmp(ch ,"uart test") == 0) { 			/* 数据读取成功 */
				printf("uart data get ok\r\n");
				system("./page/main_page/pagelogic/net_tran_msg_test 127.0.0.1 serial");
				close(fd);
				return 0;
			}
			memset(ch,0,sizeof(ch));
			errin: sleep(1);
		}
		err: sleep(1);
	}
	
	return 0;
}

