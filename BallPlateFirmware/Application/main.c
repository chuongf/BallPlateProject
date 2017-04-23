/**
*  ***********************************************************************
*  @file    main.c
*  @author  Chuong Nguyen
*  @brief   BallPlate firmware on ST32F10x. Details in Abstract.txt
*
*  @version Beta: Kalman estimation using 2nd order and 3rd order.
*                 State Space Controller.
*                 GUI support.
*
*  @note    Refer UserDef.h         for opereation options
*           Refer Controller.h      for initial value of matrices
*           Refer BallPlate_Plant.h for physical parameters of plant
*/

#define     SW_VERSION        "Beta170405_01"    //!< version string


/**
*  ***********************************************************************
*  Include headers   
*  ***********************************************************************
*/

#include  <stdio.h>             //!< for printf
#include "stm32f10x.h"          //!< STM32F10x Peripherals Definitions
#include "STM32_Init.h"         //!< STM32F10x Peripherals initialization	
#include "stm32_pwm.h"          //!< PWM for servo motor
#include "service_led.h"        //!< LED service
#include "service_com.h"        //!< Servo service

#include "Estimation.h"
#include "Controller.h"         //!< State Space control variables
#include "UserDef.h"


/**
*  ***********************************************************************
*  Globle Variables   
*  ***********************************************************************
*/

uint32_t StatusFlags = 0;         //!< system operation status. Initialize to Standby mode

uint32_t Tick100usCnt = 0;
uint32_t SampleEpoch100us = 0;
uint16_t deltaT100usCnt = 0;
float    deltaT;								  //!< Sampling time

// for usart1 and usart2
char      Rx1Buf[32];
char      Rx1GUIFrame[32];		

char 		  Rx2Buf[12];
uint8_t   Rx2PosFrame[12]={0};		//!< rx camera data buffer (11 bytes)



/**
*  ***********************************************************************
*  Function declearation   
*  ***********************************************************************
*/

void Delay_ms(uint32_t delayTime_ms);
void printPredict_console(const struct SS_SysPar *xAxis, const struct SS_SysPar *yAxis);
void printMeasure_console(const struct SS_SysPar *xAxis, const struct SS_SysPar *yAxis);
void printPlotData_GUI(const struct SS_SysPar *xAxis, const struct SS_SysPar *yAxis);
void sendData2CAMPC(const struct SS_SysPar *X, const struct SS_SysPar *Y);
int  ascii2int( char *pintstring );

