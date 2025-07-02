#ifndef __SERVO_H
#define __SERVO_H
#include "main.h"


double  angle_calculate_j2,angle_calculate_j3;

double transform_x, transform_y;

void pwm_start(void);
void translate_angle_to_pulse(double angle_1,double angle_2);// ½Ç¶È×ª»»
void servo_control(double temp_target_angle_1,double temp_target_angle_2);
void pwm_out(double angle_1, double angle_2);
void  Chessboard(uint8_t num);
void  Chess(uint8_t num);


void servo_angle_calculate(float target_x, float target_y);
void servo_test();


void servo_pid(int32_t now_x ,int32_t now_y,int32_t goal_x,int32_t goal_y);//¶æ»úPID
void angle_restrict(float angle1,float angle2);
void PWM_restrict(int32_t PWM1,int32_t PWM2);
void Servo_attract();



void Servo_putdown();
void transform(uint32_t x , uint32_t y);

void servo_reset();
#endif
