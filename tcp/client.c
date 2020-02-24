#include "TeleProtocol.h"

#include <iostream>
#include <stdlib.h>
using namespace std;


void Init_Socket(void);
	


char  pwm_char[4];

int main(void)
{
	MySocket Client(UDP, CLIENT);
	Client.SocketInit((char*)"10.0.0.9", 60000);
	Client.LinkTarget((char*)"10.0.0.8", 60000);
	//Client.SocketInit((char*)"192.168.0.107", 60000);
	//Client.LinkTarget((char*)"192.168.0.106", 60000);
	cout << endl << endl;

	int pwm_x = 500;				//���pwmֵ������500�����
	int pwm_y = 600;				//����pwmֵ������500��ǰ��
	int pwm_z = 700;				//����pwmֵ������
	int pwm_angle = 800;			//��תpwmֵ�����ӷɻ�������500��ʱ���

	do{
		pwm_x = pwm_x / 4;
		pwm_y = pwm_y / 4;	
		pwm_z = pwm_z / 4;
		pwm_angle = pwm_angle / 4;	
		
		pwm_char[0] = (char)(pwm_x);
		pwm_char[1] = (char)(pwm_y);
		pwm_char[2] = (char)(pwm_z);
		pwm_char[3] = (char)(pwm_angle);
		
		Client.Send(pwm_char);
		
		cout<<"ok"<<endl;
		sleep(1);
	
	}while(1);
	
	return 0;
}



