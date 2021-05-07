#ifndef _UTIL_DELAY_H_
#define _UTIL_DELAY_H_ 1

#ifndef F_CPU
#warning F_CPU is not defined!
#endif

/* 
 * Func delayed N cycles, where N = 3 + ( ticks * 3 )
 * so, ticks = ( N - 3 ) / 3, minimum delay is 6 CLK
 * when tick = 1, because 0 equels 65535
 */

static @inline void _delay_cycl( unsigned short __ticks )
{
#if defined(__CSMC__)
/* COSMIC */
  //#define T_COUNT(x) (( F_CPU * x / 1000000UL )-3)/3) // p�vodn� varianta byla p�esn�j�� ale hrozila podt�k�n�m
	#define T_COUNT(x) (( F_CPU * x / 1000000UL ))/3)+1
	// ldw X, __ticks ; insert automaticaly
	_asm("nop\n $N:\n decw X\n jrne $L\n nop\n ", __ticks);
#elif defined(__SDCC)
  //#define T_COUNT(x) (( F_CPU * x / 1000000UL )-5)/5) // p�vodn� varianta byla p�esn�j�� ale hrozila podt�k�n�m
	#define T_COUNT(x) (( F_CPU * x / 1000000UL ))/5)+1
	__asm__("nop\n nop\n"); 
	do { 		// ASM: ldw X, #tick; lab$: decw X; tnzw X; jrne lab$
                __ticks--;//      2c;                 1c;     2c    ; 1/2c   
        } while ( __ticks );
	__asm__("nop\n");
#elif defined(__RCST7__)
/* RAISONANCE */
  #error ToDo for RAISONANCE
#elif defined(__ICCSTM8__)
/* IAR */
  #error ToDo for IAR
#else
 #error Unsupported Compiler!          /* Compiler defines not found */
#endif
}

static @inline void _delay_us( const unsigned short __us ){
	_delay_cycl( (unsigned short)( T_COUNT(__us) );
}

void _delay_ms(uint16_t ms)
{
    for (uint16_t d = 0; d < ms; d++)
    {
        _delay_us(250);
        _delay_us(250);
        _delay_us(250);
        _delay_us(245);
    }
}


#endif
