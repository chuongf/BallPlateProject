
//#define  MATLAB_USE


#ifdef MATLAB_USE
//#ifndef  STM32_VERSION
#define  EXPORT_FCNS
#include "shrhelp.h"
#include <mex.h>  /* only needed because of mexFunction below and mexPrintf */
#endif

#include "KalmanLib2.h"

/* Function Definitions */
void states_prd(float x2_prd[2], float F2[2][2], float g2[2], float u, float x2_upd[2])
{
  x2_prd[0] = x2_upd[0] + F2[0][1] * x2_upd[1] + g2[0] * u;
  x2_prd[1] = x2_upd[1] +                        g2[1] * u;

// x2_prd(1,1) = x2_upd(1,1) + F2(1,2)*x2_upd(2,1) + g2(1,1)*u;
// x2_prd(2,1) = x2_upd(2,1) +                       g2(2,1)*u;

}

void covar_prd(float P2_prd[2][2], float F2[2][2], float P2_upd[2][2], const float  Q2[2][2])
{

  P2_prd[0][0] = Q2[0][0] + P2_upd[0][0] + F2[0][1] * P2_upd[1][0] + F2[0][1] * P2_upd[0][1] + F2[0][1] * F2[0][1] * P2_upd[1][1];
  P2_prd[1][0] = P2_upd[1][0] + F2[0][1] * P2_upd[1][1];
  P2_prd[0][1] = P2_upd[0][1] + F2[0][1] * P2_upd[1][1];
  P2_prd[1][1] = Q2[1][1] + P2_upd[1][1];

// % P2_prd(1,1) = Q2(1,1) + P2_upd(1,1) + F2(1,2)*P2_upd(2,1) + F2(1,2)*P2_upd(1,2) + F2(1,2)*F2(1,2)*P2_upd(2,2);
// % P2_prd(2,1) =                           P2_upd(2,1) +                       F2(1,2)*P2_upd(2,2);
// % P2_prd(1,2) =                                           P2_upd(1,2) +      F2(1,2)*P2_upd(2,2);
// % P2_prd(2,2) = Q2(2,2) + P2_upd(2,2);
// %  

}


void kgain_upd(float klm_gain2[2], const float h2[2], float P2_prd[2][2],const float r2)
{
  (void)h2; // for h2 = [1, 0]
  klm_gain2[0] = P2_prd[0][0] / (P2_prd[0][0] + r2);
  klm_gain2[1] = P2_prd[1][0] / (P2_prd[0][0] + r2);
  
  (void)h2; // for h2 = [1, 1]
//  klm_gain2[0] = (P2_prd[0,0] + P2_prd[0,1])/(P2_prd[0,0] + P2_prd[0,1] + P2_prd[1,0] + P2_prd[1,1] + r2);
//  klm_gain2[1] = (P2_prd[1,0] + P2_prd[1,1])/(P2_prd[0,0] + P2_prd[0,1] + P2_prd[1,0] + P2_prd[1,1] + r2);

}


void states_upd(float x2_est[2],const float h2[2], float x2_prd[2], float y, float k2[2])
{
  (void)h2;
  x2_est[0] = x2_prd[0] + k2[0] * (y - x2_prd[0]);
  x2_est[1] = x2_prd[1] + k2[1] * (y - x2_prd[0]);
  
  // for h2 = [1, 1]
//  x2_upd[0] = x2_prd[0] - k2[0]*(x2_prd[0] + x2_prd[1] - y);
//  x2_upd[1] = x2_prd[1] - k2[1]*(x2_prd[0] + x2_prd[1] - y);
}


/* Function Definitions */
void covar_upd(float P2_est[2][2],const float h2[2], float P2_prd[2][2], float k2[2])
{
  (void)h2;
  P2_est[0][0] = P2_prd[0][0] - k2[0] * P2_prd[0][0];
  P2_est[1][0] = P2_prd[1][0] - k2[1] * P2_prd[0][0];
  P2_est[0][1] = P2_prd[0][1] - k2[0] * P2_prd[0][1];
  P2_est[1][1] = P2_prd[1][1] - k2[1] * P2_prd[0][1];
  
// for h2 = [1, 1]
// P2_upd[0,0] = P2_prd[0,0] - P2_prd[0,0]*k2[0] - P2_prd[1,0]*k2[0];
// P2_upd[0,1] = P2_prd[0,1] - P2_prd[0,1]*k2[0] - P2_prd[1,1]*k2[0];
// P2_upd[1,0] = P2_prd[1,0] - P2_prd[0,0]*k2[1] - P2_prd[1,0]*k2[1];
// P2_upd[1,1] = P2_prd[1,1] - P2_prd[0,1]*k2[1] - P2_prd[1,1]*k2[1];

}

/* this function exists so that mex may be used to compile the library
   it is not otherwise needed */
#ifdef MATLAB_USE
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[] )
{
}
#endif
