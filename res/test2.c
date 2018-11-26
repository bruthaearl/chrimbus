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
typedef struct LED_t{
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
{R, G, B, register, LED number}
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
void write_led(LED *led){
    
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
