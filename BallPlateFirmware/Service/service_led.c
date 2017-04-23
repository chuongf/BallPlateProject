


#include  <service_led.h>          // STM32F10x peripherals definitions


void LED_On(uint16_t LED)
{
	switch (LED) {
		case 10: GPIO_SetBits(LED_PORT, LED_10); break;
		case 11: GPIO_SetBits(LED_PORT, LED_11); break;
		case 12: GPIO_SetBits(LED_PORT, LED_12); break;
		case 13: GPIO_SetBits(LED_PORT, LED_13); break;
		case 14: GPIO_SetBits(LED_PORT, LED_14); break;
		case 15: GPIO_SetBits(LED_PORT, LED_15); break;
		default: break;	
	}		
}

void LED_Off(uint16_t LED)
{
	switch (LED) {
		case 10: GPIO_ResetBits(LED_PORT, LED_10); break;
		case 11: GPIO_ResetBits(LED_PORT, LED_11); break;
		case 12: GPIO_ResetBits(LED_PORT, LED_12); break;
		case 13: GPIO_ResetBits(LED_PORT, LED_13); break;
		case 14: GPIO_ResetBits(LED_PORT, LED_14); break;
		case 15: GPIO_ResetBits(LED_PORT, LED_15); break;
		default: break;	
	}		
}

void LED_Toggle(uint16_t LED)
{
	switch (LED) {
		case 10: GPIO_ToggleBits(LED_PORT, LED_10); break;
		case 11: GPIO_ToggleBits(LED_PORT, LED_11); break;
		case 12: GPIO_ToggleBits(LED_PORT, LED_12); break;
		case 13: GPIO_ToggleBits(LED_PORT, LED_13); break;
		case 14: GPIO_ToggleBits(LED_PORT, LED_14); break;
		case 15: GPIO_ToggleBits(LED_PORT, LED_15); break;
		default: break;	
	}		
}
