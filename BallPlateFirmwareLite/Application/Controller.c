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

  out_temp = (float) XY->Ctrl_RefPosPxl * XY->Ctrl_V
             - XY->Ctrl_K[0] * XY->KF_X[0]
             - XY->Ctrl_K[1] * XY->KF_X[1]       
             - XY->Ctrl_K[2] * XY->KF_K[2];

  XY->Ctrl_OutAngleDeg = out_temp;

    
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


} // END of bbSSControl( )



/************************************************************************\
*    END OF MODULE Controller.c
\************************************************************************/
