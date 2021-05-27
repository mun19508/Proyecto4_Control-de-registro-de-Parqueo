/**********************************************************************
 *Autor: Daniel Mundo                                                 *
 *Carnet:19508                                                        *
 *Nombre del archivo: main.c                                          *
 *Proyecto 4: CONTROL DE REGISTRO PARA PARQUEO                        *
 *Control de Botones y 7 segmentos                                    *
 **********************************************************************
 *                              Librerias                             *
 **********************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include "driverlib/pin_map.h"
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include "driverlib/uart.h"
/**********************************************************************
 *                      Prototipo de funciones                        *
 **********************************************************************/

/**********************************************************************
 *                              Variables                             *
 **********************************************************************/
/*La tabla esta conf. para un 7 segmentos de catodo común
 *Estados de los pines del puerto B
 *                                 gfedcab*/
static uint8_t tabla7Seg[] = { 0b00111111, //0
        0b00000101, //1
        0b01011011, //2
        0b01001111, //3
        0b01100101, }; //4
/*Cantidad total de parqueos*/
uint8_t ContParqueos = 4;
/*Sirven para llevar control de los parqueos disponibles*/
uint8_t DispParqueos[] = { 1, 1, 1, 1 };
uint8_t LEDParqueos[] = { 128, 8, 16, 2 };
/**********************************************************************/
int main(void)
{
    /*Config. del reloj para freq. de 40MHz*/
    SysCtlClockSet(
    SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    /**********************************************************************/
    /*Habilita puerto A, B, D, E*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    /**********************************************************************/
    /*Habilita el UART5*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    /**********************************************************************/
    /*Se desbloquea PD7 */
    HWREG(GPIO_PORTD_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE+GPIO_O_CR) |= GPIO_PIN_7;
    /**********************************************************************/
    /*Se config. los pines del 7 segmentos*/
    GPIOPinTypeGPIOOutput(
            GPIO_PORTB_BASE,
            GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2
                    | GPIO_PIN_1 | GPIO_PIN_0);
    /**********************************************************************/
    /*Se config. los pines de los Leds*/
    GPIOPinTypeGPIOOutput( GPIO_PORTA_BASE,
    GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4);
    GPIOPinTypeGPIOOutput( GPIO_PORTE_BASE,
    GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0);
    /**********************************************************************/
    /*Se config. el UART5 y sus pines*/
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    UARTConfigSetExpClk(
            UART5_BASE, SysCtlClockGet(), 115200,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTFIFOLevelSet(UART5_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
    UARTEnable(UART5_BASE);
    /**********************************************************************/
    /*Se config. los pines de los deep switch*/
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_2);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_2,
    GPIO_STRENGTH_8MA,
                     GPIO_PIN_TYPE_STD_WPU); //con pull-up
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7 | GPIO_PIN_6);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_7 | GPIO_PIN_6,
    GPIO_STRENGTH_8MA,
                     GPIO_PIN_TYPE_STD_WPU); //con pull-up
    /**********************************************************************/
    while (1)
    {
        /**********************************************************************/
        /*Muestra de parqueos disponibles, 7 segmentos*/
        /**********************************************************************/
        GPIOPinWrite(
                GPIO_PORTB_BASE,
                GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2
                        | GPIO_PIN_1 | GPIO_PIN_0,
                tabla7Seg[ContParqueos]);
        /**********************************************************************/
        /*Muestra el estado actual del parqueo Leds*/
        /**********************************************************************/
        /*----------------------------Parqueo 1-------------------------------*/
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7 | GPIO_PIN_6, LEDParqueos[0]);
        /*--------------------------------------------------------------------*/
        /*----------------------------Parqueo 2-------------------------------*/
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3 | GPIO_PIN_2, LEDParqueos[1]);
        /*--------------------------------------------------------------------*/
        /*----------------------------Parqueo 3-------------------------------*/
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4, LEDParqueos[2]);
        /*--------------------------------------------------------------------*/
        /*----------------------------Parqueo 4-------------------------------*/
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_0, LEDParqueos[3]);
        /*--------------------------------------------------------------------*/
        /**********************************************************************/
        /*Obtencion de que parqueos siguen disponibles*/
        /**********************************************************************/
        /*----------------------------Parqueo 1-------------------------------*/
        if ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2) == 0)
                && (DispParqueos[0] == 1))
        {
            ContParqueos--;
            DispParqueos[0] = 0;
            LEDParqueos[0] = 64;
        }
        if ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2) == 4)
                && (DispParqueos[0] == 0))
        {
            ContParqueos++;
            DispParqueos[0] = 1;
            LEDParqueos[0] = 128;
        }
        /*--------------------------------------------------------------------*/
        /*----------------------------Parqueo 2-------------------------------*/
        if ((GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7) == 0)
                && (DispParqueos[1] == 1))
        {
            ContParqueos--;
            DispParqueos[1] = 0;
            LEDParqueos[1] = 4;
        }
        if ((GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_7) == 128)
                && (DispParqueos[1] == 0))
        {
            ContParqueos++;
            DispParqueos[1] = 1;
            LEDParqueos[1] = 8;
        }
        /*--------------------------------------------------------------------*/
        /*----------------------------Parqueo 3-------------------------------*/
        if ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3) == 0)
                && (DispParqueos[2] == 1))
        {
            ContParqueos--;
            DispParqueos[2] = 0;
            LEDParqueos[2] = 32;
        }
        if ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3) == 8)
                && (DispParqueos[2] == 0))
        {
            ContParqueos++;
            DispParqueos[2] = 1;
            LEDParqueos[2] = 16;
        }
        /*--------------------------------------------------------------------*/
        /*----------------------------Parqueo 4-------------------------------*/
        if ((GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6) == 0)
                && (DispParqueos[3] == 1))
        {
            ContParqueos--;
            DispParqueos[3] = 0;
            LEDParqueos[3] = 1;
        }
        if ((GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6) == 64)
                && (DispParqueos[3] == 0))
        {
            ContParqueos++;
            DispParqueos[3] = 1;
            LEDParqueos[3] = 2;
        }
        /*--------------------------------------------------------------------*/
        /**********************************************************************/
        /*Envio de datos via UART5*/
        /**********************************************************************/
        /* Se suma el total de parkeos disponibles*/
        uint8_t TotalParkDisp = DispParqueos[0] + DispParqueos[1]
                + DispParqueos[2] + DispParqueos[3];
        /*--------------------------------------------------------------------*/
        /*Se crea un arreglo de chars para enviar los datos via UART*/
        char data_send[] = { DispParqueos[0] + 48, DispParqueos[1] + 48,
                             DispParqueos[2] + 48, DispParqueos[3] + 48,
                             TotalParkDisp + 48, '\n' };
        /*--------------------------------------------------------------------*/
        /*Rutina que manda los valores del arreglo chars, separando los datos por comas y terminando con un fin de linea*/
        uint8_t i;
        for (i = 0; i < 255 && data_send[i] != '\n'; i++)
        {
            UARTCharPut(UART5_BASE, data_send[i]);
            UARTCharPut(UART5_BASE, ',');
        }
        UARTCharPut(UART5_BASE, '\n');
        /*--------------------------------------------------------------------*/
    }
}
