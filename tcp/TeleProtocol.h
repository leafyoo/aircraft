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
** 特别说明: 此类为Linux版本
** 修改之处: 1、不需要头文件winsock2.h
**	   2、不需链接WS2_32.lib库
**	   3、SOCKADDR_IN和SOCKADDR必须小写
**	   4、不需链接套接字库
**	   5、AddrSrv.sin_addr.S_un.s_addr改为AddrSrv.sin_addr.s_addr
**	   6、AddrLen声明类型由int改为addrlen_t
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
** 函数名称：MySocket(bool Type)
** 功能简述：构造函数重载1，完成部分私有变量和通信协议类型及角色的初始化
** 输　  入：通信协议类型UDP
** 输    出：无
** 说    明：TCP为0，UDP为1
** 注    意：1、IP地址设置为空*IPSrv = 0
**	    2、若选择TCP，则通信角色默认为Server
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
** 函数名称：MySocket(bool Type, bool Role)
** 功能简述：构造函数重载2，完成部分私有变量和通信协议类型及角色的初始化
** 输　  入：通信协议类型TCP或UDP，通信角色类型Server或Client
** 输    出：无
** 说    明：1、TCP为0，UDP为1
**	    2、SERVER为0，CLIENT为1
** 注    意：无
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
** 函数名称：~MySocket()
** 功能简述：析构函数，断开套接字并完成私有变量的释放
** 输　  入：无
** 输    出：无
** 说    明：无
** 注    意：无
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
** 函数名称：SocketInit(char* IP_addr, int Port)
** 功能描述：套接字初始化
** 输　  入：主机IP地址及端口号
** 输    出：无
** 说    明：F1、私有变量IPSrv和PortSrv进行赋值
**	    F2、完成主机地址信息注册
**	    F3、完成套接字的建立
**	    F4、完成套接字的绑定
**	    F5、设置套接字监听队列长度为5
** 注    意：1、输入是主机IP及端口
**	    2、F2中IP赋值问题，开始是*IPSrv = *IP_addr，recvfrom将不能阻塞
**	    3、一定要加载套接字库，否则recvfrom不能阻塞
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
** 函数名称：LinkTarget(char* IP_addr, int Port)
** 功能描述：与目标建立连接
** 输　  入：目标IP地址及端口号
**	    服务器端IP地址及端口号（TCP）
** 输    出：无
** 说    明：1、私有变量IPClient和PortClient进行赋值
**	    2、完成目标地址信息注册
** 注    意：将服务器地址存放在AddrClient中（TCP）
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
** 函数名称：LinkWait(void)
** 功能简述：建立链接套接字SockCon并阻塞等待连接
** 输　  入：无
** 输    出：无
** 说    明：TCP使用
** 注    意：未等到连接将在此阻塞
**************************************************************************/
void MySocket::LinkWait(void)
{
	if(0 == ComType)
		SockCon = accept(SockSrv,(sockaddr*)&AddrClient,&AddrLen);
}

/*************************************************************************
** 函数名称：Send(char* SendBuf)
** 功能简述：数据发送
** 输　  入：存放要发送的数据的缓冲区的指针
** 输    出：无
** 说    明：无
** 注    意：SERVER为0，CLIENT为1
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
** 函数名称：Recv(char* RecvBuf)
** 功能简述：数据接收
** 输　  入：接收数据的缓冲区指针
** 输    出：无
** 说    明：调用此函数会将接收方地址信息填入AddrClient数据结构中
** 注    意：接收函数参数中100为最大接收字符数，默认100，若大于100，需修改之
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
** 函数名称：SendToTarget(char *IP_addr, int Port, char* SendBuf)
** 功能简述：数据发送到指定地址
** 输　  入：目标IP地址及端口，和存放要发送的数据的缓冲区的指针
** 输    出：无
** 说    明：此函数就是将LinkTarget()和Send()组合起来
** 注    意：通信协议为TCP的套接字连接中，使用此函数的终端为客户端
**************************************************************************/
void MySocket::SendToTarget(char *IP_addr, int Port, char* SendBuf)
{
	LinkTarget(IP_addr, Port);
	Send(SendBuf);
}

/*************************************************************************
** 函数名称：RecvFromTarget(char *IP_addr, int Port, char* RecvBuf)
** 功能简述：从指定地址接收数据
** 输　  入：目标IP地址及端口，和接收数据的缓冲区指针
** 输    出：无
** 说    明：此函数就是将LinkTarget()和Recv()组合起来
** 注    意：无
**************************************************************************/
void MySocket::RecvFromTarget(char *IP_addr, int Port, char* RecvBuf)
{
	LinkTarget(IP_addr, Port);
	Recv(RecvBuf);
}
