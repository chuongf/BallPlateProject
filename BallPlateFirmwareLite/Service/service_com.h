#include  "stm32_usart.h"          // STM32F10x Peripherals Definitions
#include  "platform_cfg.h"       // board specific (re-)definitions


/************************************************************************\
*		Mid-level formatted console IO
\************************************************************************/

uint8_t  checkTransfer(uint8_t* pReceive);

 extern void sendFloat (uint8_t msgID, float floatWORD);
 extern void sendShort(uint8_t msgID, int16_t Word16Bit);


