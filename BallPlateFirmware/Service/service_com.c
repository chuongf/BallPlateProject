
#include  <service_com.h>          // STM32F10x peripherals definitions
#include  <string.h>               // for memcpy


uint8_t  checkTransfer(uint8_t* pReceive)
/** *********************************************************************
*  @brief     Check for error free reception of data.
*  @param[in] pReceive High and Low byte of the X/Y-position, the X/Y reference
*             position and the checksums
*  @retval    0: checksum ERROR  1: checksum OK. 
*  
************************************************************************/
{
  uint16_t X, Y, RefX, RefY, CheckSum, temp, My_CS;

  X = pReceive[1] << 8;
  X = X + pReceive[2];
  Y = pReceive[3] << 8;
  Y = Y + pReceive[4];
  RefX = pReceive[5] << 8;
  RefX = RefX + pReceive[6];
  RefY = pReceive[7] << 8;
  RefY = RefY + pReceive[8];

  temp = pReceive[9] << 8;
  temp = temp + pReceive[10];
  CheckSum = temp;

  My_CS = X+Y+RefX+RefY;
  My_CS = ~My_CS;

  if( My_CS == CheckSum && X!=0 && Y!=0 )
    return 1; // checksums are valid and ball was detected => NO ERROR

  return 0; // checksums are invalid or ball was not detected => ERROR
}


 void sendFloat (uint8_t msgID, float floatWORD)
 /** *********************************************************************
 *  @brief  Sends float data via auxSIO to PC.
 *
 *  Splits a FLOAT value in 6 bytes and generates a checksum
 *  Furthermore splits every byte in two upper and lower 4 bits and sends
 *  them over the COM interface to the PC.
 *
 *  This is necessary to ensure that identifiers and data can be distinguished.
 *  Data are in the range of 0x30 to 0x39 and 0x41 to 0x46, every other value is
 *  reserved for identifiers.
 *
 *  @param[in] msgID     : ID for float data msg
 *  @param[in] floatWORD : Data to convert & xmit
 ************************************************************************/
 {
   uint8_t wbufr[5];
   uint8_t CheckSum;
   uint8_t loByte, hiByte;
   uint8_t i;

   memcpy( wbufr, &floatWORD, sizeof(floatWORD) );
 	
   CheckSum = ~( wbufr[0]+wbufr[1]+wbufr[2]+wbufr[3] );
   //CheckSum = ~CheckSum;
   wbufr[4] = CheckSum;

   USART2SendChar(msgID);
 	
   for (i=0; i<5; i++)  // convert & xmit each binary byte as 2 ascii hex bytes
   {
 //  loByte =  wbufr[i] % 16;          // split into high/low 4 bits
 //  hiByte = (wbufr[i] / 16) % 16;
 		
     loByte =  wbufr[i] & 0x0F;        // split up into high/low nibble
     hiByte = (wbufr[i] >> 4) & 0x0F;

     if( hiByte < 10)
       hiByte = 0x30 + hiByte;
     else
       hiByte = 0x37 + hiByte;
     if( loByte < 10)
       loByte = 0x30 + loByte;
     else
       loByte = 0x37 + loByte;

     USART2SendChar(hiByte);
     USART2SendChar(loByte);
   }
 } // end sendFloat( )


 void sendShort(uint8_t msgID, short Word16Bit)
 /** *********************************************************************
 *  @brief  Sends variables of type short over the RS232 interface.
 *
 *  Splits a short value in 4 bytes and generates a checksum
 *  Furthermore splits every byte in two upper and lower 4 bits and sends
 *  them over the COM interface to the PC.
 *
 *  This is necessary to ensure that identifiers and data can be distinguished.
 *  Data are in the range of 0x30 to 0x39 and 0x41 to 0x46, every other value is
 *  reserved for identifiers
 *
 *  @param[in] msgID     : ID of 16-Bit word data msg
 *  @param[in] Word16Bit : 16-Bit word to convert & send
 ************************************************************************/
 {
   uint8_t wbufr[3];
   uint8_t CheckSum;
   uint8_t loByte, hiByte;
   uint8_t i;

   memcpy( wbufr, &Word16Bit, sizeof(Word16Bit) );
   CheckSum = ~( wbufr[0] + wbufr[1] );
   //CheckSum = ~CheckSum;
   wbufr[2] = CheckSum;

   USART2SendChar(msgID);

   for (i=0; i<3 ;i++)  // convert & xmit each binary byte as 2 ascii hex bytes
   {
 //  loByte = wbufr[i] % 16;         // split in high/low 4 bits
 //  hiByte = (wbufr[i]/16)%16;

     loByte =  wbufr[i] & 0xF;       // split in high/low 4 bits
     hiByte = (wbufr[i] >> 4) & 0xF;

     if(hiByte < 10)
     {
       hiByte = 0x30 + hiByte;
     } else {
       hiByte = 0x37 + hiByte;
     }
 		
     if(loByte < 10)
 		{
       loByte = 0x30 + loByte;
     } else{
       loByte = 0x37 + loByte;
     }

     USART2SendChar(hiByte);
     USART2SendChar(loByte);
   }
 } // end  sendShort( )


