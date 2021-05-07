/*
 * Soubor:  keypad.c
 * Datum:   14.04.2021 10:29
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL 
 * Úloha: 
 * Popis:   
 ****************************************************/

#include "keypad.h"

void init_keypad(void)
{
    // řádky nastavím do režimu výstup Open-Drain s pořáteční hodnotou HiZ = vysoká impedance
    GPIO_Init(R1_PORT, R1_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    GPIO_Init(R2_PORT, R2_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    GPIO_Init(R3_PORT, R3_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    GPIO_Init(R4_PORT, R4_PIN, GPIO_MODE_OUT_OD_HIZ_SLOW);
    // sloupce nastavím jako vstupy s aktivovaným pull-up rezistorem (30-50k)
    GPIO_Init(C1_PORT, C1_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(C2_PORT, C2_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(C3_PORT, C3_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(C4_PORT, C4_PIN, GPIO_MODE_IN_PU_NO_IT);
}

uint8_t check_keypad(void)
{
    uint8_t bagr = 0xFF;

    // první rádek
    ROW_ON(R1);
    if (COLUMN_GET(C1)) {
        bagr = 1;
    }
    if (COLUMN_GET(C2)) {
        bagr = 2;
    }
    if (COLUMN_GET(C3)) {
        bagr = 3;
    }
    if (COLUMN_GET(C4)) {
        bagr = 4;
    }
    ROW_OFF(R1);
    
    // druhý řádek
    ROW_ON(R2);
    if (COLUMN_GET(C1)) {
        bagr = 5;
    }
    if (COLUMN_GET(C2)) {
        bagr = 6;
    }
    if (COLUMN_GET(C3)) {
        bagr = 7;
    }
    if (COLUMN_GET(C4)) {
        bagr = 8;
    }
    ROW_OFF(R2);

    ROW_ON(R3);
    if (COLUMN_GET(C1)) {
        bagr = 9;
    }
    if (COLUMN_GET(C2)) {
        bagr = 10;
    }
    if (COLUMN_GET(C3)) {
        bagr = 11;
    }
    if (COLUMN_GET(C4)) {
        bagr = 12;
    }
    ROW_OFF(R3);

    ROW_ON(R4);
    if (COLUMN_GET(C1)) {
        bagr = 13;
    }
    if (COLUMN_GET(C2)) {
        bagr = 14;
    }
    if (COLUMN_GET(C3)) {
        bagr = 15;
    }
    if (COLUMN_GET(C4)) {
        bagr = 0;
    }
    ROW_OFF(R4);

    return bagr;
}