/**
*  **********************************************************************
*  Main function  
*  ************************************************************************
*/
int main(void)
{
	
  
  /***********************************************************************
  *  Local Variables   
  *  ********************************************************************/
  
	// X-axis parameters for state-space and kalman filter
 	SS_SysPar xAxis = {
    // model.
 		.Model_F = BB_SS_A,
    .Model_G = BB_SS_B,
    .Model_H = BB_SS_C,
    
    // Kalman estimation
 		.KF_X = 0,             //!< estimated states
    .KF_PP = 0,            //!< variance of predicted states
    .KF_PU = 0,            //!< variance of updated states
    .KF_K =0,              //!< internal gain of kalman
    
 		.KF_Q = BB_SS_Q,
    .KF_R = BB_SS_R, 
    
    // state space controller.
 		.Ctrl_Ts = 0.05,       //!< sampling time
    .Ctrl_K = BB_SS_K,     //!< controller gain
    .Ctrl_V = BB_SS_V,     //!< controller input filter
    .Ctrl_RefPosMet = 0,
    .Ctrl_RefPosPxl = 0,
 		.Ctrl_OutAngleRad = 0, //!< controller output
    .Ctrl_OutAngleDeg = 0,
    
    // plant input (plat angle) and output (ball position).
    .Ctrl_OutPWus = 0,
    .Plant_OffsetPWus=PWM_SERVO_X_OFFSET,
    .Plant_ServoDir= -1,
    
    .Meas_OldPosPxl = 0,
 		.Meas_PosPxl = 0,
    .Meas_PosMet = 0,
    
    // ball speed calculated by backward difference
    .Cal_SpeedPxlPerSec = 0
  };	
  
 	
  // Y-axis parameters for state-space and kalman filter
  SS_SysPar yAxis = {
    // model.
 		.Model_F = BB_SS_A,
    .Model_G = BB_SS_B,
    .Model_H = BB_SS_C,
    
    // Kalman estimation
 		.KF_X = 0,             //!< estimated states
    .KF_PP = 0,            //!< variance of predicted states
    .KF_PU = 0,            //!< variance of updated states
    .KF_K =0,              //!< internal gain of kalman
        
 		.KF_Q = BB_SS_Q,
    .KF_R = BB_SS_R, 
    
    // state space controller.
 		.Ctrl_Ts = 0.05,       //!< sampling time
    .Ctrl_K = BB_SS_K,     //!< controller gain
    .Ctrl_V = BB_SS_V,     //!< controller input filter
    .Ctrl_RefPosMet = 0,
    .Ctrl_RefPosPxl = 0,
 		.Ctrl_OutAngleRad = 0, //!< controller output
    .Ctrl_OutAngleDeg = 0,
    
    // plant input (plat angle) and output (ball position).
    .Ctrl_OutPWus = 0,
    .Plant_OffsetPWus=PWM_SERVO_Y_OFFSET,
    .Plant_ServoDir=1,
    
    .Meas_OldPosPxl = 0,
 		.Meas_PosPxl = 0,
    .Meas_PosMet = 0,
    
    // ball speed calculated by backward difference
    .Cal_SpeedPxlPerSec = 0
  };		 

  // temporary variables	
  int16_t   kSampleIndex=0;
  char      Value_temp[6];   
  uint8_t   i;
  int32_t   temp;
  float posDiff;
  #ifdef Test_Kalman
  // temporary variables for testing kalman algorithm
  float u_Deg[10] = {0.60, 0.50, 0.40, -0.60, -0.40, -0.10, 0, -0.80, -0.60, -0.30};
  float y_PXL[10] = {502,502,502, 501,501,501,501,500,500,500};
  #endif
  
  /***********************************************************************
  *  Local Variables   
  *  ********************************************************************/
	buffer_Init();          //!< init RX / TX buffers
	stm32_Init();						//!< Timer4, Uart1, Uart2, GPIO
	SysTick_Config(7200);   //!< System timer 
	//default HCLK 72MHz. need 7200 ticks for 100us interval interrupt
	#ifdef Test_Kalman
  StatusFlags |= MOD_TESTING;                   //!< default Standby mode
  #else
  StatusFlags |= MOD_RUNNING;                   //!< default Standby mode
  #endif
  if(StatusFlags & MOD_SETTING) LED_On(13);     //!< if default Setting mode
  if(StatusFlags & MOD_RUNNING) LED_On(14);     //!< if default Running mode
  if(StatusFlags & MOD_TESTING) LED_On(15);     //!< if default Testing mode
  
  setPwmXDuty_us (CENTER_PULSE_WIDTH + xAxis.Plant_OffsetPWus);
  setPwmYDuty_us (CENTER_PULSE_WIDTH + yAxis.Plant_OffsetPWus);
  
  while (1)
  {
		if(StatusFlags & MOD_RUNNING)             //!< Running mode
		{
      /// Default: input in degree, output in pixel
      
			if(StatusFlags & KF_UPD_ENABLE)                         // got measurement: kalman Prediction & Updating
      {

				// checksum if the received data is correct, e.g. 0D 01 78 01 01 00 FF 00 FF FB 88
  			if(checkTransfer(Rx2PosFrame))          //!< return 1: no error
        {
          // save old position measurement values
          xAxis.Meas_OldPosPxl = xAxis.Meas_PosPxl;
          yAxis.Meas_OldPosPxl = yAxis.Meas_PosPxl;
     
          // 're-assemble' UINT16 coordinate values from NEW RECEIVED PC message
          xAxis.Meas_PosPxl = (Rx2PosFrame[1] << 8) + Rx2PosFrame[2];
          yAxis.Meas_PosPxl = (Rx2PosFrame[3] << 8) + Rx2PosFrame[4];
          xAxis.Ctrl_RefPosPxl  = (Rx2PosFrame[5] << 8) + Rx2PosFrame[6];
          yAxis.Ctrl_RefPosPxl  = (Rx2PosFrame[7] << 8) + Rx2PosFrame[8];

          
          if ( kSampleIndex < 3 ) //!< keep everything static in first 3 controller cycles
          {
            kSampleIndex++;
            
            xAxis.KF_X[0] = xAxis.Meas_PosPxl;
            xAxis.KF_X[1] = 0.0;
            bbSSControl(&xAxis);	  //!< control calculation for xAxis

            yAxis.KF_X[0] = yAxis.Meas_PosPxl;
            yAxis.KF_X[1] = 0.0;
            bbSSControl(&yAxis);	  //!< control calculation for yAxis
            
            #if ORDER == 3        
              xAxis.KF_X[2] = 0.0;
              yAxis.KF_X[2] = 0.0;
            #endif
          }
          else
          {
            xAxis.Ctrl_Ts = deltaT;
            yAxis.Ctrl_Ts = deltaT;
            xAxis.Cal_SpeedPxlPerSec = (xAxis.Meas_PosPxl - xAxis.Meas_OldPosPxl) / xAxis.Ctrl_Ts ;    // scaled in pxl/s
            yAxis.Cal_SpeedPxlPerSec = (yAxis.Meas_PosPxl - yAxis.Meas_OldPosPxl) / yAxis.Ctrl_Ts;    // scaled in pxl/s
             
            #ifdef Overwrite_Kalman
//            xAxis.Ctrl_OutAngleDeg = 0;
//            yAxis.Ctrl_OutAngleDeg = 0;
            #endif
            
            // x direction 
            LED_On(11);					          //!< for measuring kalman computing time
            #if ORDER == 2
            Kf_Prd2(&xAxis);					//!< calculate kalman prediction (input Rad)
            Kf_Upd2(&xAxis);					//!< calculate kalman update. (use Meter)
            #endif 
            #if ORDER == 3
            Kf_Prd3(&xAxis);					//!< calculate kalman prediction (input Rad)
            Kf_Upd3(&xAxis);					//!< calculate kalman update. (use Meter)
            #endif            
            LED_Off(11);					        // 
            // test: overwrite estimated speed by backward calculation when ball stop
            posDiff = (xAxis.Ctrl_RefPosPxl - xAxis.KF_X[0]);
//            if(posDiff<0) posDiff = -posDiff;            
//            if(xAxis.Cal_SpeedPxlPerSec == 0 && posDiff > 10){
//              xAxis.KF_X[1] = (int)xAxis.KF_X[1]/1.2;
//              xAxis.KF_PU[1][1] = xAxis.KF_PU[1][1]/1.2;
//            }
            bbSSControl(&xAxis);	        //!< control calculation for xAxis
            setPwmXDuty_us(xAxis.Ctrl_OutPWus + CENTER_PULSE_WIDTH + xAxis.Plant_OffsetPWus);

            // y direction
            #if ORDER == 2
            Kf_Prd2(&yAxis);					//!< calculate kalman prediction (input Rad)
            Kf_Upd2(&yAxis);					//!< calculate kalman update. (use Meter)
            #endif 
            #if ORDER == 3
            Kf_Prd3(&yAxis);					//!< calculate kalman prediction (input Rad)
            Kf_Upd3(&yAxis);					//!< calculate kalman update. (use Meter)
            #endif
            // test: overwrite estimated speed by backward calculation when ball stop
            posDiff = (yAxis.Ctrl_RefPosPxl - yAxis.KF_X[0]);
//            if(posDiff<0) posDiff = -posDiff; 
//            if(yAxis.Cal_SpeedPxlPerSec  == 0 && posDiff > 10){
//              yAxis.KF_X[1] = (int)yAxis.KF_X[1]/1.2;
//              yAxis.KF_PU[1][1] = yAxis.KF_PU[1][1]/1.2;
//            }
            bbSSControl(&yAxis);	        //!< control calculation for yAxis
            setPwmYDuty_us(yAxis.Ctrl_OutPWus + CENTER_PULSE_WIDTH + yAxis.Plant_OffsetPWus);
            
            // set/lear flags
            StatusFlags &= ~KF_UPD_ENABLE;	  //!< clear Flag when finish
            
            // export results 
			StatusFlags |= GUI_DATA_READY;							  
            #ifdef Debug_Consol
            printf(" \n\r");
            printPredict_console(&xAxis, &yAxis);     //!<to console
            Delay_ms(2);
            printMeasure_console(&xAxis, &yAxis);     //!<to console
            Delay_ms(2);
            printf(" \n\r");
            #endif
            sendData2CAMPC(&xAxis,&yAxis);            //!<to Logger
          }
        }
				
//				else 
//          StatusFlags &= ~(KF_UPD_ENABLE);	//!< can't do update because of feedback error
			}
			
      #ifdef Extrapolation
			if((StatusFlags & KF_PRED_ENABLE) && (kSampleIndex >= 3)) //!< no measurement: kalman prediction only
      {						
				xAxis.Ctrl_Ts = deltaT;       //!< get sampling time
        #if ORDER == 2
        Kf_Prd2(&xAxis);					//!< calculate kalman prediction (input Rad)
        #elseif ORDER == 3
        Kf_Prd3(&xAxis);					//!< calculate kalman prediction (input Rad)
        #endif
        bbSSControl(&xAxis);          //!< State Space control calculation. Return angleXY
        setPwmXDuty_us(xAxis.Ctrl_OutPWus + CENTER_PULSE_WIDTH + xAxis.Plant_OffsetPWus);
        
				yAxis.Ctrl_Ts = deltaT;       //!< get sampling time
        #if ORDER == 2
        Kf_Prd2(&yAxis);					//!< calculate kalman prediction (input Rad)
        #elseif ORDER == 3
        Kf_Prd3(&yAxis);					//!< calculate kalman prediction (input Rad)
        #endif
        bbSSControl(&yAxis);          //!< State Space control calculation. Return angleXY
        setPwmYDuty_us(yAxis.Ctrl_OutPWus + CENTER_PULSE_WIDTH + yAxis.Plant_OffsetPWus);
 				
				StatusFlags &= ~KF_PRED_ENABLE;	    //!< clear Flag when finish
        
        // export results 
        #ifdef Debug_Consol
        printPredict_console(&xAxis, &yAxis);   //!< to console
        #endif
			}	
      #endif
		}
    
		else if(StatusFlags & MOD_SETTING)			  //!< Setting mode
		{
      if((StatusFlags & GUI_CMD_AVAILABLE) && (Rx1GUIFrame[1] != 0))       //!< subframe for setting
      {
        StatusFlags &= ~GUI_CMD_AVAILABLE;
        if(Rx1GUIFrame[0] == 'S' && Rx1GUIFrame[1] == 'G')//!< set control feedback gains
        {
          
          for(i = 5; i<11; i++){        //!< decode number
            Value_temp[i-5] = Rx1GUIFrame[i];
            if(Rx1GUIFrame[i] == 0)  break;
          }
          
          temp = ascii2int(Value_temp); //!< 1000 times of real value
          
          if(Rx1GUIFrame[3] == '1') {      //!< first element of feedback gains
            xAxis.Ctrl_K[0] = (float)temp/1000;
            yAxis.Ctrl_K[0] = xAxis.Ctrl_K[0];
            printf ("FB gain 1 =  %d/1000\n",temp);
          }
          if(Rx1GUIFrame[3] == '2') {      //!< second element of feedback gains
            xAxis.Ctrl_K[1] = (float)temp/1000;
            yAxis.Ctrl_K[1] = xAxis.Ctrl_K[1];
            printf ("FB gain 2 =  %d/1000\n",temp);
          }
          if(Rx1GUIFrame[3] == '3') {      //!< third element of feedback gains (3rd order only)
            #if ORDER == 3
            xAxis.Ctrl_K[2] = (float)temp/1000;
            yAxis.Ctrl_K[2] = xAxis.Ctrl_K[1];
            printf ("FB gain 3 =  %d/1000\n",temp);
            #endif
		  }
            
        }
        
        if(Rx1GUIFrame[0] == 'S' && Rx1GUIFrame[1] == 'F')//!< set control filter
        {
          // decode control gain value
          for(i = 5; i<11; i++){
            Value_temp[i-5] = Rx1GUIFrame[i];
            if(Rx1GUIFrame[i] == 0)  break;
          }
          
          temp = ascii2int(Value_temp); //!< 100 times of real value
          
          if(Rx1GUIFrame[3] == '1') { 
            xAxis.Ctrl_V = (float)temp/1000;
            yAxis.Ctrl_V = xAxis.Ctrl_V;
            printf ("Filter gain =  %d/1000\n",temp);
          }
            
        }
        if(Rx1GUIFrame[0] == 'S' && Rx1GUIFrame[1] == 'O')//!< set servo offset
        {
          // decode offset value
          for(i = 5; i<11; i++)
          {
            Value_temp[i-5] = Rx1GUIFrame[i];
            if(Rx1GUIFrame[i] == 0)  break;
          }
          temp = ascii2int(Value_temp); //!< 1 times of real value
          
          if(Rx1GUIFrame[3] == 'X') {      //!< Offset for X-axis
            xAxis.Plant_OffsetPWus = temp;
            setPwmXDuty_us (CENTER_PULSE_WIDTH + temp);
            printf ("Offset X =  %d saved\n",temp);
          }
          if(Rx1GUIFrame[3] == 'Y') {      //!< Offset for Y-axis
            yAxis.Plant_OffsetPWus = temp;
            setPwmYDuty_us (CENTER_PULSE_WIDTH + temp);
            printf ("Offset Y =  %d saved\n",temp);
          }
        }
        
      }
		}
    


		else if(StatusFlags & MOD_TESTING)			  //!< Testing mode
		{
      #ifdef Test_Kalman
      // test kalman algorithm => checked with matlab
      // get measured sampling time
      xAxis.Ctrl_Ts = 0.05;
      yAxis.Ctrl_Ts = 0.05;
            
      // initial step
      xAxis.KF_X[0] = y_PXL[0];
      // test for 10 values and compare with matlab
      for (i = 1; i <10; i++)
      {
        // x direction only u_Rad y_Met
        xAxis.Ctrl_OutAngleDeg = u_Deg[i-1];   //
        xAxis.Meas_PosPxl = y_PXL[i];
        #if ORDER == 2
        Kf_Prd2(&xAxis);					//!< calculate kalman prediction (input Rad)
        Kf_Upd2(&xAxis);					//!< calculate kalman update. (use Meter)
        #endif
        #if ORDER == 3
        Kf_Prd3(&xAxis);					//!< calculate kalman prediction (input Rad)
        Kf_Upd3(&xAxis);					//!< calculate kalman update. (use Meter)
        #endif
      }
      #endif      
		}
    

		else 	                                    //!< Standby mode (no others mode selected)
		{
			// this mode is used for Demo in future.
      Delay_ms(5);
		}

    if((StatusFlags & GUI_CMD_AVAILABLE) && (Rx1GUIFrame[1] == 0))   //!< check if operation mode change.
    {
               
      if(Rx1GUIFrame[0] == 'S')      //!< command to Setting Mode 
      { 
        StatusFlags |= MOD_SETTING;
        StatusFlags &= ~(MOD_RUNNING | MOD_TESTING);         
        printf ("System is in Setting Mode \n");
        LED_On(13);
        LED_Off(14);
        LED_Off(15);
		StatusFlags &= ~GUI_CMD_AVAILABLE;								  
      }
      if(Rx1GUIFrame[0] == 'R')      //!< command to Run Mode
      { 
        StatusFlags |= MOD_RUNNING;  
        StatusFlags &= ~(MOD_SETTING | MOD_TESTING); 
        printf ("System is in Running Mode \n");        
        LED_Off(13);         
        LED_On(14);
        LED_Off(15);
		StatusFlags &= ~GUI_CMD_AVAILABLE;								  
      }
      if(Rx1GUIFrame[0] == 'T')      //!< command to Test Mode
      { 
        StatusFlags |= MOD_TESTING;  
        StatusFlags &= ~(MOD_RUNNING | MOD_SETTING);
        printf ("System is in Test Mode \n");
        LED_Off(13);
        LED_Off(14);
        LED_On(15);
		StatusFlags &= ~GUI_CMD_AVAILABLE;								  
      }

      if(Rx1GUIFrame[0] == 'D')      //!< command to Demo/Standby Mode 
      { 
        StatusFlags &= ~(MOD_RUNNING | MOD_TESTING | MOD_SETTING);
        printf ("System is in Standby Mode \n");
        LED_Off(13);
        LED_Off(14);
        LED_On(15);
		StatusFlags &= ~GUI_CMD_AVAILABLE;								  
      }
      
      if(Rx1GUIFrame[0] == 'G')      //!< command to read all current settings
      { 
        printf("G,%d,%d",StatusFlags&0x00FF,ORDER);
        printf(",%d,%d,%d,%d",
                (int32_t)(1000*xAxis.Ctrl_K[0]),
                (int32_t)(1000*xAxis.Ctrl_K[1]),
                (int32_t)(1000*xAxis.Ctrl_K[2]),
                (int32_t)(1000*xAxis.Ctrl_V));
        printf(",%d,%d\n",xAxis.Plant_OffsetPWus,yAxis.Plant_OffsetPWus);
		StatusFlags &= ~GUI_CMD_AVAILABLE;								  
      }
      
      if(Rx1GUIFrame[0] == 'V')      //!< command to save setting to flash/eeprom
      { 
        // save setting to flash/eeprom
        printf("Save data to eeprom/flash is not supported at the moment\n");
		StatusFlags &= ~GUI_CMD_AVAILABLE;
      }
      if(Rx1GUIFrame[0] == 'P')      //!< command to get states for plotting
      { 
        if(StatusFlags & GUI_DATA_READY){
          StatusFlags &= ~GUI_DATA_READY;
          printPlotData_GUI(&xAxis, &yAxis);
          StatusFlags &= ~GUI_CMD_AVAILABLE;
        }
      }
    }  
  }
}


