#include "Krnb_hand.h"

void funtion(void){
    printf("Holle,world!");
}



struct pid_algorithum
{
	float Pid_SetSpeed;
	float Pid_ActualSpeed;
	float Pid_Error;
	float Pid_Error_Last;
	float Pid_Voltage;
	float Pid_integral;
	float Pid_kp;
	float Pid_ki;
	float Pid_kd;
}pid_algorithum;

float PID_algorithum(float target_Value,int number) {
	pid_algorithum.Pid_SetSpeed = target_Value;
	pid_algorithum.Pid_Error = pid_algorithum.Pid_SetSpeed - pid_algorithum.Pid_ActualSpeed;
	pid_algorithum.Pid_integral += pid_algorithum.Pid_Error;
	pid_algorithum.Pid_Voltage = pid_algorithum.Pid_kp * pid_algorithum.Pid_Error + pid_algorithum.Pid_ki * pid_algorithum.Pid_integral + pid_algorithum.Pid_kd * (pid_algorithum.Pid_Error - pid_algorithum.Pid_Error_Last);
	//std::cout<< "次数" << number << "	" << "pid_algorithum.Pid_ActualSpeed" << "	" << pid_algorithum.Pid_ActualSpeed << "	" << "ERROR差" << "	" << pid_algorithum.Pid_Error << "	" << "上一次ERROR差" << "	" << pid_algorithum.Pid_Error_Last << "	" << "微分系数" << "	" << pid_algorithum.Pid_kd * (pid_algorithum.Pid_Error - pid_algorithum.Pid_Error_Last) << endl;
	printf("次数%d pid_algorithum.Pid_ActualSpeed %lf  ERROR差 %lf  上一次ERROR差 %f 微分系数 %f\n",number,pid_algorithum.Pid_ActualSpeed,pid_algorithum.Pid_Error,pid_algorithum.Pid_Error_Last,pid_algorithum.Pid_kd * (pid_algorithum.Pid_Error - pid_algorithum.Pid_Error_Last));
    pid_algorithum.Pid_Error_Last = pid_algorithum.Pid_Error;
	pid_algorithum.Pid_ActualSpeed=pid_algorithum.Pid_Voltage * 1.0;
	
	return pid_algorithum.Pid_ActualSpeed;
}

void pid_inti(void) {
	pid_algorithum.Pid_SetSpeed = 0.0;
	pid_algorithum.Pid_ActualSpeed = 0.0;
	pid_algorithum.Pid_Error = 0.0;
	pid_algorithum.Pid_Error_Last = 0.0;
	pid_algorithum.Pid_Voltage = 0.0;
	pid_algorithum.Pid_integral = 0.0;
	pid_algorithum.Pid_kp = 0.2;
	pid_algorithum.Pid_ki = 0.015;
	pid_algorithum.Pid_kd = 0.2;
}