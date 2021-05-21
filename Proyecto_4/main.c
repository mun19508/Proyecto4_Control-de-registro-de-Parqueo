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
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
/***************************************************
 *           Prototipo de funciones                *
 ***************************************************/

/***************************************************
 *                  Variables                      *
 ***************************************************/
/*La tabla esta conf. para un 7 segmentos de catodo común
 *Estados de los pines del puerto B
 *                                 gfedcab*/
static uint8_t tabla7Seg[] = { 0b00111111, //0
        0b00000101, //1
        0b01011011, //2
        0b01001111, //3
        0b01100101, }; //4
uint8_t ContParqueos = 4;
uint8_t DispParqueos[] = { 1, 1, 1, 1 };
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
    /*Habilita puerto A, B & D*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    HWREG(GPIO_PORTD_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE+GPIO_O_CR) |= GPIO_PIN_7;
    /*Se config. los pines del 7 segmentos*/
    GPIOPinTypeGPIOOutput(
            GPIO_PORTB_BASE,
            GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2
                    | GPIO_PIN_1 | GPIO_PIN_0);
    /*Se config. los pines de los deep switch*/
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_2);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_2,
    GPIO_STRENGTH_8MA,
                     GPIO_PIN_TYPE_STD_WPU); //con pull-up

    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7 | GPIO_PIN_6);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_7 | GPIO_PIN_6,
    GPIO_STRENGTH_8MA,
                     GPIO_PIN_TYPE_STD_WPU); //con pull-up
    /*************************************************************/
    while (1)
    {
        /*Muestra de parqueos disponibles, 7 segmentos*/
        GPIOPinWrite(
                GPIO_PORTB_BASE,
                GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2
                        | GPIO_PIN_1 | GPIO_PIN_0,
                tabla7Seg[ContParqueos]);
        /*Obtencion de que parqueos siguen disponibles*/
        /*****************Parqueo 1*******************/
        if ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2) == 0)
                && (DispParqueos[0] == 1))
        {
            ContParqueos--;
            DispParqueos[0] = 0;
        }
        if ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2) == 4)
                && (DispParqueos[0] == 0))
        {
            ContParqueos++;
            DispParqueos[0] = 1;
        }
        /**********************************************/
        /*****************Parqueo 2*******************/
        if ((GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7) == 0)
                && (DispParqueos[1] == 1))
        {
            ContParqueos--;
            DispParqueos[1] = 0;
        }
        if ((GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7) == 128)
                && (DispParqueos[1] == 0))
        {
            ContParqueos++;
            DispParqueos[1] = 1;
        }
        /**********************************************/
        /*****************Parqueo 3*******************/
        if ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3) == 0)
                && (DispParqueos[2] == 1))
        {
            ContParqueos--;
            DispParqueos[2] = 0;
        }
        if ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3) == 8)
                && (DispParqueos[2] == 0))
        {
            ContParqueos++;
            DispParqueos[2] = 1;
        }
        /**********************************************/
        /*****************Parqueo 4*******************/
        if ((GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6) == 0)
                && (DispParqueos[3] == 1))
        {
            ContParqueos--;
            DispParqueos[3] = 0;
        }
        if ((GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6) == 64)
                && (DispParqueos[3] == 0))
        {
            ContParqueos++;
            DispParqueos[3] = 1;
        }
        /**********************************************/
    }
}