/**
*  ***********************************************************************
*  System interrupt service handler   
*  ***********************************************************************
*/

void SysTick_Handler(void)
{ 
	Tick100usCnt++;							            //!< system run time
	if(Tick100usCnt&0x1000) LED_Toggle(10); //!< blinking LED10 for about 0.8Hz to show that i'm alive!
	deltaT100usCnt++;						            //!< use for determining sampling time only.
	
	if(StatusFlags & FB_POS_AVAILABLE) {	  //!< check if Position feedback is available
    StatusFlags &= ~FB_POS_AVAILABLE;

		StatusFlags |= KF_UPD_ENABLE;				  //!< enable state update calculation
		deltaT = (float)deltaT100usCnt/10000; //!< convert to second
    SampleEpoch100us = Tick100usCnt;      //!< for ploting on GUI
		deltaT100usCnt = 0; 			            //!< reset for measuring sampling time
    
		LED_Toggle(11);       			          //!< indicate position feedback from camera
	}

  #ifdef Extrapolation  
	if(deltaT100usCnt == 100){	            //!< predict state when no Pos feedback for 10ms.
		Start_PWMXY();						            //!< Start PWM here to optimize process time
		StatusFlags |= KF_PRED_ENABLE;			  //!< enable state prediction calculatin					
		deltaT = (float)deltaT100usCnt/10000; //!< convert to second
    SampleEpoch100us = Tick100usCnt;      //!< for ploting on GUI
		deltaT100usCnt = 0; 			            //!< reset for measuring sampling time
	}
  #endif  
}

