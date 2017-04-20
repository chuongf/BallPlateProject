/**
*  ***********************************************************************
*  header file for C functions of Kalman filter
*
*  Chuong Nguyen
*  2017.1.20     Initial version
*                      
**************************************************************************
*/
#include "stdint.h"
#include "BallPlate_Plant.h"


#ifndef BALLPLATE_KFLIB_H            // avoid multiple inclusion
#define BALLPLATE_KFLIB_H




// system order 2


// prediction related functions
void states_prd(float x2_prd[2], float F2[2][2], float g2[2], float u, float x2_upd[2]);
void covar_prd(float P2_prd[2][2], float F2[2][2], float P2_upd[2][2], const float  Q2[2][2]);

// measurement update related functions

void kgain_upd(float klm_gain2[2], const float h2[2], float P2_prd[2][2], const float r2);

void states_upd(float x2_est[2], const float h2[2], float x2_prd[2], float y, float k2[2]);

void covar_upd(float P2_est[2][2], const float h2[2], float P2_prd[2][2], float k2[2]);


#endif  // #ifndef BALLPLATE_KFLIB_H

