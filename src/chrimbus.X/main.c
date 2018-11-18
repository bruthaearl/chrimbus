/*
 * File:   newmain.c
 * Author: jake
 *
 * Created on July 29, 2018, 12:16 PM
 */

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT1 // Power-up default value for COSC bits (HFINTOSC with OSCFRQ= 32 MHz and CDIV = 1:1)
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (FSCM timer enabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR pin is Master Clear function)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = OFF      // Brown-out reset enable bits (Brown-out reset disabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = OFF     // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)

// CONFIG3
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled, SWDTEN is ignored)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4
#pragma config BBSIZE = BB512   // Boot Block Size Selection bits (512 words boot block size)
#pragma config BBEN = OFF       // Boot Block Enable bit (Boot Block disabled)
#pragma config SAFEN = OFF      // SAF Enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block Write Protection bit (Application Block not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration Register not write protected)
#pragma config WRTSAF = OFF     // Storage Area Flash Write Protection bit (SAF not write protected)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (High Voltage on MCLR/Vpp must be used for programming)

// CONFIG5
#pragma config CP = OFF         // UserNVM Program memory code protection bit (UserNVM code protection disabled)

#include <xc.h>
#include <stdint.h>

#define SCK_tris TRISCbits.TRISC0
#define SDA_tris TRISCbits.TRISC1

#define LED_DRIVER_ADDRESS 0b11000000

#define _XTAL_FREQ 1000000

void sys_initialize(void);
void i2c_initialize(void);
void i2c_write(uint8_t address, uint8_t control_word, uint8_t data);

_Bool I2C_Transmit(uint8_t data);
_Bool I2C_Startup(void);
_Bool I2C_Stop(void);

uint8_t data;
uint8_t driver_address;
uint8_t control_word;

enum{SUCCESS,FAILURE};

typedef enum led_registers{
    LED0_r = 02, LED1_r, LED2_r, LED3_r, LED4_r, LED5_r, LED6_r, LED7_r, LED8_r, 
            LED9_r, LED10_r, LED11_r, LED12_r, LED13_r, LED14_r, LED15_r
}LED_r;

typedef struct I2C_Flags_t{    
    _Bool START;
    _Bool SEND;
    _Bool STOP;
} I2C_Flags;

typedef struct led_t{
    LED_r reg;
    uint8_t brightness;
} LED;

I2C_Flags I2C_FLAGS;

void main(void) {
        
    sys_initialize();
    i2c_initialize();
    driver_address = LED_DRIVER_ADDRESS;
    
    LED LED1 = {LED1_r, 0x00};
    LED LED2 = {LED2_r, 0x00};
    LED LED3 = {LED3_r, 0x00};
    LED LED4 = {LED4_r, 0x00};
    LED LED5 = {LED5_r, 0x00};
    LED LED6 = {LED6_r, 0x00};
    LED LED7 = {LED7_r, 0x00};
    LED LED8 = {LED8_r, 0x00};
    LED LED9 = {LED9_r, 0x00};
    LED LED10 = {LED10_r, 0x00};
    LED LED11 = {LED11_r, 0x00};
    LED LED12 = {LED12_r, 0x00};
    LED LED13 = {LED13_r, 0x00};
    LED LED14 = {LED14_r, 0x00};
    LED LED15 = {LED15_r, 0x00};

    
    
    while(1){ 

        I2C_FLAGS.STOP = I2C_Stop();
        
        I2C_FLAGS.START = I2C_Startup();
        
        I2C_FLAGS.SEND = I2C_Transmit(LED_DRIVER_ADDRESS);
        I2C_FLAGS.SEND = I2C_Transmit(0x00);
        I2C_FLAGS.SEND = I2C_Transmit(0x00);
        I2C_FLAGS.STOP = I2C_Stop();
       
        I2C_FLAGS.START = I2C_Startup();
        I2C_FLAGS.SEND = I2C_Transmit(LED_DRIVER_ADDRESS);
        I2C_FLAGS.SEND = I2C_Transmit(0x1F & 0x02);
        I2C_FLAGS.SEND = I2C_Transmit(0xFF);
        I2C_FLAGS.STOP = I2C_Stop();
        
        I2C_FLAGS.START = I2C_Startup();
        I2C_FLAGS.SEND = I2C_Transmit(LED_DRIVER_ADDRESS);        
        I2C_FLAGS.SEND = I2C_Transmit(0x1F & 0x14);
        I2C_FLAGS.SEND = I2C_Transmit(0b00000010);  
        I2C_FLAGS.STOP = I2C_Stop();
        

        
    }
    
    return;
}