/**
*  ***********************************************************************
*  Functions definition
*  ***********************************************************************
*/

void Delay_ms(uint32_t delayTime_ms)
{ 
	uint32_t startTick, delayTime_100us;
	startTick = Tick100usCnt;
  delayTime_100us = delayTime_ms*10;	      //!< if msTicks counts up every 100us
	while(Tick100usCnt - startTick < delayTime_100us);
  
}


void printPredict_console(const struct SS_SysPar *xAxis, const struct SS_SysPar *yAxis)
{
// print to consol position every 10ms
// |ID = E|Order=2|,|Ts|,|X1|,Y1|,|X2|,|Y2|,|LF|
printf("E2_X: dT=%2u, u=%3d, X1=%3d, X2=%3d  |  ",
      (int)(xAxis->Ctrl_Ts*1000),
      (int)(xAxis->Ctrl_OutAngleDeg),
      (int)(xAxis->KF_X[0]),
      (int)(xAxis->KF_X[1]));

printf("E2_Y: dT=%2u, u=%3d, Y1=%3d, Y2=%3d\n\r",
      (int)(yAxis->Ctrl_Ts*1000),
      (int)(yAxis->Ctrl_OutAngleDeg),
      (int)(yAxis->KF_X[0]), 
      (int)(yAxis->KF_X[1]));

}

