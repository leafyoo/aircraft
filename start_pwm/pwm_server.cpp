#include "UART_PWM.h"
#include "TeleProtocol.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main(void)
{
	unsigned char  pwm_char[4];
	int pwm;
	int pwm1_data, pwm2_data, pwm3_data, pwm4_data;

	MySocket Server(UDP,SERVER);
	//Server.SocketInit((char*)"10.0.0.8",60000);
	Server.SocketInit((char*)"192.168.0.106",60000);

	UART_PWM MyPWM;
	MyPWM.UARTInit((char*)"/dev/ttyUSB0", 38400);

	do{	

		Server.Recv(pwm_char);
		pwm = MyPWM.SetPWM(pwm_char);

		pwm1_data = ( pwm_char[0]) * 4;
		pwm2_data = ( pwm_char[1]) * 4;
		pwm3_data = ( pwm_char[2]) * 4;
		pwm4_data = ( pwm_char[3]) * 4;
	
cout<<"PWM = "<<setw(4)<<pwm1_data<<setw(4)<<pwm2_data<<setw(4)<<pwm3_data<<setw(4)<<pwm4_data<<endl;

	}while(1);

	return 0;

}




