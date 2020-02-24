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
** 函数名称：UART_PWM()
** 功能简述：构造函数，完成私有变量初始化
** 输　  入：无
** 输    出：无
** 说    明：1、PWM ID前检验码为0xAA+0xAA+0xAA+0xAA
**			 2、PWM Value前检验码0x55+0x55+0x55+0x55
**			 3、设定PWM共11位（第5位为ID，第10为Value高8位，第11位为Value低8位）
** 注    意：无
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
** 函数名称：SetPWM(uchar Id, int Val)
** 功能简述：指定PWM序号（1~6）并设定档位（0~1000）
** 输　  入：ID为PWM序号，Val为档位
** 输    出：正确返回的PWM序号（1~6），错误返回0
** 说    明：设定成功，返回PWM序号
** 注    意：序号为数字的ASCII码（原序号+0x30）
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



