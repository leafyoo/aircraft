#include <stdio.h>				
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

#define TCP 0
#define UDP 1
#define SERVER 0
#define CLIENT 1

#define DEBUG 1
	
/*************************************************************************
** �ر�˵��: ����ΪLinux�汾
** �޸�֮��: 1������Ҫͷ�ļ�winsock2.h
**	   2����������WS2_32.lib��
**	   3��SOCKADDR_IN��SOCKADDR����Сд
**	   4�����������׽��ֿ�
**	   5��AddrSrv.sin_addr.S_un.s_addr��ΪAddrSrv.sin_addr.s_addr
**	   6��AddrLen����������int��Ϊaddrlen_t
**************************************************************************/	

class MySocket					
{
public:
	MySocket();
	MySocket(bool Type);
	MySocket(bool Type, bool Role);
	~MySocket();
	void SocketInit(char* IP_addr, int Port);	
	void LinkTarget(char* IP_addr, int Port);
	void LinkWait(void);	
	void Send(char* SendBuf);										
	void Recv(char* RecvBuf);										
	void SendToTarget(char *IP_addr, int Port, char* SendBuf);		
	void RecvFromTarget(char *IP_addr, int Port, char* RecvBuf);	
private:
	bool ComType;				
	bool ComRole;
	int SockSrv;			
	int SockCon;		
	/* Srv */
	sockaddr_in AddrSrv;	
	char IPSrv[20];			
	int PortSrv;			
	/* Client */
	sockaddr_in AddrClient;	
	char IPClient[20];		
	int PortClient;			
	socklen_t AddrLen;		
};
	
MySocket::MySocket()
{

}
/*************************************************************************
** �������ƣ�MySocket(bool Type)
** ���ܼ��������캯������1����ɲ���˽�б�����ͨ��Э�����ͼ���ɫ�ĳ�ʼ��
** �䡡  �룺ͨ��Э������UDP
** ��    ������
** ˵    ����TCPΪ0��UDPΪ1
** ע    �⣺1��IP��ַ����Ϊ��*IPSrv = 0
**	    2����ѡ��TCP����ͨ�Ž�ɫĬ��ΪServer
**************************************************************************/
MySocket::MySocket(bool Type)
{
	ComType = Type;
	ComRole = 0;
	*IPSrv = 0;					
	PortSrv = 0;					
	*IPClient = 0;					
	PortClient = 0;
	AddrLen = sizeof(sockaddr);			
}

/*************************************************************************
** �������ƣ�MySocket(bool Type, bool Role)
** ���ܼ��������캯������2����ɲ���˽�б�����ͨ��Э�����ͼ���ɫ�ĳ�ʼ��
** �䡡  �룺ͨ��Э������TCP��UDP��ͨ�Ž�ɫ����Server��Client
** ��    ������
** ˵    ����1��TCPΪ0��UDPΪ1
**	    2��SERVERΪ0��CLIENTΪ1
** ע    �⣺��
**************************************************************************/
MySocket::MySocket(bool Type, bool Role)
{
	ComType = Type;
	ComRole = Role;	
	*IPSrv = 0;					
	PortSrv = 0;					
	*IPClient = 0;					
	PortClient = 0;
	AddrLen = sizeof(sockaddr);	
}

/*************************************************************************
** �������ƣ�~MySocket()
** ���ܼ����������������Ͽ��׽��ֲ����˽�б������ͷ�
** �䡡  �룺��
** ��    ������
** ˵    ������
** ע    �⣺��
**************************************************************************/
MySocket::~MySocket()
{
	close(SockSrv);	
	close(SockCon);
	/* Server */
	PortSrv = 0;					
	memset(IPSrv, 0, strlen(IPSrv));			
	memset(&AddrSrv, 0, sizeof(AddrSrv));		
	/* Client */
	PortClient = 0;					
	memset(IPClient, 0, strlen(IPClient));	
	memset(&AddrClient, 0, sizeof(AddrClient));
}

/*************************************************************************
** �������ƣ�SocketInit(char* IP_addr, int Port)
** �����������׽��ֳ�ʼ��
** �䡡  �룺����IP��ַ���˿ں�
** ��    ������
** ˵    ����F1��˽�б���IPSrv��PortSrv���и�ֵ
**	    F2�����������ַ��Ϣע��
**	    F3������׽��ֵĽ���
**	    F4������׽��ֵİ�
**	    F5�������׽��ּ������г���Ϊ5
** ע    �⣺1������������IP���˿�
**	    2��F2��IP��ֵ���⣬��ʼ��*IPSrv = *IP_addr��recvfrom����������
**	    3��һ��Ҫ�����׽��ֿ⣬����recvfrom��������
**************************************************************************/
void MySocket::SocketInit(char* IP_addr, int Port)		
{
	/* F1 */
	sprintf(IPSrv,"%s",IP_addr);	
	PortSrv = Port;
	/* F2 */
	AddrSrv.sin_family = AF_INET;				
    	AddrSrv.sin_port = htons(Port);
    	AddrSrv.sin_addr.s_addr = inet_addr(IPSrv);
	/* F3 */
	if(0 == ComType)
		SockSrv = socket(AF_INET,SOCK_STREAM,0);
	if(1 == ComType)
		SockSrv = socket(AF_INET,SOCK_DGRAM,0);	
	/* F4 */
	bind(SockSrv,(sockaddr*)&AddrSrv,AddrLen);
	/* F5 */
	if ((0 == ComType) && (0 == ComRole))
	{
		listen(SockSrv, 5);
		printf("listen...\n");
	}
}

