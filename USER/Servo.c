
#include "tim.h"
#include "arm_math.h"
#include "usart.h"
#include "main.h"


#define PIX2ANG   0.1f   // 像素→角度比例
#define PIX2ANG_Y 0.1f
#define MIN_ANG   45.0f
#define MAX_ANG  180.0f
#define MID_ANG   90.0f
#define MAX_U    5.0f
#define DT_MIN    0.005f
#define DT_MAX    0.1f
#define I_MAX     (MAX_U/KI)
#define CLAMP(x, lo, hi)  ((x)<(lo)?(lo):((x)>(hi)?(hi):(x)))

#define KP    0.1f       // 比例
#define KI    0.01f      // 积分
#define KD    0.05f      // 微分

#define MAX_INT  50.0f   // 最大积分限幅
 
extern UART_HandleTypeDef huart1;

static float integ_h, prev_e_h;
static uint32_t last_t;


int32_t PWM1_OUT,PWM2_OUT,NOW_PWM1,NOW_PWM2,Last_PWM1,Last_PWM2;


int32_t error2 ,error1,last_error1,last_error2;
int32_t  error_angle2_kp,error_angle1_kp,error_angle1x_kp;
double angle1_out,angle2_out,error2_kp;

double abs_angle_error_1;
double abs_angle_error_2;
double a1,a2;
double now_angle_1 = 90,now_angle_2 = 90;
double pulse_1,pulse_2;
extern double  angle_calculate_j2,angle_calculate_j3;

extern double transform_x, transform_y;


void pwm_start(void)  // pwm波初始化
{
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);//PWM_1
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);//PWM_2
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);//PWM_3
}


void translate_angle_to_pulse(double angle_1,double angle_2)// 角度转换
{
	pulse_1 = ((angle_1)/ 180 *2000+500);
	pulse_2 =	((angle_2)/ 180 *2000+500);
	NOW_PWM1 = pulse_1;
	NOW_PWM2 = pulse_2;
}

void pwm_out(double angle_1, double angle_2)
{
	translate_angle_to_pulse(angle_1,angle_2);
	if(pulse_1 != NULL)
	{
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, pulse_1);
	}
	if(pulse_2 != NULL)
	{
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, pulse_2);
	}
	
}

void servo_control(double temp_target_angle_1,double temp_target_angle_2)
{
	abs_angle_error_1 = now_angle_1 -temp_target_angle_1;
	abs_angle_error_2 = now_angle_2 -temp_target_angle_2;
	

	
	if(abs_angle_error_1 !=0)
	{
		if(abs_angle_error_1 >0)
		{
			while(abs_angle_error_1>=0)
			{
				abs_angle_error_1--;
				now_angle_1 --;
				pwm_out(now_angle_1,now_angle_2);
			}
		}
		if(abs_angle_error_1 <0)
		{
		  abs_angle_error_1	=fabs(abs_angle_error_1);
			while(abs_angle_error_1>=0)
			{
				abs_angle_error_1--;
				now_angle_1 ++;
				pwm_out(now_angle_1,now_angle_2);
			}
		}
	
	}
	if(abs_angle_error_2 !=0)
	{
		if(abs_angle_error_2 >0)
		{
			while(abs_angle_error_2 >=0)
			{
				abs_angle_error_2--;
				now_angle_2 --;
				HAL_Delay(1);
				pwm_out(now_angle_1,now_angle_2);
			}
		}
		if(abs_angle_error_2 <0)
		{
		  abs_angle_error_2	=fabs(abs_angle_error_2);
			while(abs_angle_error_2>=0)
			{
				abs_angle_error_2--;
				now_angle_2 ++;
				pwm_out(now_angle_1,now_angle_2);
			}
		}
	}	
}


void servo_angle_calculate(float target_x, float target_y)
{
		
	double x,y;
	double A2,A3;
	double j2,j3,j1;
	double len;
	A2=11.5;
	A3=15;	

	x=target_x; 
	y=target_y;
	len = sqrt((y*y)+(x*x));
	//printf("%f",len);
	 
	 j2 = atan2(y, x)-acos(((len*len)+(A2*A2)-(A3*A3))/(2*A2*len));
	 j3 = acos(((A3*A3)+(A2*A2)-(len*len))/(2*A3*A2));
	 j2 =j2*57.3 ;
	 j3 = j3*57.3;
		
	 angle_calculate_j2 = j2;
	 angle_calculate_j3 =j3;
	 HAL_Delay(1000);
}

