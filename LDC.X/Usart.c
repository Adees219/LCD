#include "Usart.h"
void UART_RX_config(uint16_t baudrate){
    /*
    1. Initialize the SPBRGH, SPBRG register pair and
    the BRGH and BRG16 bits to achieve the
    desired baud rate (see Section 12.3 “EUSART
    Baud Rate Generator (BRG)”).*/
    
    TXSTAbits.BRGH = 0; // Low speed
    BAUDCTLbits.BRG16 = 0; //8 bits
    
    //para valores de FOSC = 8MHz (revisar tabla de la pag 162 datasheet)
    if(baudrate == 1200){
        SPBRG = 103;
        SPBRGH = 0;
    }
    else if(baudrate == 2400){
        SPBRG = 51;
        SPBRGH = 0;
    }
    else if(baudrate == 9600){
        SPBRG = 12;
        SPBRGH = 0;
    }

    
    /*
    2. Enable the serial port by setting the SPEN bit.
    The SYNC bit must be clear for asynchronous
    operation.*/
    
    RCSTAbits.SPEN = 1; //serial port habilitado (RX y TX)
    TXSTAbits.SYNC = 0; //modo asincrono
    
    /*
    3. If interrupts are desired, set the RCIE interrupt
    enable bit and set the GIE and PEIE bits of the
    INTCON register.
     */
     
    PIE1bits.RCIE = 1; //interrupcion EUSART
    PIR1bits.RCIF = 1; //bandera interrupcion EUSART
    /* 
    4. If 9-bit reception is desired, set the RX9 bit.
     */
    
    RCSTAbits.RX9 = 0; //recepcion de 8bits
    
    /*
    5. Enable reception by setting the CREN bit.
     */
    
    RCSTAbits.CREN = 1; //habilita el receptor
    
    /*
    6. The RCIF interrupt flag bit will be set when a
    character is transferred from the RSR to the
    receive buffer. An interrupt will be generated if
    the RCIE interrupt enable bit was also set.
    
    7. Read the RCSTA register to get the error flags
    and, if 9-bit data reception is enabled, the ninth
    data bit.
     
    8. Get the received 8 Least Significant data bits
    from the receive buffer by reading the RCREG
    register.
     
    9. If an overrun occurred, clear the OERR flag by
    clearing the CREN receiver enable bit
     */
    return;
}


void UART_TX_config(uint16_t baudrate){
    /*
    1. Initialize the SPBRGH, SPBRG register pair and
    the BRGH and BRG16 bits to achieve the desired
    baud rate (see Section 12.3 “EUSART Baud
    Rate Generator (BRG)”).
     */ 
    
        
    TXSTAbits.BRGH = 0; // Low speed
    BAUDCTLbits.BRG16 = 0; //8 bits
    
    //para valores de FOSC = 8MHz (revisar tabla de la pag 162 datasheet)
    if(baudrate == 1200){
        SPBRG = 103;
        SPBRGH = 0;
    }
    else if(baudrate == 2400){
        SPBRG = 51;
        SPBRGH = 0;
    }
    else if(baudrate == 9600){
        SPBRG = 12;
        SPBRGH = 0;
    }

    
    
     /* 
    2. Enable the asynchronous serial port by clearing
    the SYNC bit and setting the SPEN bit.
     */
    
        RCSTAbits.SPEN = 1; //serial port habilitado (RX y TX)
        TXSTAbits.SYNC = 0; //modo asincrono
        
    /*
    3. If 9-bit transmission is desired, set the TX9 control
    bit. A set ninth data bit will indicate that the 8
    Least Significant data bits are an address when
    the receiver is set for address detection.
     */
        
        TXSTAbits.TX9 = 0;// 8 bits de transmision
        
     /* 
    4. Enable the transmission by setting the TXEN
    control bit. This will cause the TXIF interrupt bit
    to be set.
      */
        
        TXSTAbits.TXEN = 1; //transmision habilitada
      
      /* 
    5. If interrupts are desired, set the TXIE interrupt
    enable bit. An interrupt will occur immediately
    provided that the GIE and PEIE bits of the
    INTCON register are also set.
       */
        
       // PIE1bits.TXIE = 1;
        PIR1bits.TXIF = 0;
        
       /* 
    6. If 9-bit transmission is selected, the ninth bit
    should be loaded into the TX9D data bit.
    7. Load 8-bit data into the TXREG register. This
    will start the transmission.
     */
        return;
}

void UART_write_string(char *a){
    int i;
    
    for (i = 0; a[i] != '\0'; i++){
        while(!PIR1bits.TXIF);
        UART_write_char(a[i]);
    }
    
    return;
}

void UART_write_char(char c){ //PIC a pc
        TXREG = c; // envia caracter por caracter a la terminal
   
    return;
}

char UART_read_char(){ // de PC a pic
    
   char datoRecibido = RCREG;
   return datoRecibido;
}