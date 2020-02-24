/*�ͻ���
�ͻ������ʲ۷�����Ϣ��Ҫ���ȴ��ʲۣ�Ȼ��ֱ��ʹ��WriteFile����д�롣���ʲ�ʹ��CreateFile�������ͻ����ڴ��ʲ�ʱ��Ҫ֪���ʲ�����*/
//#include <windows.h>
#include <stdio.h> 
#include "stdafx.h"

LPTSTR lpszSlotName = TEXT("\\\\.\\mailslot\\ycy_mailslot");		// mailslot��
static char mail_distance[2]; // ͨ�ŵ�����


//========================================== Ultrasonic a===========================================

HANDLE hCom_u; 
char lpOutBuffer_u[4];
DWORD dwBytesWrite_u=4;
COMSTAT ComStat_u;
DWORD dwErrorFlags_u;
BOOL bWriteStat_u;
static int distance_z = 0;
void Init_u(void);
void write_u(void);
void write_g(void);

HANDLE hCom_g; 
char lpOutBuffer_g[4];
DWORD dwBytesWrite_g=4;
COMSTAT ComStat_g;
DWORD dwErrorFlags_g;
BOOL bWriteStat_g;
float y_roll_angle = 0.0f;
float x_roll_angle = 0.0f;


int main()
{   
	mail_distance[0] = 0x00;
	mail_distance[1] = 0x00;
	DWORD cbWritten; 
	DWORD cbMessage;

	Init_u ();
	//Init_g();	
	//write_g();

	// ��mailslot
	HANDLE hFile = CreateFile(lpszSlotName, 
		GENERIC_WRITE,		// ��д
		FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES) NULL, 
		OPEN_EXISTING,		// ��һ���Ѿ����ڵ�mailslot��Ӧ���ɷ�����Ѿ�����
		FILE_ATTRIBUTE_NORMAL, 
		(HANDLE) NULL); 

	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		printf("CreateFile failed with %d.\n", GetLastError()); 
		return ; 
	}
	// ��mailslotд��

	while(1)
	{
		write_u();

		if(!WriteFile(hFile, 
			&mail_distance, 
			2,  
			&cbWritten, 
			(LPOVERLAPPED) NULL)) 
		{ 
			printf("WriteFile failed with %d.\n", GetLastError()); 
			return ; 
		} 
		distance_z = (mail_distance[0] << 8) + mail_distance[1];
		printf("z: %d \n", distance_z); 

		Sleep(50);
	}	
	
	CloseHandle(hFile);  // ���� 

	return ;
}


void Init_u(void)
{
	lpOutBuffer_u[0]=0x22;
	lpOutBuffer_u[1]=0x00;
	lpOutBuffer_u[2]=0x00;
	lpOutBuffer_u[3]=0x22;
	
		  hCom_u = CreateFile("COM4",  //COM1��
			  GENERIC_READ|GENERIC_WRITE, //�������д
			  0,  //��ռ��ʽ
			  NULL,
			  OPEN_EXISTING,  //�򿪶����Ǵ���
			  //ͬ��ģʽ
			  0,
			  NULL);
		  if(hCom_u == INVALID_HANDLE_VALUE)
		  {
			  AfxMessageBox("��COMʧ��!");
		  }
		  else
		  {
			  //AfxMessageBox("��COM�ɹ�!");
			  SetupComm(hCom_u,4,4); //���뻺����������������Ĵ�С����4
			  COMMTIMEOUTS TimeOuts;
			  GetCommTimeouts(hCom_u, &TimeOuts);
			  //�趨����ʱ
			  TimeOuts.ReadIntervalTimeout = 1000;
			  TimeOuts.ReadTotalTimeoutMultiplier = 500;
			  TimeOuts.ReadTotalTimeoutConstant = 5000;
			  //�趨д��ʱ
			  TimeOuts.WriteTotalTimeoutMultiplier = 500;
			  TimeOuts.WriteTotalTimeoutConstant = 2000;
			  SetCommTimeouts(hCom_u, &TimeOuts); //���ó�ʱ
			  
			  DCB dcb;
			  GetCommState(hCom_u, &dcb);
			  dcb.BaudRate = 9600; //������Ϊ9600
			  dcb.ByteSize = 8; //ÿ���ֽ���8λ
			  dcb.Parity = NOPARITY; //����żУ��λ
			  dcb.StopBits = ONESTOPBIT; //һ��ֹͣλ
			  SetCommState(hCom_u, &dcb);
			  
			  PurgeComm(hCom_u,PURGE_TXCLEAR|PURGE_RXCLEAR);
		  }
}


