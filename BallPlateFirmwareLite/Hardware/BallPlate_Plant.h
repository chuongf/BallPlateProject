/**
*  ***********************************************************************
*  @file    BallPlate_Plant.h: Parameters related to Ball Plate
**************************************************************************/

/************************************************************************\
*    Physical Parameters
\************************************************************************/

#define   DEG2RAD                0.0174533  // pi/180


/// conversion factor (1 meter == 1385 pxl)  ??? FOR WHICH CAMERA vgl. ZiSo ???
#define   PIXEL_PER_METER        (float) 1385.0


#define   PLATE_XMIN_PXL          80      // visible ballplate image X coordinates 
#define   PLATE_XMAX_PXL         680

#define   PLATE_YMIN_PXL          30      // visible ballplate image Y coordinates 
#define   PLATE_YMAX_PXL         570


// PWM/Timer related constants
/// pulse width for ballplate in horizontal position in [us] units (1500 = 1.5ms)
#define   CENTER_PULSE_WIDTH     1500
#define   MIN_PULSE_WIDTH        1000
#define   MAX_PULSE_WIDTH        2000

/// servo actuator angle offsets for calibrating horizontal position of ballplate
#define   PWM_SERVO_X_OFFSET      -50  // in PWM [us] units -50 seems OK
#define   PWM_SERVO_Y_OFFSET        0  // in PWM [us] units   0 ? -100 to far EAST


/// default pulse period of PWM (22000 = 22ms, 10000 = 10ms)
//#define DEFAULT_PULSE_PERIOD   22000
#define   DEFAULT_PULSE_PERIOD   10000
/// default pulse width of PWM
#define   DEFAULT_PULSE_WIDTH    0.0

// mechanical dimensions
#define   MAX_SERVO_ANGLE_DEG    50
/// maximum possible servo angle in radians ( -50 deg <= servo angle <= 50 deg )
#define   MAX_SERVO_ANGLE_RAD    DEG2RAD*MAX_SERVO_ANGLE_DEG
/// length of servo arm in [m]
#define   SERVO_ARM_LENGTH       0.03 
/// distance between plate axis and servo arm attachment point in [m]
#define   HALF_BOARD_LENGTH      0.2  


// Slope to scale from angle to duty
//(MAX_PULSE_WIDTH - CENTER_PULSE_WIDTH)/MAX_SERVO_ANGLE_RAD
#define   SLOPE_RAD    572.9578
//(MAX_PULSE_WIDTH - CENTER_PULSE_WIDTH)/MAX_SERVO_ANGLE_DEG
#define   SLOPE_DEG    10 

// Jerk limit profile
#define MAX_JERK 1000 



/************************************************************************\
*    END OF MODULE BallPlate.h
\************************************************************************/
