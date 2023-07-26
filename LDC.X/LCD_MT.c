/* 
 * File:   LCD_Mt.c
 * Author: USUARIO FINAL
 *
 * Created on 20 de julio de 2023, 10:12 AM
 */

//**********************************************************************************************
//palabra de configuracion
//**********************************************************************************************

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//**********************************************************************************************
//prototipos de funciones
//**********************************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config.h"
#include "ADC.h"
#include "LCD.h"
#include "Usart.h"
//**********************************************************************************************
//prototipo de funciones
//**********************************************************************************************
void setup(void);
void voltaje(void);
void contador(void);
void MENU(void);
//**********************************************************************************************
//variables
//**********************************************************************************************

#define _XTAL_FREQ 8000000

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RB5
#define D6 RD6
#define D7 RD7

char strADC[4]; 
int unidad;
int decena;
int centena;

char UART_get;
char menu[] = "\n Menu: \n 1)Leer voltaje \n 2)enviar Ascii \r";

//**********************************************************************************************
//ISR
//**********************************************************************************************
void __interrupt() isr(void)
{
    if(ADIF){
        int valorADC = adc_read();
        int valorMapeado = map(valorADC, 0,255,0,500);
        int residuo;
        
        centena = (uint8_t) (valorMapeado/100);
        residuo = valorMapeado%100;

        decena = (uint8_t) residuo/10;
        residuo %= 10;

        unidad = residuo/1;
        ADIF = 0;
    }
    
    if(RCIF){
         UART_get = UART_read_char();
         RCIF = 0;
    }

}


//**********************************************************************************************
//Codigo principal
//**********************************************************************************************
void main(void) {
    setup();
    Lcd_Init();
    
    while(1){
        
        
        
         if(ADCON0bits.GO == 0){
            __delay_ms(10);
            ADCON0bits.GO = 1; 
        }
         
         if(UART_get == 'm'){
           MENU();
         }
         
         Lcd_Clear();    
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Voltaje");
        Lcd_Set_Cursor(2,1);
        sprintf(strADC,"%d,%d%d V",centena,decena,unidad);
        Lcd_Write_String(strADC);
        
        
        

        __delay_ms(500);
    }
}


//**********************************************************************************************
//Codigo de configuracion
//**********************************************************************************************
void setup(){
    
    configOsc(8); //configuracion del oscilador
    configPort(); //configuracion de puertos  
    configIntbits(); //configuracion de bits de interrupcion    
    adc_init(0,8); //configuracion del adc
    UART_RX_config(9600); 
    UART_TX_config(9600);
    
    
    return;
}

/*******************************************************************/
//Funciones locales
/*******************************************************************/
void MENU(void){
    UART_write_string(menu);
    UART_get = '\0';
    while(UART_get == '\0'); //espera a que la persona ingrese una opcion

            switch(UART_get){
        case '1':   //valor potenciometro
            voltaje();
            break;

        case '2':   //ascii
            contador();
            break;

        default:    //opcion no existente
            UART_write_string("No existe esa opcion, presione enter para continuar \r");
            while(RCREG != 13); //espera el enter
         
    } 
    return;
}

void voltaje(){
    UART_write_string(strADC);
    UART_write_char('\n');
    return;
}
void contador(){
    UART_write_string("Ingrese su caracter: ");
    UART_get = '\0';
    while(UART_get == '\0'); //esperar a que presione   
    UART_write_char(UART_get); // muestra el caracter enviado
    UART_write_char('\n');
    if(UART_get == '+'){
         PORTB++;      
    } 
    else if(UART_get == '-'){
        PORTB--;
    }       
    return;
}