void printMeasure_console(const struct SS_SysPar *xAxis, const struct SS_SysPar *yAxis)
{
// print to consol position every update data is available
// |ID = M|Order=2|,|Ts|,|X1|,Y1|,|X2|,|Y2|,|LF|
printf("M2_X:               X1=%3d, X2=%3d  |  ",
      (int)xAxis->Meas_PosPxl, 
      xAxis->Cal_SpeedPxlPerSec);
  
printf("M2_Y:               Y1=%3d, Y2=%3d",
      (int)yAxis->Meas_PosPxl,
      yAxis->Cal_SpeedPxlPerSec);
}

void printPlotData_GUI(const struct SS_SysPar *xAxis, const struct SS_SysPar *yAxis)
{
  // print to GUI consol when requested: 
  // |P2(3),X,Ts_ms,U_PWM,X1_Pxl,X2,X3,PX1,PX2,Xref_Pxl,Xdiff,LF|
  // printf("PX,ORDER,Ts_ms,U_PWM,X1_Pxl,X2,X3,PX1,PX2,Xref,Xdiff,LF");
  printf("PXY,%u,%u,%d,%d,%d,%d,%d,%d,%d,%d",
             ORDER,
        (int)(SampleEpoch100us/10),
              xAxis->Ctrl_OutPWus,
        (int)(xAxis->KF_X[0]),
        (int)(xAxis->KF_X[1]),
        (int) (xAxis->KF_X[2]),
        (int)(xAxis->KF_PU[0][0]),
        (int)(xAxis->KF_PU[1][1]),
              xAxis->Meas_PosPxl,
              xAxis->Cal_SpeedPxlPerSec
        );
  // |PY,ORDER,Ts_ms,U_PWM,Y1_Pxl,Y2,Y3,PX1,PX2,Yref_Pxl,Ydiff,LF|
  printf(",%d,%d,%d,%d,%d,%d,%d,%d\n",
//              ORDER,
//        (int)(SampleEpoch100us/10),
              yAxis->Ctrl_OutPWus,
        (int)(yAxis->KF_X[0]),
        (int)(yAxis->KF_X[1]),
        (int) (yAxis->KF_X[2]),
        (int)(xAxis->KF_PU[0][0]),
        (int)(xAxis->KF_PU[1][1]),
              yAxis->Meas_PosPxl,
              yAxis->Cal_SpeedPxlPerSec
        );
}