/*************************************************************************
** �������ƣ�LinkTarget(char* IP_addr, int Port)
** ������������Ŀ�꽨������
** �䡡  �룺Ŀ��IP��ַ���˿ں�
**	    ��������IP��ַ���˿ںţ�TCP��
** ��    ������
** ˵    ����1��˽�б���IPClient��PortClient���и�ֵ
**	    2�����Ŀ���ַ��Ϣע��
** ע    �⣺����������ַ�����AddrClient�У�TCP��
**************************************************************************/
void MySocket::LinkTarget(char* IP_addr, int Port)
{
	/* F1 */
	sprintf(IPClient,"%s",IP_addr);
	PortClient = Port;
	/* F2 */
	AddrClient.sin_family = AF_INET;				
    	AddrClient.sin_port = htons(PortClient);
	AddrClient.sin_addr.s_addr = inet_addr(IPClient);
	if(0 == ComType)
	{
		printf("connect...");
		connect(SockSrv,(sockaddr*)&AddrClient,AddrLen);
	}
}

/*************************************************************************
** �������ƣ�LinkWait(void)
** ���ܼ��������������׽���SockCon�������ȴ�����
** �䡡  �룺��
** ��    ������
** ˵    ����TCPʹ��
** ע    �⣺δ�ȵ����ӽ��ڴ�����
**************************************************************************/
void MySocket::LinkWait(void)
{
	if(0 == ComType)
		SockCon = accept(SockSrv,(sockaddr*)&AddrClient,&AddrLen);
}

/*************************************************************************
** �������ƣ�Send(char* SendBuf)
** ���ܼ��������ݷ���
** �䡡  �룺���Ҫ���͵����ݵĻ�������ָ��
** ��    ������
** ˵    ������
** ע    �⣺SERVERΪ0��CLIENTΪ1
**************************************************************************/
void MySocket::Send(char* SendBuf)
{
	int size;
	size = strlen(SendBuf) + 1;
	if( (0 == ComType) && (0 == ComRole) )
		send(SockCon,SendBuf,size,0);
	if( (0 == ComType) && (1 == ComRole) )
		send(SockSrv,SendBuf,size,0);
	if(1 == ComType)
		sendto(SockSrv, SendBuf, size, 0, (sockaddr*)&AddrClient, AddrLen);
}

/*************************************************************************
** �������ƣ�Recv(char* RecvBuf)
** ���ܼ��������ݽ���
** �䡡  �룺�������ݵĻ�����ָ��
** ��    ������
** ˵    �������ô˺����Ὣ���շ���ַ��Ϣ����AddrClient���ݽṹ��
** ע    �⣺���պ���������100Ϊ�������ַ�����Ĭ��100��������100�����޸�֮
**************************************************************************/
void MySocket::Recv(char* RecvBuf)
{
	if( (0 == ComType) && (0 == ComRole) )
		recv(SockCon, RecvBuf, 100, 0);
	if( (0 == ComType) && (1 == ComRole) )
		recv(SockSrv, RecvBuf, 100, 0);
	if(1 == ComType)
		recvfrom(SockSrv, RecvBuf, 100, 0, (sockaddr*)&AddrClient,&AddrLen);
}

/*************************************************************************
** �������ƣ�SendToTarget(char *IP_addr, int Port, char* SendBuf)
** ���ܼ��������ݷ��͵�ָ����ַ
** �䡡  �룺Ŀ��IP��ַ���˿ڣ��ʹ��Ҫ���͵����ݵĻ�������ָ��
** ��    ������
** ˵    �����˺������ǽ�LinkTarget()��Send()�������
** ע    �⣺ͨ��Э��ΪTCP���׽��������У�ʹ�ô˺������ն�Ϊ�ͻ���
**************************************************************************/
void MySocket::SendToTarget(char *IP_addr, int Port, char* SendBuf)
{
	LinkTarget(IP_addr, Port);
	Send(SendBuf);
}

/*************************************************************************
** �������ƣ�RecvFromTarget(char *IP_addr, int Port, char* RecvBuf)
** ���ܼ�������ָ����ַ��������
** �䡡  �룺Ŀ��IP��ַ���˿ڣ��ͽ������ݵĻ�����ָ��
** ��    ������
** ˵    �����˺������ǽ�LinkTarget()��Recv()�������
** ע    �⣺��
**************************************************************************/
void MySocket::RecvFromTarget(char *IP_addr, int Port, char* RecvBuf)
{
	LinkTarget(IP_addr, Port);
	Recv(RecvBuf);
}