void servo_test()
{
	 servo_angle_calculate(2,18);
	 servo_control(angle_calculate_j2+5,angle_calculate_j3-10);
	 HAL_Delay(2000);
	 servo_angle_calculate(4,18);
	 servo_control(angle_calculate_j2+5,angle_calculate_j3-10);
	 HAL_Delay(2000);
}

void Servo_attract()
{
	 __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,1850);  //舵机发出
	 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
	 HAL_Delay(2000);
	 __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,1100);
	 HAL_Delay(1000);
}
void Servo_putdown()
{
	 __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,1650);  //舵机发出
	 HAL_Delay(2000);
	 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);
	 __HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,1300);
	 HAL_Delay(1000);
}


void servo_reset()
{
//	 servo_angle_calculate(-14,14);
//	 servo_control(angle_calculate_j2,angle_calculate_j3-10);
	pwm_out(90,90);
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,1300);
	 HAL_Delay(2000);
}

void transform(uint32_t x , uint32_t y)
{
	if(x<700){x=x - 335;}
	x =x-335 +10;
	y =817-y;
	transform_x = -14+(x* 0.28)/10;
	transform_y = 14+(y* 0.3)/10;

}


void angle_restrict(float angle1,float angle2)
{
	
	angle1_out = angle1;
	angle2_out = angle2;
	if(angle1>=175) { angle1_out = 175; }
	if(angle1<=5)		{ angle1_out = 5;   }
  if(angle2>=175) { angle2_out = 175; }
	if(angle2<=5)		{ angle2_out = 5;   }

}

void PWM_restrict(int32_t PWM1,int32_t PWM2)
{
	if(PWM1>=2400) { PWM1_OUT = 2400; }
	if(PWM1<=550)		{ PWM1_OUT = 550;   }
  if(PWM2>=2400) { PWM2_OUT = 2400; }
	if(PWM2<=550)		{ PWM2_OUT = 550;   }

}

/*void servo_pid_h(uint32_t now_x, uint32_t goal_x)
{
    // 目标丢失 → 清状态 + 复位
    if (now_x == 0) {
        integ_h = prev_e_h = 0;
        last_t = HAL_GetTick();
        servo_control(MID_ANG, MID_ANG);
        return;
    }

    // 1) 计算 dt
    uint32_t t = HAL_GetTick();
    float dt = CLAMP((t - last_t) * 0.001f, DT_MIN, DT_MAX);
    last_t = t;

    // 2) 误差 (°)
    float e = ((float)goal_x - (float)now_x) * PIX2ANG;

    // 3) 积分 & 限幅
    integ_h = CLAMP(integ_h + e * dt, -I_MAX, I_MAX);

    // 4) 微分
    float d = (e - prev_e_h) / dt;
    prev_e_h = e;

    // 5) PID 输出 & 限幅
    float u = CLAMP(KP*e + KI*integ_h + KD*d, -MAX_U, MAX_U);

    // 6) 目标角度 & 限幅
    float tgt = CLAMP(now_angle_1 + u, MIN_ANG, MAX_ANG);

    // 7) 下发舵机
    servo_control(tgt, MID_ANG);

    // 8) 串口调试输出（同原格式）
    int16_t u16 = (int16_t)(u * 100.0f);
    const int16_t U16_MAX = (int16_t)(MAX_U * 100.0f);
    if      (u16 >  U16_MAX) u16 =  U16_MAX;
    else if (u16 < -U16_MAX) u16 = -U16_MAX;
    uint8_t tx[10] = {
      0x03, ~0x03,
      (uint8_t)(goal_x & 0xFF), (uint8_t)(goal_x >> 8),
      (uint8_t)(now_x  & 0xFF), (uint8_t)(now_x  >> 8),
      (uint8_t)(u16     & 0xFF), (uint8_t)(u16     >> 8),
      ~0x03, 0x03
    };
    HAL_UART_Transmit(&huart1, tx, sizeof(tx), HAL_MAX_DELAY);
}*/

// 竖直通道的状态变量，和水平通道分开
static float  integ_v, prev_e_v;
static uint32_t last_t_v;

