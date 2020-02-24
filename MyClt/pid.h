#ifndef _PID_H_
#define _PID_H_

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <windows.h>

//for MFC printf
#include <io.h>
#include <fcntl.h>

#define DATA_NUM 100

void read_position(int pid_num);
void write_pid();
int  pid_cal(int pid_type, float center,float set);
void read_parameter();
void write_check_parameter();
char * show_system_time();
long ms_time();
void InitConsoleWindow(); //for MFC printf

extern int distance_z;
// 坐标说明：
// x，从左向右：0到1024
// y，从下向上：0到1024
// 角度：红色指向蓝色，水平向右（3点钟方向）为0度，从3点到6点：0到-90度，从3点到12点：0到90度。

//int pwm_x;			横滚pwm值，大于500向左飞
//int pwm_y;			俯仰pwm值，大于500向前飞
//int pwm_z;			升降pwm值，油门
//int pwm_angle;		旋转pwm值，俯视飞机，大于500逆时针飞

//参数说明：以下参数初值均从记事本中读取。
int pwm_x;				//横滚pwm值
int pwm_y;				//俯仰pwm值
int pwm_z;				//升降pwm值
int pwm_angle;			//旋转pwm值

float set_x;				//设置的位置
float set_y;				//设置的位置
float set_z;				//设置的位置
float set_angle;			//设置的角度

//pwm 输出偏置
int offset_x;
int offset_y;
int offset_z;
int offset_angle;

int path_type = 0;

float Kp;					//比例系数
float Ki;					//积分系数
float Kd;	                //微分系数

float Kp1;				
float Ki1;				
float Kd1;           

//y
float Kp2;				
float Ki2;				
float Kd2;	            

//z
float Kp3;			
float Ki3;			
float Kd3;          

//angle
float Kp4;
float Ki4;
float Kd4;

float center_x = 0, center_y = 0, center_z = 0, angle = 0;
float length_x = 0, length_y = 0, length_RB = 0, angle_1 = 0;

float Ek1[5] = {0};	                //当前的偏差值
float Ek1_1[5] = {0};	            	//上一次的偏差值
float Ek1_2[5] = {0};	            	//上上次的偏差值
float Uk1[5] = {0};	                //目标增量
float Ukk1[5] = {0};	                //上一次目标增量

int pid_cal(int pid_type, float center,float set, int pwm_min, int pwm_max)
{
	int pwm, i;

	switch(pid_type)
	{	
		case 1: Kp = Kp1;
				Ki = Ki1;
				Kd = Kd1;
				break;

		case 2: Kp = Kp2;
				Ki = Ki2;
				Kd = Kd2;
				break;

		case 3: Kp = Kp3;
				Ki = Ki3;
				Kd = Kd3;
				break;

		case 4: Kp = Kp4;
				Ki = Ki4;
				Kd = Kd4;
				break;
		default:
				printf("pid_type read error!\n");
				exit(0);
	}
	i = pid_type;
	Ek1[i] = center - set;								            //计算偏差值
/*	if (Ek1[i] > 100 || Ek1[i] < -100)
	{
		Kp=4*Kp;
		Kd=4*Kd;
		Ki=4*Ki;
	}*/
	Uk1[i] = Kp * (Ek1[i] - Ek1_1[i]) + Ki * Ek1[i] + Kd * (Ek1[i]-2 * Ek1_1[i] + Ek1_2[i]) + Ukk1[i];		//PID调节
	Ukk1[i] = Uk1[i];											//保存输出值
	Ek1_2[i] = Ek1_1[i];										//保存上次偏差
	Ek1_1[i] = Ek1[i];											//保存这次偏差

	if(Uk1[i] >= -1 && Uk1[i] <= 1)								//如果调节值在阈值范围内，不需要调节
	{
		Uk1[i] = Ukk1[i];
	}
	else
	{
		if(Uk1[i] > pwm_max)			//限制值大小
			Uk1[i] = pwm_max;
		else if(Uk1[i] < pwm_min)	
			Uk1[i] = pwm_min;
	}

	pwm = 500 + Uk1[i];		//5%—10%对应PWM范围：0—1000，中间位置为500
	
	return pwm;
}

