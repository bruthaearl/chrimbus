#include <stdio.h>
#include <stdint.h>

typedef struct LEDRGB_t{
    uint8_t LED_R;
    uint8_t LED_G;
    uint8_t LED_B;
} LED_RGB;


union LED{
    LED_RGB LEDRGB;
    int rvalue;
} ;

union LED LED1, LED2, LED3, LED4, LED5;

int main()
{
    
    LED1.rvalue = 0xAABBCC;
    printf("Size of LED: %d\n", LED1.LEDRGB.LED_R);

    return 0;
}
