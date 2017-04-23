
#include "Estimation.h"
#include "KalmanLib2.h"
#include "KalmanLib3.h"

/**
  * @brief  Calculate Kalman filter for prediction states and variance
  * @param  
  * @retval States & Variance
  */
	
#ifdef _SYS_2ND_ORDER_KF_
void Kf_Prd2(SS_SysPar *XY)
{
  
    #ifdef Extrapolation  
    XY->Model_F[0][1] = XY->Ctrl_Ts;
    XY->Model_G[0] = -0.4414*XY->Ctrl_Ts*XY->Ctrl_Ts;   //0.5*kbb*T*T
    XY->Model_G[1] = -0.8829*XY->Ctrl_Ts;               //kbb*T
    #endif
    
    states_prd(XY->KF_X,XY->Model_F,XY->Model_G,XY->Ctrl_OutAngleDeg,XY->KF_X);
    
    covar_prd(XY->KF_PP,XY->Model_F,XY->KF_PU, XY->KF_Q);
    
}

/**
  * @brief  Calculate Kalman filter for update states and variance
  * @param  
  * @retval States, Variance and Kalman gain.
  */
void Kf_Upd2(SS_SysPar *XY)
{
  
  #ifdef Extrapolation
    XY->Model_F[0][1] = XY->Ctrl_Ts; 
  #endif
  //  void kgain_upd(float klm_gain2[2], float h2[2], float P2_prd[4], float r2);
    kgain_upd(XY->KF_K, XY->Model_H, XY->KF_PP,XY->KF_R);
    
  //  void states_upd(float x2_est[2], float h2[2], float x2_prd[2], float y, float k2[2]);
    states_upd(XY->KF_X,XY->Model_H,XY->KF_X,XY->Meas_PosPxl,XY->KF_K);

    //  void covar_upd(float P2_est[4], float h2[2], float P2_prd[4], float k2[2]);
    covar_upd(XY->KF_PU,XY->Model_H,XY->KF_PP,XY->KF_K);
}

#endif


#ifdef _SYS_3RD_ORDER_KF_

/**
  * @brief  Calculate Kalman filter for prediction states and variance
  * @param[in,out]   *XY 				points to the axis structure.   
  * @retval none
  */
void Kf_Prd3(SS_SysPar *XY)
{
    states_prd3(XY->Model_F,XY->Model_G,XY->Ctrl_OutAngleDeg,XY->KF_X,XY->KF_X);

    covar_prd3(XY->Model_F,XY->KF_PU, XY->KF_Q,XY->KF_PP);
}

  /**
  * @brief  Calculate Kalman filter for update states and variance
  * @param[in,out]   *XY 				points to the axis structure  
  * @retval none
  */
void Kf_Upd3(SS_SysPar *XY)
{

    kgain_upd3(XY->Model_H, XY->KF_PP,XY->KF_R,XY->KF_K);
    
    states_upd3(XY->Model_H,XY->KF_X,XY->Meas_PosPxl,XY->KF_K,XY->KF_X);
                  
    covar_upd3(XY->Model_H,XY->KF_PP,XY->KF_K,XY->KF_PU);
  
}

#endif
