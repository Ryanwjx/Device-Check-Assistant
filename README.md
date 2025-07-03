

# 1. 整体功能介绍

<img src="E:\0_wjx_file\embedded_ai\study_data\0 note\project\production_assistant\image-20241028155341206.png" alt="image-20241028155341206" style="zoom:67%;" />

```
实现一个硬件交互式检测系统，对于部分硬件如按键、网络等自动检测；对于led、喇叭手动触控检测。
```

# 2. 整体框架

![image-20241028155544350](E:\0_wjx_file\embedded_ai\study_data\0 note\project\production_assistant\image-20241028155544350.png)

```
程序框架分为三部分，输入、显示、页面，分别实现了各自的管理器与子模块

输入管理器与子模块：
	子模块实现：ops结构体（初始化、读取数据）
	管理器实现：子设备选择、子设备初始化、子设备数据自动读取
	=>子设备注册方法：子设备-注册函数，调用管理器-加入全局list的函数； 由于无法自动触发，管理器-设备注册函数，调用子设备注册
	=>环形缓冲区：环形缓冲区，空时iw=ir，满时(iw+1)%size==ir，读时上锁并用条件变量等待条件满足，写时上锁并发送条件满足信号
	
显示管理器与子模块
	子模块实现：ops结构体（初始化mmap映射framebuf、写数据函数）
	管理器实现：子设备选择、子设备初始化、写数据
	=>子设备注册方法：子设备-注册函数，调用管理器-加入全局list的函数； 由于无法自动触发，管理器-设备注册函数，调用子设备注册

ui组件
	basemodule提供基础信息
	label提供默认执行函数与事件处理函数
	button提供状态与事件处理函数，事件处理函数根据状态执行不同操作
	
页面管理与子页面
	子页面实现：页面结构体（name、初始化构建ui组件）
	管理器实现：页面选择、页面初始化（构建、调用默认函数）、页面显示（显ui组件）、页面刷新（持续捕获输入，根据输入刷新部分ui组件区域）
	=>子页面注册方法：子设备-注册函数，调用管理器-加入全局list的函数； 由于无法自动触发，管理器-设备注册函数，调用子设备注册
	
main函数调用流程
	初始化各个模块
	启动页面显示、页面刷新
```

# 3. 功能实现

``` 
创建一个页面，实现部分器件自动检测，部分器件手动检测

page init函数，进行ui模块创建，对于Label调用默认函数（创建线程，检测器件，并回环网发送数据到input模块）
page_manager的refresh函数，持续捕获输入，并更新输入调用ui模块的处理函数（label直接变色，button根据状态变色）
```

## 4.1 wifi

```
modprobe 8188eu

ifconfig wlan0 up

搜索
iwlist wlan0 scan

写入/etc/wpa_supplicant.conf
ctrl_interface=/var/run/wpa_supplicant
ap_scan=1
network={
ssid="wjx"
psk="12345678"
}

创建wpa的工作空间
mkdir /var/run/wpa_supplicant -p

建立连接
wpa_supplicant -D wext -c /etc/wpa_supplicant.conf -i wlan0 &

申请ip
udhcpc -i wlan0

ping -I 192.168.137.187 www.baidu.com  
//发现无法ping通，需要设置网关，并放在第一个！！！
//route add default gw 192.168.31.1
//default         XiaoQiang       0.0.0.0         UG    0      0        0 wlan0
//default         192.168.0.1     0.0.0.0         UG    0      0        0 eth0
//default         XiaoQiang       0.0.0.0         UG    10     0        0 wlan0
```

## 4.1 bind失败，端口占用

```
当服务器bind失败，可能原因是端口占用
struct sockaddr_in tSocketServerAddr;
int reuse = 1;

g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
if (g_iSocketServer == -1)
{
    printf("socket error!\n");
    return -1;
}
if (setsockopt(g_iSocketServer, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)	//设置socket属性，删除后立马可被使用
{
    perror("setsockopet error\n");
    return -1;
}
```

## 4.2声卡，音频等需要单点触发，有需要持续占用的程序

```
原来框架为：不可触碰按键使用线程执行，并返回信息至网络；可触碰按键直接在程序内执行，不可阻塞

但需要触碰的工作可能存在阻塞，比如音频播放、录音

尝试：
不修改框架，通过sh调用程序，在外部创建线程。但当程序结束后，线程死亡
修改框架，直接在内部创建线程并进行相应控制（原来的两态开关仍然可用，因为不同状态的线程之间互斥，彼此删除）
```

## 4.3 模块化设计---回环网

```
回环网络需要重复使用，直接将其定义为单独模块，通过system可调用
```

