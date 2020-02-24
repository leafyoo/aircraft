#include "TeleProtocol.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(void)
{

	char  pwm_char[12];
	char distance_char[3];
	int pwm1, pwm2, pwm3, pwm4, distance;
	int pwm1_data, pwm2_data, pwm3_data, pwm4_data;

	MySocket Server(TCP,SERVER);
	Server.SocketInit((char*)"127.0.0.1",60000);
	//Server.SocketInit((char*)"192.168.0.106",60000);
	Server.LinkWait();

	cout << "Connect Succeed ^o^" << endl << endl ;

	do{	

		Server.Send((char*)"1");
		Server.Recv(pwm_char);

		pwm1_data = ( pwm_char[0] - 48)*100 + (pwm_char[1] - 48)*10 + (pwm_char[2] - 48);
		pwm2_data = ( pwm_char[3] - 48)*100 + (pwm_char[4] - 48)*10 + (pwm_char[5] - 48);
		pwm3_data = ( pwm_char[6] - 48)*100 + (pwm_char[7] - 48)*10 + (pwm_char[8] - 48);
		pwm4_data = ( pwm_char[9] - 48)*100 + (pwm_char[10] - 48)*10 + (pwm_char[11] - 48);

cout<<"PWM : 1 = "<<pwm1_data<<"   2: "<<pwm2_data<<"   3: "<<pwm3_data<<"   4: "<<pwm4_data<<"     Z: "<<distance<<endl;
//向PC发送超声测距
		//distance = pwm4;
		distance = 654;
		distance = distance + 5;  //使之大于4，以便与其他4路区分

		distance_char[0] = (char)((distance / 100 ) + '0');
		distance_char[1] = (char)((distance / 10) % 10 + '0');
		distance_char[2] = (char)(distance % 10 + '0');		
		Server.Send(distance_char);

		usleep(50000);	

	}while(1);

	return 0;

}




