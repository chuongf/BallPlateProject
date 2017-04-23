/**
*  ***********************************************************************
*  @file    Controller.c
**************************************************************************
*/

#include "Controller.h"           // State Space control variables
#include "BallPlate_Plant.h"      // Plant parameters in matrices
#include "KalmanLib2.h"           // States variables


void bbSSControl( SS_SysPar *XY)
/**
  Full state feedback controller
	Control_ouput = posRef * V - K' * xEst
*/

{
  int16_t pw;
  float out_temp; 
  #ifdef INPUT_JERK_LIMIT
  float jerk;
  #endif
  
#ifdef _SYS_PXL_DEG   //pos in pxl, input in degree
  
  #ifdef Overwrite_Kalman
    out_temp = (float) XY->Ctrl_RefPosPxl * XY->Ctrl_V
           - XY->Ctrl_K[0] * XY->Meas_PosPxl
           - XY->Ctrl_K[1] * XY->Cal_SpeedPxlPerSec;
   
  #else
    out_temp = (float) XY->Ctrl_RefPosPxl * XY->Ctrl_V
               - XY->Ctrl_K[0] * XY->KF_X[0]
               - XY->Ctrl_K[1] * XY->KF_X[1];
                   
    #if ORDER == 3
      out_temp = out_temp - XY->Ctrl_K[2] * XY->KF_K[2];
    #endif
  #endif

  #ifdef INPUT_JERK_LIMIT
  jerk = (out_temp - XY->Ctrl_OutAngleDeg) / XY->Ctrl_Ts;
  if(jerk > MAX_JERK) {
    jerk = MAX_JERK;
    out_temp = XY->Ctrl_OutAngleDeg + MAX_JERK*XY->Ctrl_Ts;
  }
  #else
  XY->Ctrl_OutAngleDeg = out_temp;
  #endif
    
  if ( XY->Ctrl_OutAngleDeg > MAX_SERVO_ANGLE_DEG )
  { 
    XY->Ctrl_OutAngleDeg =    MAX_SERVO_ANGLE_DEG;
  }

  if( XY->Ctrl_OutAngleDeg < -MAX_SERVO_ANGLE_DEG )
  {
    XY->Ctrl_OutAngleDeg =   -MAX_SERVO_ANGLE_DEG;
  }

  //convert into PWM to control motor
  pw = (int16_t) (SLOPE_DEG*XY->Ctrl_OutAngleDeg);
  XY->Ctrl_OutPWus = XY->Plant_ServoDir*pw;

#else
  #error "Only support Degree Input, Pixel output"
#endif
/*


#ifdef _SYS_PXL_RAD     //pos in pixel, input rad
  
  XY->Ctrl_OutAngleRad = XY->Ctrl_RefPosPxl * XY->Ctrl_V
                     - XY->Ctrl_K[0] * XY->KF_X[0]
                     - XY->Ctrl_K[1] * XY->KF_X[1];
										 
  #if ORDER == 3
    XY->Ctrl_OutAngleRad = XY->Ctrl_OutAngleRad - XY->Ctrl_K[2] * XY->KF_K[2];
  #endif

  XY->Ctrl_OutAngleRad = XY->Plant_ServoDir*XY->Ctrl_OutAngleRad;


  // Limit servo angle  |cmdServoAngleRad| <= MAX_SERVO_ANGLE_RAD
  if ( XY->Ctrl_OutAngleRad > MAX_SERVO_ANGLE_RAD )
  { 
    XY->Ctrl_OutAngleRad =    MAX_SERVO_ANGLE_RAD;
  }

  if( XY->Ctrl_OutAngleRad < -MAX_SERVO_ANGLE_RAD )
  {
    XY->Ctrl_OutAngleRad =   -MAX_SERVO_ANGLE_RAD;
  }
  
  //convert into PWM to control motor
  pw = (int16_t) (SLOPE_RAD*XY->Ctrl_OutAngleRad);
  XY->Ctrl_OutPWus = XY->Plant_ServoDir*pw + CENTER_PULSE_WIDTH + XY->Plant_OffsetPWus;

#endif

#ifdef _SYS_MET_RAD   //pos in meter, input rad
  
  XY->Ctrl_OutAngleRad = XY->Ctrl_RefPosMet * XY->Ctrl_V
                     - XY->Ctrl_K[0] * XY->KF_X[0]
                     - XY->Ctrl_K[1] * XY->KF_X[1];
										 
  #if ORDER == 3
    XY->Ctrl_OutAngleRad = XY->Ctrl_OutAngleRad - XY->Ctrl_K[2] * XY->KF_K[2];
  #endif

  XY->Ctrl_OutAngleRad = XY->Plant_ServoDir*XY->Ctrl_OutAngleRad;


  // Limit servo angle  |cmdServoAngleRad| <= MAX_SERVO_ANGLE_RAD
  if ( XY->Ctrl_OutAngleRad > MAX_SERVO_ANGLE_RAD )
  { 
    XY->Ctrl_OutAngleRad =    MAX_SERVO_ANGLE_RAD;
  }

  if( XY->Ctrl_OutAngleRad < -MAX_SERVO_ANGLE_RAD )
  {
    XY->Ctrl_OutAngleRad =   -MAX_SERVO_ANGLE_RAD;
  }
  
  //convert into PWM to control motor
  pw = (int16_t) (SLOPE_RAD*XY->Ctrl_OutAngleRad);
  XY->Ctrl_OutPWus = XY->Plant_ServoDir*pw + CENTER_PULSE_WIDTH + XY->Plant_OffsetPWus;


#endif


*/
} // END of bbSSControl( )



/************************************************************************\
*    END OF MODULE Controller.c
\************************************************************************/
