#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "display_manager.h"

static struct fb_var_screeninfo var;
static unsigned char * fb_base;
static unsigned int screen_bytes;
static int fd_fb;

int FrameBufferDeviceInit(void)
{
 	unsigned int pixel_bytes;
	unsigned int x_bytes;

	fd_fb = open("/dev/fb0", O_RDWR);
	if (fd_fb < 0)
	{
		printf("can't open /dev/fb0\n");
		return -1;
	}

	if (ioctl(fd_fb, FBIOGET_VSCREENINFO,&var))
	{
		printf("can't get fb_screeninfo\n");
		return -1;
	}
	pixel_bytes = var.bits_per_pixel /8;
	x_bytes = var.xres * pixel_bytes;
	screen_bytes = var.yres * x_bytes;
	
	fb_base = (unsigned char *)mmap(NULL, screen_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if(fb_base == (unsigned char *)-1)
	{
		printf("can't mmap\n");
		return -1;
	}

	return 0;	
}

int FrameBufferDeviceExit(void)
{
	munmap(fb_base,screen_bytes);
	close(fd_fb);
	return 0;
}

int FrameBufferGetBuffer(PDispBuff ptDispBuff)
{
	ptDispBuff->Buff 	= (char *)fb_base;
	ptDispBuff->iBpp 	= var.bits_per_pixel;
	ptDispBuff->iXres	= var.xres;
	ptDispBuff->iYres	= var.yres;
	return 0;
}

int FrameBufferFlushRegion(PRegion ptRegion)
{
	return 0;
}

static DispOpr g_FrameBufferDisOpr={
	.name 			= "fb",
	.DeviceInit		= FrameBufferDeviceInit,
	.DeviceExit		= FrameBufferDeviceExit,
	.GetBuffer		= FrameBufferGetBuffer,
	.FlushRegion	= FrameBufferFlushRegion,
};

PDispOpr FrameBufferRegister(void)
{
	return &g_FrameBufferDisOpr;
}

