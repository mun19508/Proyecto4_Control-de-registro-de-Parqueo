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
/*La tabla esta conf. para un 7 segmentos de catodo com�n
 *Estados de los pines del puerto B
 *                                 gfedcab*/
static uint8_t tabla7Seg[] = {  0b00111111, //0
                                0b00000101, //1
                                0b01011011, //2
                                0b01001111, //3
                                0b01100101};//4
 uint32_t LoadTime;
 /*Contador de prueba*/
 uint8_t CountTime = 0;
/***************************************************/
int main(void)
{
    /*Config. del reloj para freq. de 40MHz*/
    SysCtlClockSet(
    SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    LoadTime = SysCtlClockGet();
    /*Habilita puerto F*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    /*Habilita Timer 0*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    /*Se config. los pines del 7 segmentos*/
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,
            GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2
                    | GPIO_PIN_1 | GPIO_PIN_0);
    while (1)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0, tabla7Seg[CountTime]);
        SysCtlDelay(20000000); //delay aprox. 1.5s
        SysCtlDelay(20000000); //delay aprox. 1.5s
        CountTime++;
        if(CountTime == 5) CountTime = 0;
    }
}