## 4.3 echo与led设备驱动

```
使用echo后，led的write被持续调用
因为写的设备驱动write并不规范，没有返回收到用户空间的实际字节数

另外，当echo 1 时，内部收到49
为了使得led设备驱动可被echo等指令、文件读写，需要进行 或 操作，多输入可控
if (status == 1 || status == '1')
    err = gpiod_direction_output(g_led_gpio, 1);
else if (status == 0 || status == '0')
    err = gpiod_direction_output(g_led_gpio, 0);
```

## 4.4 网络登录不稳定

```
setenv bootcmd 'mmc dev 0;fatload mmc 0:1 80800000 zImage;fatload mmc 0:1 83000000 imx6ull-alientek-emmc.dtb;setenv bootargs console=ttymxc0,115200 CONFIG_BOOTARGS_CMA_SIZE root=/dev/nfs rw nfsroot=192.168.0.198:/home/book/nfs_rootfs ip=192.168.0.200:192.168.0.198:192.168.0.1:255.255.255.0::eth0:off ;bootz 80800000 - 83000000'
```

## 4.5 数据访问不存在，考虑生命周期

```
根据PageInfo解析得到的Command信息存储在全局Buffer中，当Label需要执行DefaultFunc时可创建线程并出传入Buffer初始地址，可正常使用数据
而对于Button则需要根据全局Buffer再对末尾修改并存储在一个整体创建函数的局部变量中，当Button执行ProcessInput时创建线程并传入该局部变量，存在线程之间的顺序问题（原函数释放了对应地址数据）而需要加入顺序控制：可在创建线程函数中malloc，在线程任务中free就可以了。

原来也是类似架构，但是并未出现SegmentFault错误，可能是因为Buffer并未这么快被系统清空。而修改后框架Buffer分配过多，需要处理。
```

```
原来InputManager中定义全局InputBuffer，在统一的GetInput中定义局部InputEvent，将InputEvent的指针传递至底层获取底层局部InputEvent。实现数据生命周期的拉长。

现InputManager中定义了BaseInput的全局InputBuffer，在底层malloc完整的数据。故放弃！！！
```



## 4.6 互斥量与条件变量---阻塞链表

```
使用mutex可实现资源的互斥访问

使用cond可在mutex的基础上，判断是否满足条件
	满足则无影响
	不满足则解锁mutex，阻塞此线程等待完成，被外部唤醒条件满足后重新锁上mutex
	
int GetEventBuffer(PInputEvent ptEvent)
{	
	pthread_mutex_lock(&g_tMutex);
	pthread_cond_wait(&g_tConVar, &g_tMutex);  
	if(!EventBufferIsEmpty())
	{
		*ptEvent = g_EventBuffer[g_iRead++];

		if (g_iRead % INPUT_BUFFER_LEN == 0)
		{
			g_iRead = 0;
		}
		pthread_mutex_unlock(&g_tMutex);
		printf("really get event = %d\r\n",ptEvent->iType);
		return 0;
	}
	pthread_mutex_unlock(&g_tMutex);
	return -1;
}

int PutEventBuffer(PInputEvent ptEvent)
{
	pthread_mutex_lock(&g_tMutex);
	if(!EventBufferIsFull())
	{
		g_EventBuffer[g_iWrite++] = *ptEvent;

		if (g_iWrite % INPUT_BUFFER_LEN == 0)
		{
			g_iWrite = 0;
		}
		printf("input event = %d\r\n",ptEvent->iType);
		pthread_cond_signal(&g_tConVar);
		pthread_mutex_unlock(&g_tMutex);
		return 0;
	}
	pthread_mutex_unlock(&g_tMutex);
	return -1;
}
```

# 4. 编译

## 1. 交叉编译环境配置

```
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
export PATH=$PATH:/usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin
//也可将最后一个“交叉编译工具链目录，写入PATH中，则不需要每次都设置”
/opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi
```

## 2. Makefile文件解读

下述为顶层Makefile、递归调用Makefile.build、底层Makefile，大致配合流程为。

![image-20240905150534500](E:\0_wjx_file\embedded_ai\study_data\0 note\project\production_assistant\image-20240905150254408.png)

