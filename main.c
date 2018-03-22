/*This work is licensed under a
Creative Commons Attribution-NonCommmercial-ShareAlike 4.0 International

This license lets others remix, tweak, and build upon this work non-commercially,
as long as they credit the AUTHOR and license their new creations under the identical terms.
*/

/*
David Josue Barrientos Rojas (2017)
Simple Code for a Remote Controlled Sumobot
d[dot]b[dot]gt[at]ieee[dot]org
Universidad de San Carlos de Guatemala
EE School
*/



/*************************************Librerias**************************************/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pwm.h"
/************************************Definiciones********************************************/
#define RIGHT 0x08
#define LEFT 0x02
#define BACKWARD 0x05
#define FORWARD 0x0A
#define STOP 0x00
/*************************************Variables************************************************/
//*************UART*************************//
volatile char command;
/************************************UART*********************************************************/
void UARTIntHandler(void) {
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART1_BASE, true);
	UARTIntClear(UART1_BASE, ui32Status);
	while(UARTCharsAvail(UART1_BASE)){
		command = UARTCharGetNonBlocking(UART1_BASE);
		UARTCharPut(UART1_BASE, command);
		SysCtlDelay(SysCtlClockGet()/(1000 * 3));
		}
}
/************************************Main*********************************************************/
int main(void) {
/************************************Reloj********************************************************/
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_XTAL_16MHZ);
/************************************UART0*********************************************************/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIOPinConfigure(GPIO_PB0_U1RX);
	GPIOPinConfigure(GPIO_PB1_U1TX);
	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 9600,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
/************************************GPIOS*********************************************************/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
/*********************************INTERRUPCIONES*****************************************************/
	IntMasterEnable();
	IntEnable(INT_UART1);
	UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
/*******************************CICLO INFINITO******************************************************/
	while (1){
		switch(command){
		    case 'w':
		    	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, FORWARD);
		    	break;
		    case 'x'  :
		    	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, BACKWARD);
		    	break;
		    case 'd'  :
		    	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, RIGHT);
		    	break;
		    case 'a'  :
		    	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, LEFT);
		    	break;
		    case 's'  :
		    	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, STOP);
		   	    break;
		}
    	SysCtlDelay(133333);
	}
}