/**
 * 竖直 PID 跟踪：now_y、goal_y 单位是像素
 * 保持水平通道中位，竖直通道跟踪目标
 */
 
 /*
void servo_pid_w(uint32_t now_y, uint32_t goal_y)
{
    // 丢失目标 → 清状态 + 复位
    if (now_y == 0) {
        integ_v = prev_e_v = 0;
        last_t_v = HAL_GetTick();
        servo_control(MID_ANG, MID_ANG);
        return;
    }

    // 1) 计算 dt
    uint32_t t = HAL_GetTick();
    float dt = CLAMP((t - last_t_v) * 0.001f, DT_MIN, DT_MAX);
    last_t_v = t;

    // 2) 误差 (°)
    float e = ((float)goal_y - (float)now_y) * PIX2ANG;

    // 3) 积分 & 限幅
    integ_v = CLAMP(integ_v + e * dt, -I_MAX, I_MAX);

    // 4) 微分
    float d = (e - prev_e_v) / dt;
    prev_e_v = e;

    // 5) PID 输出 & 限幅
    float u = CLAMP(KP*e + KI*integ_v + KD*d, -MAX_U, MAX_U);

    // 6) 目标角度 & 限幅
    float tgt = CLAMP(now_angle_2 + u, MIN_ANG, MAX_ANG);

    // 7) 下发舵机（水平保持中位）
    servo_control(MID_ANG, tgt);

    // 8) 串口调试输出：GY, NY, U
    int16_t u16 = (int16_t)(u * 100.0f);
    const int16_t U16_MAX = (int16_t)(MAX_U * 100.0f);
    u16 = CLAMP(u16, -U16_MAX, U16_MAX);

    uint8_t tx[10] = {
      0x03, ~0x03,
      (uint8_t)(goal_y & 0xFF),  (uint8_t)(goal_y >> 8),
      (uint8_t)(now_y  & 0xFF),  (uint8_t)(now_y  >> 8),
      (uint8_t)(u16     & 0xFF),  (uint8_t)(u16     >> 8),
      ~0x03, 0x03
    };
    HAL_UART_Transmit(&huart1, tx, sizeof(tx), HAL_MAX_DELAY);
}*/

void servo_pid(uint32_t now_x, uint32_t goal_x,
               uint32_t now_y, uint32_t goal_y)
{
    // 如果两轴都丢失 → 清状态 + 中位复位
    if (now_x == 0 && now_y == 0) {
        integ_h = prev_e_h = 0;
        integ_v = prev_e_v = 0;
        last_t  = HAL_GetTick();
        servo_control(MID_ANG, MID_ANG);
        return;
    }

    // 计算公共 dt
    uint32_t t = HAL_GetTick();
    float dt = CLAMP((t - last_t) * 0.001f, DT_MIN, DT_MAX);
    last_t = t;

    // ——— 水平通道 ———
    float e_h = ((float)goal_x - now_x) * PIX2ANG;
    integ_h  = CLAMP(integ_h + e_h * dt, -I_MAX, I_MAX);
    float d_h = (e_h - prev_e_h) / dt;
    prev_e_h = e_h;
    float u_h = CLAMP(KP*e_h + KI*integ_h + KD*d_h, -MAX_U, MAX_U);
    float tgt_h = CLAMP(now_angle_1 + u_h, MIN_ANG, MAX_ANG);

    // ——— 竖直通道 ———
    float e_v = ((float)goal_y - now_y) * PIX2ANG;
    integ_v  = CLAMP(integ_v + e_v * dt, -I_MAX, I_MAX);
    float d_v = (e_v - prev_e_v) / dt;
    prev_e_v = e_v;
    float u_v = CLAMP(KP*e_v + KI*integ_v + KD*d_v, -MAX_U, MAX_U);
    float tgt_v = CLAMP(now_angle_2 + u_v, MIN_ANG, MAX_ANG);

    // 同步下发两路舵机
    servo_control(tgt_h, tgt_v);

    // 可选：一次发送两个调试帧
    int16_t uh16 = CLAMP((int16_t)(u_h*100), -MAX_U*100, MAX_U*100);
    int16_t uv16 = CLAMP((int16_t)(u_v*100), -MAX_U*100, MAX_U*100);

    uint8_t tx1[10] = {
      0x03,~0x03,
      (uint8_t)(goal_x &0xFF),(uint8_t)(goal_x>>8),
      (uint8_t)(now_x  &0xFF),(uint8_t)(now_x >>8),
      (uint8_t)(uh16    &0xFF),(uint8_t)(uh16   >>8),
      ~0x03,0x03
    };
    HAL_UART_Transmit(&huart1, tx1,10,HAL_MAX_DELAY);

    uint8_t tx2[10] = {
      0x04,~0x04,
      (uint8_t)(goal_y &0xFF),(uint8_t)(goal_y>>8),
      (uint8_t)(now_y  &0xFF),(uint8_t)(now_y >>8),
      (uint8_t)(uv16    &0xFF),(uint8_t)(uv16   >>8),
      ~0x04,0x04
    };
    HAL_UART_Transmit(&huart1, tx2,10,HAL_MAX_DELAY);
}