void write_pid()
{
	FILE * fp_pid;

	if(NULL==(fp_pid=fopen("000000000_pid_data.txt","a+")))
	{
		printf("cannot open pid_data.txt!\n");
		exit(0);
	}
	fprintf(fp_pid, "%4d    %4d    %4d    %4d  --- %ld ms %s\n",pwm_x, pwm_y, pwm_z, pwm_angle, ms_time(), show_system_time());
	fclose(fp_pid);
}

void read_parameter()
{
	FILE * fp_parameter;
	if (NULL==(fp_parameter=fopen("0000parameter.txt","r+")))
	{
		printf("cannot open 0000parameter.txt!\n");
		exit(0);
	}
	if (21 != (fscanf(fp_parameter," %f %f %f \
		 %f %f %f \
		 %f %f %f \
		 %f %f %f \
		 %d %d %d %d \
		 %d %d %d %d \
		 %d",\
		&Kp1, &Ki1, &Kd1,\
		&Kp2, &Ki2, &Kd2,\
		&Kp3, &Ki3, &Kd3,\
		&Kp4, &Ki4, &Kd4,\
		&offset_x, &offset_y, &offset_z, &offset_angle,\
		&pwm_x, &pwm_y, &pwm_z, &pwm_angle, \
		&path_type)))
	{
		printf("cannot read parameter.txt!\n");
		exit(0);
	}
	fclose(fp_parameter);	
}

/*
void write_check_parameter()
{
	FILE * fp_write_check_parameter;
	
	if(NULL == (fp_write_check_parameter = fopen ("00000000_check_parameter.txt","a+")))
	{
		printf ("cannot open pid_data.txt!\n");
		exit (0);
	}
	fprintf (fp_write_check_parameter, "  \n%s \
%f	%f	%f\n \
%f	%f	%f\n \
%f	%f	%f\n \
%f	%f	%f\n \
%f	%f	%f	%f\n \
%d	%d	%d	%d\n \
%d	%d	%d	%d\n ",\

show_system_time(),\
Kp1, Ki1, Kd1,\
Kp2, Ki2, Kd2,\
Kp3, Ki3, Kd3,\
Kp4, Ki4, Kd4,\
set_x, set_y, set_z, set_angle,\
offset_x, offset_y, offset_z, offset_angle,\
pwm_x, pwm_y, pwm_z, pwm_angle);

	fclose (fp_write_check_parameter);
}
*/

void write_check_parameter()
{
	FILE * fp_write_check_parameter;
	
	if(NULL == (fp_write_check_parameter = fopen ("00000000_check_parameter.txt","a+")))
	{
		printf ("cannot open pid_data.txt!\n");
		exit (0);
	}
	fprintf (fp_write_check_parameter, "%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f	%f\n",\
										Kp1, Ki1, Kd1,Kp2, Ki2, Kd2,Kp3, Ki3, Kd3,Kp4, Ki4, Kd4);
	
	fclose (fp_write_check_parameter);
}

void write_position()
{
	FILE * fp_position;
	if(NULL==(fp_position=fopen("000000000_position_data.txt","a+")))
	{
		printf("cannot open position_data.txt!\n");
		exit(0);
	}

	fprintf( fp_position, "%8.1f  %4d   	%8.1f  %4d  	%8.1f  %4d  	%8.1f  %4d --- %8ld ms %s\n",\
		center_x,pwm_x,  center_y,pwm_y,  center_z,pwm_z, angle,pwm_angle,ms_time(), show_system_time());

	fclose(fp_position);
}

char * show_system_time()
{	
	time_t t;
	time(&t);
	return ctime(&t);
}

long ms_time()
{		
	static DWORD ms_start, ms_stop;
	static int ms_flag = 0;

	if (!ms_flag) 
		ms_start = GetTickCount();
	ms_stop = GetTickCount();
	ms_flag++;

	return (ms_stop - ms_start);
}

void InitConsoleWindow()
{
    int nCrt = 0;
    FILE* fp;
    AllocConsole();
    nCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
    fp = _fdopen(nCrt, "w");
    *stdout = *fp;
    setvbuf(stdout, NULL, _IONBF, 0);
}

/*
struct timeval t_start, t_end;
gettimeofdat(&t_start, NULL);
Sleep(1000);
gettimeofday(&t_end, NULL);
long t_use = 1000000 * (t_end.tv_sec - t_start.tv_sec) + (t_end.tv_usec - t_start.tv_usec);
*/
#endif