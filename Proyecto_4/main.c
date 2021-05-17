/****************************************************
 *Autor: Daniel Mundo                               *
 *Carnet:19508                                      *
 *Nombre del archivo: main.c                        *
 *Proyecto 4: CONTROL DE REGISTRO PARA PARQUEO      *
 *Control de Botones y 7 segmentos                  *
 ****************************************************
 *              Librerias                           *
 ****************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
/***************************************************
 *           Prototipo de funciones                *
 ***************************************************/


/***************************************************
 *                  Variables                      *
 ***************************************************/


/***************************************************/
int main(void)
{
     /*Config. del reloj para freq. de 40MHz*/
     SysCtlClockSet(
     SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
     LoadTime = SysCtlClockGet();
     /*Habilita puerto F*/
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
     /*Habilita Timer 0*/
     SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
     /*Se config. los pines de los leds como salidas*/
     GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
     GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1);
}