void sys_initialize(void){
    
        
    // CLKRMD CLKR enabled; SYSCMD SYSCLK enabled; FVRMD FVR enabled; IOCMD IOC enabled; NVMMD NVM enabled; 
    PMD0 = 0x00;
    // TMR0MD TMR0 enabled; TMR1MD TMR1 enabled; TMR2MD TMR2 enabled; NCOMD DDS(NCO) enabled; 
    PMD1 = 0x00;
    // ZCDMD ZCD disabled; CMP1MD CMP1 disabled; ADCMD ADC disabled; CMP2MD CMP2 disabled; DAC1MD DAC1 disabled; 
    PMD2 = 0x67;
    // CCP2MD CCP2 disabled; CCP1MD CCP1 disabled; PWM4MD PWM4 disabled; PWM3MD PWM3 disabled; PWM6MD PWM6 disabled; PWM5MD PWM5 disabled; 
    PMD3 = 0x3F;
    // CWG1MD CWG1 disabled; UART2MD EUSART2 disabled; MSSP1MD MSSP1 enabled; UART1MD EUSART disabled; 
    PMD4 = 0xC1;
    // CLC3MD CLC3 disabled; CLC4MD CLC4 disabled; CLC1MD CLC1 disabled; CLC2MD CLC2 disabled; 
    PMD5 = 0x1E;
    SSP1CLKPPS = 0x10;
    SSP1DATPPS = 0x11;
    RC0PPS = 0x15;
    RC1PPS = 0x16;
    
    I2C_FLAGS.START = SUCCESS;
    I2C_FLAGS.SEND = SUCCESS;
    I2C_FLAGS.STOP = SUCCESS;
    
    
}

void i2c_initialize(void){
    // setup SCK and SDA
    
    // All port C pins digital
    ANSELC = 0x00;
    
    //Set MSSP pins to inputs
    SCK_tris = 1;
    SDA_tris = 1;
    
    // SMP Standard Speed; CKE disabled; 
    SSP1STAT = 0x80;
    // SSPEN enabled; I2C master mode
    SSP1CON1 = 0x28;
    // SBCDE disabled; BOEN disabled; SCIE disabled; PCIE disabled; DHEN disabled; SDAHT 100ns; AHEN disabled; 
    SSP1CON3 = 0b01110000;
    // SSPADD 3; 
    SSP1ADD = 0x04; //50kHz 4f
  
}



_Bool I2C_Startup(void){
    
    SSP1CON2bits.SEN = 1; //Initial start condition
    while(SSP1CON2bits.SEN != 0);
    
    
    //Check for bus collision
    if (PIR3bits.BCL1IF == 1){
        PIR3bits.BCL1IF = 0;
        return FAILURE;
    }
    //Otherwise clear interrupt
    else{
        PIR3bits.SSP1IF = 0;
        __delay_ms(1);
        return SUCCESS;

    }   
}

_Bool I2C_Transmit(uint8_t data){
    
    SSP1BUF = data;
    
    if(SSP1CON2bits.ACKSTAT == 1){
        return FAILURE;
    }
    else{
        if (PIR3bits.BCL1IF == 1){
            PIR3bits.BCL1IF = 0;
            return FAILURE;
        } 
        else{
            PIR3bits.SSP1IF = 0;
            __delay_ms(1);
            return SUCCESS;

        }
    }
    
}

_Bool I2C_Stop(void){
    PIR3bits.SSP1IF = 0;
    SSP1CON2bits.PEN = 1;
    while(SSP1CON2bits.PEN != 0);

    if (PIR3bits.BCL1IF == 1){
//            PIR3bits.BCL1IF = 0;
            return FAILURE;
        } 
        else{
            PIR3bits.SSP1IF = 0;
            __delay_ms(1);
            return SUCCESS;
            ;
    }
       
}

void Adjust_LED(LED LEDno, uint8_t brightness){
    
    
}