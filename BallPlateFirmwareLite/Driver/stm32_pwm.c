
#include  "stm32_pwm.h"          

  
void setPwmPeriod_us(uint16_t period_us)
{
//  TIM4->ARR = period_us;
  TIM_XYPERIOD = period_us;
}


void setPwmXDuty_us(uint16_t duty_us)
{
//  TIM4->CCR3 = duty_us;
  TIM_XDUTY = duty_us;
}


void setPwmYDuty_us(uint16_t duty_us)
{
//  TIM4->CCR4 = duty_us;
  TIM_YDUTY = duty_us;
}

void Start_PWMXY(void )
{
	TIM4->CNT = 0;	//clear CNT. All PWM pins get active state
}

void Set_DutyX(uint32_t duty_us )
{
	TIM_XDUTY = duty_us;
}

void Set_DutyY(uint32_t duty_us  )
{
	TIM_YDUTY = duty_us;	
}