int32_t  ascii2int( char *pintstring )
/**
*  ***********************************************************************
*  @brief  Convert integer value string in "%d" format to 32 bit integer
*
*          Wandelt Integerzahl-String analog "%d" in 32 Bit int-Wert um.
*  @param  *pintstring : string input buffer
*  @retval none
**************************************************************************
*/
{
  int8_t   negative = 0;          //< default assumed positive, no sign
  int32_t  intval   = 0;

  while ( *pintstring != 0 )      //< do until end of string
  {
    if ( *pintstring == '-' )  negative = 1;
    if ( *pintstring == '+' )  negative = 0;
    if ( (*pintstring >= '0' ) && (*pintstring <= '9') )
    {
      intval = intval*10 + (*pintstring - '0');
    }
    pintstring++;                 //< next char in input buffer
  }

  if ( negative )                 //< take 2's complement
  {
    intval = ~intval + 1;
  }
  return  intval;
}


void sendData2CAMPC (const struct SS_SysPar *X, const struct SS_SysPar *Y)
/** *********************************************************************
*  @brief  Send data to the Camera PC
*  @param X pointer to struct of type SS_SysPar for x-axis
*  @param Y pointer to struct of type SS_SysPar for y-axis
************************************************************************/
{
  sendFloat('a', X->Cal_SpeedPxlPerSec);  Delay_ms(1);    //!< 11 Byte = 1 identifier + 8 data bytes + 2 checksum bytes
  sendFloat('b', Y->Cal_SpeedPxlPerSec);  Delay_ms(1);    //!< 11 Byte
  sendFloat('c', X->Ctrl_OutAngleDeg);    Delay_ms(1);    //!< 11 Byte = 1 identifier + 8 data bytes + 2 checksum bytes
  sendFloat('d', Y->Ctrl_OutAngleDeg);    Delay_ms(1);    //!< 11 Byte
  sendShort('e', X->Meas_PosPxl);         Delay_ms(1);    //!< 7 Byte = 1 identifier + 4 data bytes + 2 checksum bytes
  sendShort('f', Y->Meas_PosPxl);         Delay_ms(1);    //!< 7 Byte
  sendShort('g', X->Ctrl_RefPosPxl);      Delay_ms(1);    //!< 7 Byte
  sendShort('h', Y->Ctrl_RefPosPxl);      Delay_ms(1);    //!< 7 Byte
  sendFloat('i', X->Ctrl_OutAngleDeg);    Delay_ms(1);    //!< 7 Byte
  sendFloat('j', Y->Ctrl_OutAngleDeg);    Delay_ms(1);    //!< 7 Byte
  sendShort('k', X->Ctrl_OutPWus);        Delay_ms(1);    //!< 7 Byte
  sendShort('l', Y->Ctrl_OutPWus);        Delay_ms(1);    //!< 7 Byte
  sendFloat('m', (float)X->KF_X[0]);      Delay_ms(1);    //!< 7 Byte PXL   
  sendFloat('n', (float)X->KF_X[1]);      Delay_ms(1);    //!< 7 Byte PXL/m
  sendFloat('o', (float)Y->KF_X[0]);      Delay_ms(1);    //!< 7 Byte PXL
  sendFloat('p', (float)Y->KF_X[1]);      Delay_ms(1);    //!< 7 Byte PXL/m
                                                          // ---------
                                                          // 130 Byte per processing cycle / image frame
  USART2SendChar(0x0A); //!< ASC_LF. is ignored by PC
  USART2SendChar(0x0D); //!< ASC_CR. for a neat display (not HYPERTERMINAL)
}
