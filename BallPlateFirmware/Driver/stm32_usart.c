
#include  "stm32_usart.h"          // STM32F10x peripherals definitions
#include  "UserDef.h"

extern uint32_t StatusFlags;

extern char Rx1Buf[32], Rx1GUIFrame[32] ;
extern char Rx2Buf[12], Rx2PosFrame[12] ;	


/* buffer for USART1 */
static struct buf_st tbuf = { 0, 0, };
#define SIO_TBUFLEN ((unsigned short)(tbuf.in - tbuf.out))
static unsigned int tx_restart = 1;               // NZ if TX restart is required


/* buffer for USART2 */
static struct buf_st tbuf2 = { 0, 0, };
#define SIO2_TBUFLEN ((unsigned short)(tbuf2.in - tbuf2.out))
static unsigned int tx2_restart = 1;               // NZ if TX restart is required


/*----------------------------------------------------------------------------
  USART1_IRQHandler
  Handles USART1 global interrupt request.
 *----------------------------------------------------------------------------*/
void USART1_IRQHandler (void) {
  volatile unsigned int IIR;
  struct buf_st *p;
	char rx1dump, i;
	static unsigned int rx1data_cnt =0;
  
    IIR = USART1->SR;
    if (IIR & USART_FLAG_RXNE) {                  // receive interrupt
      USART1->SR &= ~USART_FLAG_RXNE;	          // clear interrupt

      // read ASCII char from uart1, detect CR character and return a frame got.
      rx1dump = USART1->DR;
      if(rx1data_cnt == 0)
      {
        if((rx1dump == 0x0D) || (rx1dump == 0x0A))
        {
          // ignore the fist data if it is LF or CR
          return;
        }
        else // data comes
        {
          Rx1Buf[rx1data_cnt] = rx1dump ;
					rx1data_cnt++; 
        }
          
      }
      else    // rx1data_cnt != 0
      {
        if((rx1dump == 0x0D) || (rx1dump == 0x0A))
        {
          StatusFlags |= GUI_CMD_AVAILABLE;            // flag shows a new frame just come, need to be cleared in main
          // save data from buffer
					for (i = 0; i<rx1data_cnt; i++)
          {
            Rx1GUIFrame[i] = 	Rx1Buf[i];
          }
          Rx1GUIFrame[rx1data_cnt] = 0; // mark the end of frame
          rx1data_cnt = 0;
          return;
        }
        Rx1Buf[rx1data_cnt] = rx1dump ;
        rx1data_cnt++;
      }
    }

    if (IIR & USART_FLAG_TXE) {                 // send interrupt
      USART1->SR &= ~USART_FLAG_TXE;	          // clear interrupt

      p = &tbuf;

      if (p->in != p->out) {
        USART1->DR = (p->buf [p->out & (TBUF_SIZE-1)] & 0x1FF);
        p->out++;
        tx_restart = 0;
      }
      else {
        tx_restart = 1;
		USART1->CR1 &= ~USART_FLAG_TXE;		      // disable TX interrupt if nothing to send

      }
    }
}


/*------------------------------------------------------------------------------
  buffer_Init
  initialize the buffers
 *------------------------------------------------------------------------------*/
void buffer_Init (void) {

  tbuf.in = 0;                                    // Clear com buffer indexes
  tbuf.out = 0;
  tx_restart = 1;
  
  tbuf2.in = 0;                                    // Clear com buffer indexes
  tbuf2.out = 0;
  tx2_restart = 1;
}

/*------------------------------------------------------------------------------
  SenChar
  transmit a character
 *------------------------------------------------------------------------------*/
int SendChar (int c) {
  struct buf_st *p = &tbuf;

                                                  // If the buffer is full, return an error value
  if (SIO_TBUFLEN >= TBUF_SIZE)
    return (-1);
                                                  
  p->buf [p->in & (TBUF_SIZE - 1)] = c;           // Add data to the transmit buffer.
  p->in++;

  if (tx_restart) {                               // If transmit interrupt is disabled, enable it
    tx_restart = 0;
	USART1->CR1 |= USART_FLAG_TXE;		          // enable TX interrupt
  }

  return (0);
}

/*------------------------------------------------------------------------------
  SenChar
  transmit a character
 *------------------------------------------------------------------------------*/
int GetKey (void) {
  return (0);
}




/*----------------------------------------------------------------------------
  USART2_IRQHandler
  Handles USART1 global interrupt request.
 *----------------------------------------------------------------------------*/
void USART2_IRQHandler (void) {
  volatile unsigned int IIR2;
  struct buf_st *p;
	char rxdump, i;
	static unsigned int rx2data_cnt =0;

    IIR2 = USART2->SR;
    if (IIR2 & USART_FLAG_RXNE) {                  // receive interrupt
      USART2->SR &= ~USART_FLAG_RXNE;	          // clear interrupt
			rxdump = (USART2->DR & 0x1FF);
			
			if(rx2data_cnt == 0){
				if(rxdump != 0x0D){
					//frame wrong: ignore this data
					return;
				}
				else {
					// correct begining of the frame
					Rx2Buf[rx2data_cnt] = rxdump ;
					rx2data_cnt++;
				}
			}
			else{ // second byte or higher
				Rx2Buf[rx2data_cnt] = rxdump ;
				rx2data_cnt++;
				if(rx2data_cnt >10){
					rx2data_cnt = 0;
					StatusFlags |= FB_POS_AVAILABLE;		// signal that received completed
					// save data from buffer
					for (i = 0; i<11; i++){
					Rx2PosFrame[i] = 	Rx2Buf[i];
					}
				}
			}
		}

				

    if (IIR2 & USART_FLAG_TXE) {							// write interrupt
      USART2->SR &= ~USART_FLAG_TXE;	          // clear interrupt

      p = &tbuf2;

      if (p->in != p->out) {
        USART2->DR = (p->buf [p->out & (TBUF_SIZE-1)] & 0x1FF);
        p->out++;
        tx2_restart = 0;
      }
      else {
        tx2_restart = 1;
		USART2->CR1 &= ~USART_FLAG_TXE;		      // disable TX interrupt if nothing to send

      }
    }
}


/*------------------------------------------------------------------------------
  SenChar
  transmit a character
 *------------------------------------------------------------------------------*/
int USART2SendChar (int c) {
  struct buf_st *p = &tbuf2;

                                                  // If the buffer is full, return an error value
  if (SIO2_TBUFLEN >= TBUF_SIZE)
    return (-1);
                                                  
  p->buf [p->in & (TBUF_SIZE - 1)] = c;           // Add data to the transmit buffer.
  p->in++;

  if (tx2_restart) {                               // If transmit interrupt is disabled, enable it
    tx2_restart = 0;
	USART2->CR1 |= USART_FLAG_TXE;		          // enable TX interrupt
  }

  return (0);
}

///*------------------------------------------------------------------------------
//  GetKey
//  receive a character
// *------------------------------------------------------------------------------*/
//int USART2GetKey (void) {
//  struct buf_st *p = &rbuf2;
//  char temp;

//  if (SIO2_RBUFLEN == 0)
//    return (-1);

//  temp = (p->buf [(p->out++) & (RBUF_SIZE - 1)]);
//  return temp;
//}


