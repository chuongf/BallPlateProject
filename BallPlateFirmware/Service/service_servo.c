
#include  <service_servo.h>          // STM32F10x peripherals definitions
#include  "BallPlate_Plant.h"       // Ballplate parameters
#include  <UserDef.h>


//int16_t AngleDeg2PulseWidth(float AngleDeg, int8_t Dir, int16_t OffsetPWM)
//{
//  int16_t pw;
//	pw = (int16_t) (SLOPE_DEG*AngleDeg);
//  return Dir*pw + CENTER_PULSE_WIDTH + OffsetPWM;
//}
//int16_t AngleRad2PulseWidth(float AngleRad, int8_t Dir, int16_t OffsetPWM)
//{
//  int16_t pw;
//	pw = (int16_t) (SLOPE_RAD*AngleRad);
//  return Dir*pw + CENTER_PULSE_WIDTH + OffsetPWM;
//}




//void SetServoXAngleDeg(float AngleDeg)
//{
//	uint16_t duty;
//  duty = AngleDeg2Duty(AngleDeg);
//  if(duty<1000) duty = 1000;
//  if(duty>2000) duty = 2000;
//	setPwmXDuty_us(duty+PWM_SERVO_X_OFFSET);
////  setPwmXDuty_us(1000);
//	
//}

//void SetServoYAngleDeg(float AngleDeg)
//{
//	uint16_t duty;
//  duty = AngleDeg2Duty(AngleDeg);
//    if(duty<1000) duty = 1000;
//  if(duty>2000) duty = 2000;
//	setPwmYDuty_us(duty + PWM_SERVO_Y_OFFSET);
////  setPwmYDuty_us(1100);
//}


//void SetServoXAngleRad(float AngleRad)
//{
//	uint16_t duty;
//  duty = AngleRad2Duty(AngleRad);
//  if(duty<1000) duty = 1000;
//  if(duty>2000) duty = 2000;
//	setPwmXDuty_us(duty+PWM_SERVO_X_OFFSET);
////  setPwmXDuty_us(1000);
//	
//}

//void SetServoYAngleRad(float AngleRad)
//{
//	uint16_t duty;
//  duty = AngleRad2Duty(AngleRad);
//    if(duty<1000) duty = 1000;
//  if(duty>2000) duty = 2000;
//	setPwmYDuty_us(duty+PWM_SERVO_Y_OFFSET);
////  setPwmYDuty_us(1100);
//}

//int16_t AngleRad2Duty(float AngleRad)
//{
//  int16_t duty;
//	duty = (int16_t) (SLOPE_RAD*AngleRad);
//	return duty + CENTER_PULSE_WIDTH;
//}
