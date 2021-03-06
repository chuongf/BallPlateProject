/**
*  ***********************************************************************
*  @file    UserDef.h
*  @author  Chuong Nguyen 
*  @version V1.00
*  @brief   Define structure, macro, constant...
*
**************************************************************************
*/

#include "BallPlate_Plant.h"
#include "stdint.h"

#ifndef __USER_DEF__
#define __USER_DEF__


/****** Feature options ******/
//#define     Extrapolation
//#define     Test_Kalman
//#define     Debug_Consol
//#define     Overwrite_Kalman

/******* Modeling ******/
//#define   _SYS_2ND_ORDER_KF_
#define   _SYS_3RD_ORDER_KF_

/******* Model Unit ******/
#define _SYS_PXL_DEG
//#define _SYS_MET_RAD
//#define _SYS_PXL_RAD

/******* Input profile ******/
//#define INPUT_JERK_LIMIT




#if defined _SYS_2ND_ORDER_KF_
  #define ORDER 2
  
#elif defined _SYS_3RD_ORDER_KF_
  #define ORDER 3

#else
  #error "Define one of '_SYS_2ND_ORDER_KF_' or '_SYS_3RD_ORDER_KF_'"
#endif


// system operation and flags index

#define     MOD_STANDBY			  0x0000    	//!< bit-wise index of operation mode
#define     MOD_RUNNING			  0x0001    	// 0x0-0xFF
#define     MOD_SETTING	      0x0002    					 
#define     MOD_TESTING	      0x0004    					 

#define     KF_PRED_ENABLE    0x0100    	//!< bit-wise index of Kalman filter
#define     KF_UPD_ENABLE     0x0200    	//!< 0x01xx-0xFFxx

#define     FB_POS_AVAILABLE  0x0400   		//!< bit-wise index of postion Sensor feedback
#define     FB_ACC_AVAILABLE  0x0800   		//!< 

#define     GUI_CMD_AVAILABLE	0x1000   	
#define     GUI_DATA_READY  	0x2000


/** 
*   Struct for complete state-space representation of an Axis.
*/
typedef struct SS_SysPar // State space parameters
{
  // state space representation of model.
  float       Model_F[ORDER][ORDER];    //!< Transition Matrix
  float       Model_G[ORDER];           //!< Input Matrix 
  const float Model_H[ORDER];           //!< Transpose of C Matrix
  
  // Kalman estimation
  float       KF_X[ORDER];              //!< states estimation
  float       KF_PP[ORDER][ORDER];      //!< vairance of states
  float       KF_PU[ORDER][ORDER];      //!< vairance of states
  float       KF_K[ORDER]; 				      //!< kalman gain
  const float KF_Q[ORDER][ORDER];       //!< Variance of modeling
  const float KF_R;                     //!< Variance of measurement
  
  // state space controller.
  float       Ctrl_Ts;                  //!< sampling time [second]
  float       Ctrl_K[ORDER];            //!< full state feedback gain vector
  float       Ctrl_V;                   //!< Pre-filter 
  float       Ctrl_RefPosMet;           //!< Reference input [meter]
  int16_t     Ctrl_RefPosPxl;           //!< Reference input [pixel]
  float       Ctrl_OutAngleRad;         //!< control output value [rad]
  float       Ctrl_OutAngleDeg;         //!< control output value [deg]
  int16_t     Ctrl_OutPWus;

  // plant input and output (ball position).
  int16_t     Plant_OffsetPWus;          //!< offset of servo to have plate balanced
  int8_t      Plant_ServoDir;           //!< servo inverse (-1) or not (1)
  
  int16_t     Meas_OldPosPxl;       //!< current measured position [pxl]
  int16_t     Meas_PosPxl;          //!< current measured position [pxl]
  float       Meas_PosMet;          //!< current measured position [m]

  // ball speed by backward difference
  int32_t     Cal_SpeedPxlPerSec;       //!< current measured position [pxl]
  
} SS_SysPar;


#endif
