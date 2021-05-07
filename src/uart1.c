/*
 * Soubor:  uart.c
 * Datum:   15.01.2021 13:24
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL 
 * Úloha: 
 * Popis:   
 ****************************************************/
#include "uart1.h"
/* ***************   Funkce    ******************** */

void init_uart1(void)
{
    UART1_DeInit();         // smažu starou konfiguraci
    UART1_Init((uint32_t) 115200,
               UART1_WORDLENGTH_8D,
               UART1_STOPBITS_1,
               UART1_PARITY_NO,
               UART1_SYNCMODE_CLOCK_DISABLE,
               UART1_MODE_TXRX_ENABLE);
    UART1_Cmd(ENABLE);  // povolí UART1

    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);   // povolí přerušení UART1 Rx
    enableInterrupts();
}


PUTCHAR_PROTOTYPE
{
  /* Loop until the end of transmission */
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
  /* Write a character to the UART1 */
  UART1_SendData8(c);

  return (c);
}

/**
  * @brief Retargets the C library scanf function to the USART.
  * @param None
  * @retval char Character to Read
  */
GETCHAR_PROTOTYPE
{
#ifdef _COSMIC_
    char c = 0;
#else
    int c = 0;
#endif

    /* Loop until the Read data register flag is SET */
    while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);
    c = UART1_ReceiveData8();
    return (c);
}
