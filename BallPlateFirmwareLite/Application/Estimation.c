
#include "Estimation.h"
#include "KalmanLib2.h"
#include "KalmanLib3.h"


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
