#include "TeleProtocol.h"

#include <iostream>
using namespace std;

int main(void)
{
	MySocket Server(TCP,SERVER);
	Server.SocketInit((char*)"192.168.0.106",60000);
	Server.LinkWait();
	char recv[100];
	cout << "Connect Succeed!" << endl << endl ;

	do{	
		Server.Send("001");
		Server.Recv(recv);
		cout << recv << endl;

		Server.Send("002");
		Server.Recv(recv);
		cout << recv << endl;

		Server.Send("003");
		Server.Recv(recv);
		cout << recv << endl;

		Server.Send("004");
		Server.Recv(recv);
		cout << recv << endl;

	}while(1);


	return 0;
}
