
//#define  MATLAB_USE

#ifdef MATLAB_USE
#define  EXPORT_FCNS
#include "shrhelp.h"
#include <mex.h>  /* only needed because of mexFunction below and mexPrintf */
#endif

#include "KalmanLib3.h"

/* Function Definitions */

/*
 * State Prediction from plant model.
 */

  void states_prd3(float  F3[3][3], 
                  float   g3[3], 
                  float   u, 
                  float   x3_upd[3], 
                  float   x3_prd[3])
{
     x3_prd[0] = x3_upd[0] + F3[0][1]*x3_upd[1] + F3[0][2]*x3_upd[2] + g3[0]*u;
     x3_prd[1] =                      x3_upd[1] + F3[1][2]*x3_upd[2] + g3[1]*u;
     x3_prd[2] =                                  F3[2][2]*x3_upd[2] + g3[2]*u;
}


/*
 * Covariance Prediction.
 */
  void covar_prd3(float       F3[3][3], 
                  float       P3_upd[3][3], 
                  const float Q3[3][3],
                  float       P3_prd[3][3])
{
     P3_prd[0][0] = P3_upd[0][0] + Q3[0][0] + F3[0][1]*(P3_upd[0][1] + F3[0][1]*P3_upd[1][1] + F3[0][2]*P3_upd[2][1]) + F3[0][2]*(P3_upd[0][2] + F3[0][1]*P3_upd[1][2] + F3[0][2]*P3_upd[2][2]) + F3[0][1]*P3_upd[1][0] + F3[0][2]*P3_upd[2][0];
     P3_prd[0][1] = P3_upd[0][1] + F3[1][2]*(P3_upd[0][2] + F3[0][1]*P3_upd[1][2] + F3[0][2]*P3_upd[2][2]) + F3[0][1]*P3_upd[1][1] + F3[0][2]*P3_upd[2][1];
     P3_prd[0][2] = F3[2][2]*(P3_upd[0][2] + F3[0][1]*P3_upd[1][2] + F3[0][2]*P3_upd[2][2]);
     P3_prd[1][0] = P3_upd[1][0] + F3[0][1]*(P3_upd[1][1] + F3[1][2]*P3_upd[2][1]) + F3[0][2]*(P3_upd[1][2] + F3[1][2]*P3_upd[2][2]) + F3[1][2]*P3_upd[2][0];
     P3_prd[1][1] = P3_upd[1][1] + Q3[1][1] + F3[1][2]*(P3_upd[1][2] + F3[1][2]*P3_upd[2][2]) + F3[1][2]*P3_upd[2][1];
     P3_prd[1][2] = F3[2][2]*(P3_upd[1][2] + F3[1][2]*P3_upd[2][2]);
     P3_prd[2][0] = F3[2][2]*P3_upd[2][0] + F3[0][1]*F3[2][2]*P3_upd[2][1] + F3[0][2]*F3[2][2]*P3_upd[2][2];
     P3_prd[2][1] = F3[2][2]*P3_upd[2][1] + F3[1][2]*F3[2][2]*P3_upd[2][2];
     P3_prd[2][2] = P3_upd[2][2]*F3[2][2]*F3[2][2] + Q3[2][2];
}


/*
 * Kalman gain estimation.

 */
  void kgain_upd3(const float h3[3], 
                  float       P3_prd[3][3], 
                  float       r3, 
                  float       klm_gain3[3])
{
  (void)h3;
     klm_gain3[0] = P3_prd[0][0]/(P3_prd[0][0] + r3);
     klm_gain3[1] = P3_prd[1][0]/(P3_prd[0][0] + r3);
     klm_gain3[2] = P3_prd[2][0]/(P3_prd[0][0] + r3);

}

/*
 * state update estimation.
 */
  void states_upd3( const float h3[3], 
                    float       x3_prd[3], 
                    float       y, 
                    float       k3[3], 
                    float       x3_upd[3])
{
  (void)h3;
     
     x3_upd[1] = x3_prd[1] - k3[1]*(x3_prd[0] - y);
     x3_upd[2] = x3_prd[2] - k3[2]*(x3_prd[0] - y);
     x3_upd[0] = x3_prd[0] - k3[0]*(x3_prd[0] - y);

}

/*
 * covariance update function.
 */
void covar_upd3(const float h3[3], 
                float       P3_prd[3][3], 
                float       k3[3],
                float       P3_upd[3][3])
{
  (void)h3;
     P3_upd[0][0] = P3_prd[0][0] - P3_prd[0][0]*k3[0];
     P3_upd[0][1] = P3_prd[0][1] - P3_prd[0][1]*k3[0];
     P3_upd[0][2] = P3_prd[0][2] - P3_prd[0][2]*k3[0];
     P3_upd[1][0] = P3_prd[1][0] - P3_prd[0][0]*k3[1];
     P3_upd[1][1] = P3_prd[1][1] - P3_prd[0][1]*k3[1];
     P3_upd[1][2] = P3_prd[1][2] - P3_prd[0][2]*k3[1];
     P3_upd[2][0] = P3_prd[2][0] - P3_prd[0][0]*k3[2];
     P3_upd[2][1] = P3_prd[2][1] - P3_prd[0][1]*k3[2];
     P3_upd[2][2] = P3_prd[2][2] - P3_prd[0][2]*k3[2];
}

/* this function exists so that mex may be used to compile the library
   it is not otherwise needed */
#ifdef MATLAB_USE
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[] )
{
}
#endif
