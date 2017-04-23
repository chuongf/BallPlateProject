#include  "stm32f10x.h"          // STM32F10x peripherals definitions
#include  "platform_cfg.h"       // board specific (re-)definitions


void setPwmPeriod_us(uint16_t period_us);
void setPwmXDuty_us(uint16_t duty_us);
void setPwmYDuty_us(uint16_t duty_us);

void Start_PWMXY(void );
void Set_DutyX(uint32_t duty_us);
void Set_DutyY(uint32_t duty_us);

