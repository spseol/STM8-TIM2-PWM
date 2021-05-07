#include "milis.h"
#include "stm8s.h"

#include "uart1.h"
#include "keypad.h"

#define _ISOC99_SOURCE
#define _GNU_SOURCE

#define LEDR_PORT GPIOD
#define LEDR_PIN  GPIO_PIN_4
#define LEDG_PORT GPIOD
#define LEDG_PIN  GPIO_PIN_3
#define LEDB_PORT GPIOA
#define LEDB_PIN  GPIO_PIN_3


#define LED_ON(COLOR)   GPIO_WriteHigh(LED##COLOR##_PORT, LED##COLOR##_PIN);
#define LED_OFF(COLOR)  GPIO_WriteLow(LED##COLOR##_PORT, LED##COLOR##_PIN);
#define LED_REVERSE(COLOR) GPIO_WriteReverse(LED##COLOR##_PORT, LED##COLOR##_PIN);

#define BTN_PORT GPIOE
#define BTN_PIN  GPIO_PIN_4
#define BTN_PUSH (GPIO_ReadInputPin(BTN_PORT, BTN_PIN)==RESET) 


/*----------------------------------------------------------------------------------------------*/



void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovat MCU na 16MHz

    init_milis();
    init_keypad();
    init_uart1();

    /*----          GPIO setup           ---------*/
    GPIO_Init(LEDR_PORT, LEDR_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LEDG_PORT, LEDG_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LEDB_PORT, LEDB_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    /*----           TIM2 setup           ---------*/
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 7000 - 1 ); 
    //TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    TIM2_OC1Init(                // inicializujeme kanál 1 (TM2_CH1)
        TIM2_OCMODE_PWM1,        // režim PWM1
        TIM2_OUTPUTSTATE_ENABLE, // Výstup povolen (TIMer ovládá pin)
        3000,                    // výchozí hodnota šířky pulzu (střídy) 1056/1600 = 66%
        TIM2_OCPOLARITY_HIGH      // Polarita LOW protože LED rozsvěcím 0 (spol. anoda)
     );
    TIM2_OC2Init(TIM2_OCMODE_PWM1,TIM2_OUTPUTSTATE_ENABLE,
                 3000, TIM2_OCPOLARITY_HIGH);
    TIM2_OC3Init(TIM2_OCMODE_PWM1,TIM2_OUTPUTSTATE_ENABLE,
                 3000, TIM2_OCPOLARITY_HIGH);

    // ošetření nežádoucích jevů při změně PWM
    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_OC2PreloadConfig(ENABLE);
    TIM2_OC3PreloadConfig(ENABLE);

    TIM2_Cmd(ENABLE);

}



int main(void)
{
    int32_t time_led = 0;
    int32_t time_key = 0;
    uint8_t key_last = 0;
    uint8_t key_now = 0;
    uint8_t key_pressed = 0;
    uint16_t PWM_R = 3000;
    uint16_t PWM_G = 3000;
    uint16_t PWM_B = 3000;

    setup();

    /*------  nekonečná smyčka    ---*/
    while (1) {
        if (milis() - time_led > 777) {
            time_led = milis();
            LED_REVERSE(B);
        }
        if (milis() - time_key > 50) {
            time_key = milis();
            key_now = check_keypad();
            if (key_last == 0xFF && key_now != 0xFF) {
                key_pressed = key_now;
                printf("%X", key_pressed);

                // nastavení R,G a B kanálu probíhá vždy v 7 krocích.
                if (key_pressed == 1) {
                    PWM_R += 1000;
                    if (PWM_R > 7000 ) {
                        PWM_R = 7000;
                    }
                    printf("R: %u\n", PWM_R);
                }
                if (key_pressed == 5) {
                    PWM_R -= 1000;
                    if (PWM_R > 7000 ) {  // podtečení
                        PWM_R = 0;
                    }
                    printf("R: %u\n", PWM_R);
                }
                if (key_pressed == 2) {
                    PWM_G += 1000;
                    if (PWM_G > 7000 ) {
                        PWM_G = 7000;
                    }
                    printf("G: %u\n", PWM_G);
                }
                if (key_pressed == 6) {
                    PWM_G -= 1000;
                    if (PWM_G > 7000 ) {
                        PWM_G = 0;
                    }
                    printf("G: %u\n", PWM_G);
                }
                if (key_pressed == 3) {
                    PWM_B += 1000;
                    if (PWM_B > 7000 ) {
                        PWM_B = 7000;
                    }
                    printf("B: %u\n", PWM_B);
                }
                if (key_pressed == 7) {
                    PWM_B -= 1000;
                    if (PWM_B > 7000) {
                        PWM_B = 0;
                    }
                    printf("B: %u\n", PWM_B);
                }
                TIM2_SetCompare1(PWM_R);
                TIM2_SetCompare2(PWM_G);
                TIM2_SetCompare3(PWM_B);
            }
            key_last = key_now;
        }
    }
}


/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
