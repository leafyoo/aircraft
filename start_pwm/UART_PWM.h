#include "UART_Devices.h"

class UART_PWM : public UART_Devices
{
public:
	UART_PWM();
	int SetPWM(unsigned char  * val);
private:

	uchar Send[5];
	uchar Recv[1];
};
/*************************************************************************
** �������ƣ�UART_PWM()
** ���ܼ��������캯�������˽�б�����ʼ��
** �䡡  �룺��
** ��    ������
** ˵    ����1��PWM IDǰ������Ϊ0xAA+0xAA+0xAA+0xAA
**			 2��PWM Valueǰ������0x55+0x55+0x55+0x55
**			 3���趨PWM��11λ����5λΪID����10ΪValue��8λ����11λΪValue��8λ��
** ע    �⣺��
**************************************************************************/
UART_PWM::UART_PWM()
{

	/* Check Codes Before PWM ID */

	/* Send 11-bit */
	Send[0] = 0xFF;
	Send[1] = 0;
	Send[2] = 0;
	Send[3] = 0;
	Send[4] = 0;
}

/*************************************************************************
** �������ƣ�SetPWM(uchar Id, int Val)
** ���ܼ�����ָ��PWM��ţ�1~6�����趨��λ��0~1000��
** �䡡  �룺IDΪPWM��ţ�ValΪ��λ
** ��    ������ȷ���ص�PWM��ţ�1~6�������󷵻�0
** ˵    �����趨�ɹ�������PWM���
** ע    �⣺���Ϊ���ֵ�ASCII�루ԭ���+0x30��
**************************************************************************/
int UART_PWM::SetPWM(unsigned char * val)
{	

	int Size;
	Send[1] = val[0];
	Send[2] = val[1];
	Send[3] = val[2];
	Send[4] = val[3];

	Size = UART_Devices::GetData(Send, Recv, 5, 0);
	return 0;
	
}



