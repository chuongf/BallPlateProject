/**
*  ***********************************************************************
*  header file for C functions of Kalman filter
*
*  Chuong Nguyen
*  2017.1.20     Initial version
*                      
**************************************************************************
*/
#include "UserDef.h"
#include "KalmanLib3.h"

// prediction related functions
void Kf_Prd2(SS_SysPar *XY);
 
// measurement update related functions
 void Kf_Upd2(SS_SysPar *XY);

// prediction related functions
void Kf_Prd3(SS_SysPar *XY);
 
// measurement update related functions
 void Kf_Upd3(SS_SysPar *XY);

//void Kf_Prd3(SS_SysPar *XY);

//void Kf_Upd3(SS_SysPar *XY);
