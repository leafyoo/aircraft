#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/*  �ַ����� bzero() */
#include <unistd.h>		/*	UNIX��׼�� sleep() */
#include <fcntl.h>		/*	�ļ����ƿ� open() */
#include <termios.h>
#include <iostream>
using namespace std;

#define uchar unsigned char
#define uint unsigned int

class UART_Devices		/* �����豸���� */
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
** �������ƣ�UART_Devices()
** ���ܼ��������캯�����豸�ļ���������ֵΪ-1
** �䡡  �룺��
** ��    ������
** ˵    ������
** ע    �⣺��
**************************************************************************/
UART_Devices::UART_Devices()
{
	fd = -1;
	m_BaudRate = 0;
}

/*************************************************************************
** �������ƣ�~UART_Devices()
** ���ܼ����������������رմ����豸�ļ�
** �䡡  �룺��
** ��    ������
** ˵    ������
** ע    �⣺��
**************************************************************************/
UART_Devices::~UART_Devices()
{
	close(fd);
}

/*************************************************************************
** �������ƣ�UARTInit(char* DeviceName, uint BaudRate)
** ���ܼ�����Linux����ͨ�ų�ʼ��
** �䡡  �룺�豸�ļ����Ƽ�Ҫ���õĴ��䲨����
** ��    ������
** ˵    ����1���򿪴����豸�ļ���Ĭ��Ϊ��������ʽ
**			 2������ԭ�д���������Oldtio�У�����Newtio
**			 3��λ���������CLOCAL�ͽ���ʹ��CREAD
**			 4�����ò����ʣ�Ĭ��Ϊ9600����������һ��
**			 5�����ô���λ��-8λ
**			 6��������żУ��λ-��У��λ
**			 7������ֹͣλ-1λ
**			 8�����������ַ��͵ȴ�ʱ��-0
**			 9������Ҫд������ö���
**			 10���������ã��ı������������Ч
** ע    �⣺��
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
** �������ƣ�GetData(uchar* SendBuf, uchar* RecvBuf, int RecvByteSize)
** ���ܼ�������ȡ����
** �䡡  �룺1��Ҫ�������ݵ�ָ��
**			 2���������ݵ�ָ��
**			 3���������ݵ��ֽ���
** ��    ���������ճɹ������ؽ����ֽ��������򷵻�-1
** ˵    ������
** ע    �⣺��
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
