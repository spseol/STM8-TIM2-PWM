/*
 * Soubor:  keypad.h
 * Datum:   14.04.2021 10:26
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL 
 * Úloha: 
 * Popis:   hlavičkový soubor pro keypad.c
 */
#include "stm8s.h"

#ifndef __KEYPAD_H__
#define __KEYPAD_H__

// sloupce
#define C1_PORT GPIOG
#define C1_PIN  GPIO_PIN_0
#define C2_PORT GPIOC
#define C2_PIN  GPIO_PIN_2
#define C3_PORT GPIOC
#define C3_PIN  GPIO_PIN_3
#define C4_PORT GPIOD
#define C4_PIN  GPIO_PIN_1
// řádky
#define R1_PORT GPIOC
#define R1_PIN  GPIO_PIN_1
#define R2_PORT GPIOE
#define R2_PIN  GPIO_PIN_0
#define R3_PORT GPIOD
#define R3_PIN  GPIO_PIN_5
#define R4_PORT GPIOD
#define R4_PIN  GPIO_PIN_6

// aktivuje == uzemní řádek
#define ROW_ON(R) GPIO_WriteLow(R##_PORT, R##_PIN);
// deaktivuje řádek == přepne do stavu vysoké impedance
#define ROW_OFF(R) GPIO_WriteHigh(R##_PORT, R##_PIN);
// je tento sloupec přibitý k zemi?
#define COLUMN_GET(C) (GPIO_ReadInputPin(C##_PORT, C##_PIN)==RESET) 

void init_keypad(void);
uint8_t check_keypad(void);


#endif
