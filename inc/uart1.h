/*
 * Soubor:  uart1.h
 * Datum:   07.05.2021 07:17
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL 
 * Popis:   hlavičkový soubor pro uart1.c
 */

#ifndef __UART1_H__
#define __UART1_H__


#include "stm8s.h"
#include <stdio.h>

#define _ISOC99_SOURCE
#define _GNU_SOURCE

/* ***************    Makra    ******************** */
#ifdef _RAISONANCE_
  #define PUTCHAR_PROTOTYPE int putchar (char c)
  #define GETCHAR_PROTOTYPE int getchar (void)
#elif defined (_COSMIC_)
  #define PUTCHAR_PROTOTYPE char putchar (char c)
  #define GETCHAR_PROTOTYPE char getchar (void)
#elif defined (_SDCC_)         /* SDCC patch: ensure same types as stdio.h */
  #if SDCC_VERSION >= 30605      // declaration changed in sdcc 3.6.5 (officially with 3.7.0)
    #define PUTCHAR_PROTOTYPE int putchar(int c)
    #define GETCHAR_PROTOTYPE int getchar(void)
  #else
    #define PUTCHAR_PROTOTYPE void putchar(char c)
    #define GETCHAR_PROTOTYPE char getchar(void)
  #endif 
#else /* _IAR_ */
  #define PUTCHAR_PROTOTYPE int putchar (int c)
  #define GETCHAR_PROTOTYPE int getchar (void)
#endif /* _RAISONANCE_ */


void init_uart1(void);

#endif

