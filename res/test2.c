#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>


/* 
Structure for the five RGB LEDs. 
R,G,B: PWM values
reg: starting register for auto-increment
num: LED num
*/
typedef struct LED_t {
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t reg;
    uint8_t num; 
} LED;

/*
Write the PWM values to LED controller
led points to LED struct
*/
void write_led(LED *led);

/*
Initialize LEDs
*/
LED LED1 = {0, 0, 0, 0x02, 1};
LED LED2 = {0, 0, 0, 0x05, 2};
LED LED3 = {0, 0, 0, 0x08, 3};
LED LED4 = {0, 0, 0, 0x0B, 4};
LED LED5 = {0, 0, 0, 0x0E, 5};

/*
main loop
*/
int main(){

   write_led(&LED1);
   write_led(&LED2);
   write_led(&LED3);
   write_led(&LED4);
   write_led(&LED5);

    return 0;
}

/*
Write PWM values to controller
*/
void write_led(LED *led) {
    
    volatile int r;
    
    //seed from time
    srand(time(0));
    //get random number
    r = rand();
    
    //bit shift and mask for 1st, 2nd, and 3rd bytes
    led->R = (uint8_t) (r>>16) & 0xFF;
    led->G = (uint8_t) (r>>8) & 0xFF;
    led->B = (uint8_t) r & 0xFF;
    
    printf("LED%d \n", led->num);
    printf("Starting Address: %X \n", led->reg);
    printf("random number: %X \n", r);
    printf("R: %X \n", led->R);
    printf("G: %X \n", led->G);
    printf("B: %X \n", led->B);
    
}


void Initialize_Driver(){
    
    // startup delay
    
    
    //Turn the damn thing on
    //Send start condition
    I2C_Startup();
    //Slave Address
    I2C_Transmit(LED_DRIVER_ADDRESS);
    //Select MODE1 register 0x00
    I2C_Transmit(0x00);
    //Enable main oscillator
    I2C_Transmit(0x00);
    //Done with this block
    I2C_Stop();
    
    //Enable output drivers
    //Send start condition
    I2C_Startup();
    //Slave address
    I2C_Transmit(LED_DRIVER_ADDRESS);
    //Select LEDOUT0 register 0x14 WITH AUTO INCREMENT
    I2C_Transmit(0b10000000 | 0x14);
    //Enable LEDOUT0
    I2C_Transmit(0xAA);
    //Enable LEDOUT1
    I2C_Transmit(0xAA);
    //Enable LEDOUT2
    I2C_Transmit(0xAA);
    //Enable LEDOUT3
    I2C_Transmit(0xAA);
    //Done with this block
    I2C_Stop();
    
    //Turn on star LED
    //Send start condition
    I2C_Startup();
    //Slave address
    I2C_Transmit(LED_DRIVER_ADDRESS);
    //Select PWM15 register 0x11
    I2C_Transmit(0x11);
    //Light em up bb
    I2C_Transmit(0xFF);
    //Done with this block
    I2C_Stop;
    
}

void Write_LED(LED *led){
    
    // SEED CODE GOES HERE
    
    // Generate random number
    volatile r = rand();
    
    //Bit-shift and mask for RGB values
    led->R = (uint8_t) (r>>16) & 0xFF;
    led->G = (uint8_t) (r>>8) & 0xFF;
    led->B = (uint8_t) r & 0xFF;
    
    //Send start condition
    I2C_Startup();
    //Slave address
    I2C_Transmit(LED_DRIVER_ADDRESS);
    //Select PWM0 register 0x02 WITH AUTO INCREMENT
    I2C_Transmit(0b10100000 | led->reg);
    //Send R value
    I2C_Transmit(led->R);
    //Send G value
    I2C_Transmit(led->G);
    //Send B value
    I2C_Transmit(led->B);
    //Hopefully things worked?
    I2C_Stop();
    
}
