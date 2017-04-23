#include "BallPlate_Plant.h"
#include "UserDef.h"


/**
*  ***********************************************************************
*  @file    Controller.h
*  @author  Chuong Nguyen 
*  @version V1.00
*  @brief   State Space controller for 2nd and 3rd order system.
*
**************************************************************************
*/


/************************************************************************\
*    model of BallPlate Plant in State-Space representation
\************************************************************************/


#if defined _SYS_2ND_ORDER_KF_


//  #define ORDER 2 with ball-plate unit (pixel and degree)
#ifdef _SYS_PXL_DEG

  #define BB_SS_A     {{+1.000000e+00, +5.000000e-02},\
                       {+0.000000e+00, +1.000000e+00}}
  #define BB_SS_B     {-2.658140e-02, -1.063255e+00}
  #define BB_SS_C     {+1.000000e+00, +0.000000e+00}

  #define BB_SS_Q     {{+5.000000e+00, +0.000000e-00},\
                       {+0.000000e+00, +1.00000e+02}}
  #define BB_SS_R     1.0      
                       
  #define BB_SS_V      -2.257
  #define BB_SS_K     {-2.257,         -0.658}            
	
#endif
        
/* PXL_RAD and MET_RAD define
  
//  #define ORDER 2 with SI unit (meter & radian)
#ifdef _SYS_MET_RAD

  #define BB_SS_A     {{+1.000000e+00, +5.000000e-02},\
                       {+0.000000e+00, +1.000000e+00}}
  #define BB_SS_B     {-1.103248e-03, -4.412993e-02}
  #define BB_SS_C     {+1.000000e+00, +0.000000e+00}

  #define BB_SS_Q     {{+3.610000e-03, +0.000000e-00},\
                       {+0.000000e+00, +1.444000e-01}}
  #define BB_SS_R     7.22e-4       // 1pxl = 1/1385 m 

  #define BB_SS_V      -45.305
  #define BB_SS_K     {-45.305,         -14.724}          
	
#endif
  
//  #define ORDER 2 with output in pixel and input in radian
#ifdef _SYS_PXL_RAD

  #define BB_SS_A     {{+1.000000e+00, +5.000000e-02},\
                       {+0.000000e+00, +1.000000e+00}}
  #define BB_SS_B     {-1.52852e+00, -6.1140825e+01}
  #define BB_SS_C     {+1.000000e+00, +0.000000e+00}

  #define BB_SS_Q     {{+3.610000e-03, +0.000000e-00},\
                       {+0.000000e+00, +1.444000e-01}}
  #define BB_SS_R     1       // 1pxl = 1/1385 m 
                       
  #define BB_SS_V      -0.033
  #define BB_SS_K     {-0.033,         -0.011}            
  
#endif
                       
*/                       
                       
                       
#elif defined _SYS_3RD_ORDER_KF_

  #define ORDER 3 
  
  // kga=-21.265, as = 0.25; T = 0.5
  #define BB_SS_A  {{1.0000000e+00,   5.0000001e-02, -2.4894489e-02},\
                    {           0 ,   1.0000000e+00, -9.6367836e-01},\
                    {           0 ,              0 ,  8.1873077e-01}}
  #define BB_SS_B   {-4.9788980e-03,  -1.9273569e-01,   1.6374615e-01}
  #define BB_SS_C   {+1.000000e+00, +0.000000e+00, +0.000000e+00}

  #define BB_SS_Q   {{+5.000000e+00, +0.000000e-00, +0.000000e-00},\
                     {+0.000000e+00, +1.00000e+02, +0.000000e-00},\
                     {+0.000000e+00, +1.00000e+02, +1.00000e+01}}
  #define BB_SS_R    1.0      
                       
  #define BB_SS_V    -0.24
  #define BB_SS_K    {-0.24,         -0.11,  0.0}  
  
#else
//  #undef ORDER
  #error "Define one of '_SYS_2ND_ORDER_KF_' or '_SYS_3RD_ORDER_KF_'"
#endif

 
/************************************************************************\
*    Function reference
\************************************************************************/


void bbSSControl( SS_SysPar *XY);