```
/* 顶层Makefile */
//声明编译器相关env
CROSS_COMPILE ?= 
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E						//仅预处理
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump
export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

//设置编译选项值
CFLAGS := -Wall -O2 -g						//设置输出all warning
CFLAGS += -I $(shell pwd)/include			//设置包含路径选项
LDFLAGS := -lts -lpthread -lfreetype -lm	//设置需要链接的库
export CFLAGS LDFLAGS

//设置顶部目录
TOPDIR := $(shell pwd)
export TOPDIR

//设置编译总target
TARGET := test

//设置编译目标obj-y
obj-y += display/
obj-y += unitest/

//1. all依赖start_recursive_build生成各级目录built-in.o；依赖$(TARGET)完成链接生成可执行
all : start_recursive_build $(TARGET)	//虽然start_recursive_build无实体，故每次都会调用，创建built-in.o
	@echo $(TARGET) has been built!

//2. 完成所有中间目标生成（关键部分由Makefile.build完成迭代创建）
start_recursive_build:					//创建built-in.o
	make -C ./ -f $(TOPDIR)/Makefile.build

//3. 完成总target链接，需要依赖built-in.o（由start_recursive_build生成）
$(TARGET) : built-in.o
	$(CC) -o $(TARGET) built-in.o $(LDFLAGS)

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)
	
```

```
/* 顶层Makefile.build */
//声明本地变量，部分从当前目录Makefile中继承值
__build :
obj-y :=
subdir-y :=
EXTRA_CFLAGS :=
include Makefile

//获取obj-y中子目录，如obj-y := a.o b.o c/ d/ => c d
# obj-y := a.o b.o c/ d/
# $(filter %/, $(obj-y))   : c/ d/
# __subdir-y  : c d
# subdir-y    : c d
__subdir-y	:= $(patsubst %/,%,$(filter %/, $(obj-y))) //obj-y中包含目录、文件，获取其中/结尾的子目录，去除/取得名字
subdir-y	+= $(__subdir-y)

//设置obj-y中子目录编译目标值，如c d => c/built-in.o d/built-in.o
subdir_objs := $(foreach f,$(subdir-y),$(f)/built-in.o) //去除subdir-y每一个，返回多个值，每个为subdir-y-i/built-in.o

//获取obj-y中.o目标，如obj-y := a.o b.o c/ d/ => a.o b.o
cur_objs := $(filter-out %/, $(obj-y)) 			//obj-y中包含目录、文件，去除其中/结尾的子目录，返回各文件

//设置obj-y中.o目标编译依赖值到.o.d文件，如a.o b.o => a.o.d b.o.d
//编译时，需要检查源文件依赖关系，否则依赖发生变换，目标可能不会重新编译
//编译器提供-M或者-MD等选项生成依赖关系如input_manager.o: input_manager.c...等
//这里指明依赖关系存储的名称为xxx.o.d
dep_files := $(foreach f,$(cur_objs),.$(f).d)	//尾部增加.d
dep_files := $(wildcard $(dep_files)) 			//保留实际存在的.o.d文件

//如果依赖文件存在，直接包含依赖文件中目录
ifneq ($(dep_files),)
  include $(dep_files)
endif

//1. PHONY为虚假总目标，实际是$(subdir-y)和built-in.o
.PHONY : __build $(subdir-y)	 
__build : $(subdir-y) built-in.o	//不直接.PHONY: $(subdir-y) built-in.o，会反复进入根目录

//3. $(subdir-y)各个子目录迭代调用此文件
$(subdir-y):
	make -C $@ -f $(TOPDIR)/Makefile.build //使用顶层的$(TOPDIR)/Makefile.build，编译顶层目录+各个子目录

//4. 当前目录总built-in.o依赖当前目录各.o文件；依赖各子目录的built-in.o
built-in.o : $(cur_objs) $(subdir_objs) 
	$(LD) -r -o $@ $^					//链接$@=built-in.o，从$^=$(cur_objs) $(subdir_objs) 

//5. 当前目录内.c编译，生成.o文件
dep_file = .$@.d
%.o : %.c 
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CFLAGS_$@) -Wp,-MD,$(dep_file) -c -o $@ $<	//-MD,$(dep_file)，其中dep_file表示一个取.o换成.o.d形式
	
```

```
//各非底层Makefile
EXTRA_CFLAGS  := 
CFLAGS_file.o := 

obj-y += disp_test.o	//继承到Makefile.build中
```

## 3. 子编写Makefile

```
SRCS = my_ap3216_test.c
OBJS = $(SRCS:.c=.o)
TARGETS = $(SRCS:.c=)
CC = arm-linux-gnueabihf-gcc			//如果没有，系统会自动调用cc
all: $(TARGETS)

%: %.o
	$(CC) -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGETS)
```

```
主Makefile添加
DEVICE_TEST:
	make -C ./device_test -f $(TOPDIR)/device_test/Makefile
clean:
	rm -f $(shell find -name "my_ap3216_test")
```

# 