void write_u(void)
{
	ClearCommError(hCom_u,&dwErrorFlags_u,&ComStat_u);

	bWriteStat_u = WriteFile(hCom_u, lpOutBuffer_u, dwBytesWrite_u, &dwBytesWrite_u, NULL);
	if(!bWriteStat_u)
	{
		AfxMessageBox("д���� 4 ʧ��!");
	}
	PurgeComm(hCom_u, PURGE_TXABORT|
			  PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);

	BYTE rxdata_u[4];
	CString strtemp_u;
	DWORD wCount_u;//��ȡ���ֽ���
	BOOL bReadStat_u;
	bReadStat_u = ReadFile(hCom_u,rxdata_u,4,&wCount_u,NULL);
	if(!bReadStat_u)
	{
		AfxMessageBox("������ 4 ʧ��!");
	}
	
	mail_distance[0] = rxdata_u[1];
	mail_distance[1] = rxdata_u[2];
}

//========================================== gyro a===========================================


void Init_g(void)
{
	lpOutBuffer_g[0]=0x24;
	lpOutBuffer_g[1]=0x32;
	lpOutBuffer_g[2]=0x00;
	lpOutBuffer_g[3]=0x56;
	// TODO: Add your control notification handler code here
	hCom_g = CreateFile("COM1",  //COM1��
		GENERIC_READ|GENERIC_WRITE, //�������д
		0,  //��ռ��ʽ
		NULL,
		OPEN_EXISTING,  //�򿪶����Ǵ���
		//FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, //�ص���ʽ
		0,
		NULL);
	if(hCom_g == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("��COMʧ��!");
	}
	else
	{
		//AfxMessageBox("��COM�ɹ�!");
		SetupComm(hCom_g,11,4); //���뻺����������������Ĵ�С����1024
		COMMTIMEOUTS TimeOuts;
		GetCommTimeouts(hCom_g, &TimeOuts);
		//�趨����ʱ
		TimeOuts.ReadIntervalTimeout = 1000;
		TimeOuts.ReadTotalTimeoutMultiplier = 500;
		TimeOuts.ReadTotalTimeoutConstant = 5000;
		//�趨д��ʱ
		TimeOuts.WriteTotalTimeoutMultiplier = 500;
		TimeOuts.WriteTotalTimeoutConstant = 2000;
		SetCommTimeouts(hCom_g, &TimeOuts); //���ó�ʱ
		
		DCB dcb;
		GetCommState(hCom_g, &dcb);
		dcb.BaudRate = 57600; //������Ϊ9600
		dcb.ByteSize = 8; //ÿ���ֽ���8λ
		dcb.Parity = NOPARITY; //����żУ��λ
		dcb.StopBits = ONESTOPBIT; //һ��ֹͣλ
		SetCommState(hCom_g, &dcb);
		
		PurgeComm(hCom_g,PURGE_TXCLEAR|PURGE_RXCLEAR);
	}
}

//========================================== gyro b ====================


//========================================== gyro a===========================================
void write_g(void)
{
	ClearCommError(hCom_g,&dwErrorFlags_g,&ComStat_g);

	bWriteStat_g = WriteFile(hCom_g, lpOutBuffer_g, dwBytesWrite_g, &dwBytesWrite_g, NULL);
	if(!bWriteStat_g)
	{
		AfxMessageBox("д���� 1 ʧ��!");
	}
	PurgeComm(hCom_g, PURGE_TXABORT|
			  PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);

	BYTE rxdata_g[11];
	CString strtemp_g;
	DWORD wCount_g;//��ȡ���ֽ���
	BOOL bReadStat_g;
	bReadStat_g = ReadFile(hCom_g,rxdata_g,11,&wCount_g,NULL);
	if(!bReadStat_g)
	{
		AfxMessageBox("������ 1 ʧ��!");
	}
	else
	{
		unsigned char * p_y_roll = (unsigned char *) &y_roll_angle;
		unsigned char * p_x_roll = (unsigned char *) &x_roll_angle;
		p_y_roll[0] = rxdata_g[3];
		p_y_roll[1] = rxdata_g[4];
		p_y_roll[2] = rxdata_g[5];
		p_y_roll[3] = rxdata_g[6];
		
		p_x_roll[0] = rxdata_g[7];
		p_x_roll[1] = rxdata_g[8];
		p_x_roll[2] = rxdata_g[9];
		p_x_roll[3] = rxdata_g[10];			
	}
}
//========================================== gyro b ====================