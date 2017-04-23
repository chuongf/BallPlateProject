#ifndef BALLPLATE_KFLIB_H            // avoid multiple inclusion
#define BALLPLATE_KFLIB_H

//#include "shrhelp.h"

/* Function Definitions */

/*
 * State Prediction from plant model.
 */
void states_prd3(float F3[3][3], 
                 float g3[3], 
                 float u, 
                 float x3_upd[3], 
                 float x3_prd[3]);

/*
 * Covariance Prediction.
 */
void covar_prd3(float       F3[3][3], 
                float       P3_upd[3][3], 
                const float Q3[3][3],
                float       P3_prd[3][3]);


/*
 * Kalman gain estimation.

 */
void kgain_upd3(const float h3[3], 
                float       P3_prd[3][3], 
                float       r3, 
                float       klm_gain3[3]);

/*
 * state update estimation.
 */
void states_upd3(const float h3[3], 
                 float       x3_prd[3], 
                 float       y, 
                 float       k3[3], 
                 float       x3_upd[3]);

/*
 * covariance update function.
 */
void covar_upd3(const float h3[3], 
                float       P3_prd[3][3], 
                float       k3[3],
                float       P3_upd[3][3]);

#endif  // #ifndef BALLPLATE_KFLIB_H
