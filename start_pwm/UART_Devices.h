#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/*  字符串库 bzero() */
#include <unistd.h>		/*	UNIX标准库 sleep() */
#include <fcntl.h>		/*	文件控制库 open() */
#include <termios.h>
#include <iostream>
using namespace std;

#define uchar unsigned char
#define uint unsigned int

class UART_Devices		/* 串口设备基类 */
{
public:
	UART_Devices();
	~UART_Devices();
	void UARTInit(char* DeviceName, uint BaudRate);
	int GetData(uchar* SendBuf, uchar* RecvBuf, int SendByteSize, int RecvByteSize);
private:
	int fd;
	uint m_BaudRate;
	struct termios Newtio, Oldtio;
};

/*************************************************************************
** 函数名称：UART_Devices()
** 功能简述：构造函数，设备文件描述符赋值为-1
** 输　  入：无
** 输    出：无
** 说    明：无
** 注    意：无
**************************************************************************/
UART_Devices::UART_Devices()
{
	fd = -1;
	m_BaudRate = 0;
}

/*************************************************************************
** 函数名称：~UART_Devices()
** 功能简述：析构函数，关闭串口设备文件
** 输　  入：无
** 输    出：无
** 说    明：无
** 注    意：无
**************************************************************************/
UART_Devices::~UART_Devices()
{
	close(fd);
}

/*************************************************************************
** 函数名称：UARTInit(char* DeviceName, uint BaudRate)
** 功能简述：Linux串口通信初始化
** 输　  入：设备文件名称及要设置的传输波特率
** 输    出：无
** 说    明：1、打开串口设备文件，默认为阻塞读方式
**			 2、保存原有串口配置至Oldtio中，清零Newtio
**			 3、位激活本地连接CLOCAL和接受使能CREAD
**			 4、设置波特率，默认为9600，输入和输出一样
**			 5、设置传输位数-8位
**			 6、设置奇偶校验位-无校验位
**			 7、设置停止位-1位
**			 8、设置最少字符和等待时间-0
**			 9、处理要写入的引用对象
**			 10、激活配置，改变的配置立即生效
** 注    意：无
**************************************************************************/
void UART_Devices::UARTInit(char* DeviceName, uint BaudRate)
{
	/* F1 */
	fd = open(DeviceName, O_RDWR | O_NOCTTY);
	if(-1 == fd)
    {
		perror("Open Devcie\n");
		exit(0);
    }
	/* F2 */
	tcgetattr(fd, &Oldtio);
	bzero(&Newtio,sizeof(Newtio));
	/* F3 */
	Newtio.c_cflag |= (CLOCAL | CREAD);
	/* F4 */
	m_BaudRate = BaudRate;
	switch( m_BaudRate )
    {
        case 2400:
            cfsetispeed(&Newtio, B2400);
            cfsetospeed(&Newtio, B2400);
			break;
        case 4800:
            cfsetispeed(&Newtio, B4800);
            cfsetospeed(&Newtio, B4800);
			break;
        case 9600:
            cfsetispeed(&Newtio, B9600);
            cfsetospeed(&Newtio, B9600);
			break;
        case 38400:
            cfsetispeed(&Newtio, B38400);
            cfsetospeed(&Newtio, B38400);
			break;
        case 115200:
            cfsetispeed(&Newtio, B115200);
            cfsetospeed(&Newtio, B115200);
			break;
        case 460800:
            cfsetispeed(&Newtio, B460800);
            cfsetospeed(&Newtio, B460800);
			break;
        default:
            cfsetispeed(&Newtio, B9600);
            cfsetospeed(&Newtio, B9600);
			break;
    }
	/* F5 */
	Newtio.c_cflag &= ~CSIZE; 
	Newtio.c_cflag |= CS8;
	/* F6 */
	Newtio.c_cflag &= ~PARENB;	
	/* F7 */
	Newtio.c_cflag &= ~CSTOPB;
	/* F8 */
	Newtio.c_cc[VMIN] = 0;
	Newtio.c_cc[VTIME] = 0;
	/* F9 */
	tcflush(fd, TCIFLUSH);
	/* F10 */
	tcsetattr(fd,TCSANOW,&Newtio);
}

/*************************************************************************
** 函数名称：GetData(uchar* SendBuf, uchar* RecvBuf, int RecvByteSize)
** 功能简述：获取数据
** 输　  入：1、要发送数据的指针
**			 2、接受数据的指针
**			 3、接受数据的字节数
** 输    出：若接收成功，返回接收字节数；否则返回-1
** 说    明：无
** 注    意：无
**************************************************************************/
int UART_Devices::GetData(uchar* SendBuf, uchar* RecvBuf, int SendByteSize, int RecvByteSize)
{
	int num;
	write(fd, SendBuf, SendByteSize);
	num = read(fd, RecvBuf, RecvByteSize);
	if(RecvByteSize == num)
	{
//		fprintf(stdout, "Read Succeed!\n");
		return RecvByteSize;
	}
	else
	{
//		fprintf(stderr, "Read Fail!\n");
		return -1;
	}
}
