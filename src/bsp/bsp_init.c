#include "heads.h"


static void ADC_init(void)
{
    // initialize A/D convertor
    ADCCRL = 0xA0;  	// setting
    ADCCRH = 0x00;  	// trigger source, alignment, frequency
}

static void ADC_start(unsigned char ch)
{
    // start A/D convertor
    ADCCRL = (ADCCRL & 0xf0) | (ch & 0x0f);	// select channel
    ADCCRL |= 0x40; 	// start ADC
}

static void BIT_init(void)
{
    // initialize Basic interval timer
    BITCR = 0x66;   	// period = 0.128000mS
    BITCR |= 0x08;  	// clear counter
    BITCR &= 0x7F;  	// clear BIT interrupt flag
    IE3 |= 0x10;    	// Enable BIT interrupt
}

static void BUZ_OnOff(unsigned char On)
{
    // Buzzer ON(On=1) / OFF(On=0)
    BUZCR = (On) ? (BUZCR | 1) : (BUZCR & (~1));	// ON / OFF
}

static void BUZ_init(void)
{
    // initialize Buzzer
    // Frequency (Hz) = 3968.253968
    BUZCR = 0x00;   	// clock source
    BUZDR = 0x3E;   	// count value
}

static void LCD_init(void)
{
    // initialize LCD
    // Frame freq. = 16.000000Hz
    LCDCRH = 0x00;  	// LCD control High
    LCDCRL = 0x00;  	// LCD control Low
    LCDCCR = 0x00;  	// LCD contrast
}

static void Timer0_init(void)
{
    // initialize Timer0
    // 8bit timer, use external input EC0
    T0CR = 0x8E;    	// timer setting
    T0DR = 0xff;    	// period count
    IE2 |= 0x02;    	// Enable Timer0 interrupt
    T0CR |= 0x01;   	// clear counter
}

static void Timer1_init(void)
{
    // initialize Timer1
    // 16bit PWM, period = 0.250000mS ( 4000.000000Hz )
    // PWM duty = 50.000000%
    T1CRH = 0x20;   	// PWM setting High, 1 shot mode
    T1CRH |= 0x30;	    // PWM repeat mode
    T1CRL = 0xC0;   	// PWM setting Low
    //T1BDRH = 0x07;  	// duty High
    //T1BDRL = 0xCF;  	// duty Low
    T1BDRH = 0x00;  	// duty High
    T1BDRL = 0x00;  	// duty Low
    T1ADRH = 0x0F;  	// period count High
    T1ADRL = 0x9F;  	// period count Low
    T1CRH |= 0x80;  	// enable counter
}

static void UART_init(void)
{
    // initialize UART interface
    // UART1 : ASync. 9615bps N 8 1
    USI1CR2 = 0x02; 	// activate USI1
    USI1CR1 = 0x06; 	// Async/Sync, bit count, parity
    USI1CR2 |= 0xEC;	// interrupt, speed
    USI1CR3 = 0x00; 	// stop bit
    USI1BD = 0x67;  	// baud rate
    IE |= 0x08;     	// enable UART1 interrupt
    IE |= 0x10; 
}

static void WDT_clear(void)
{
    WDTCR |= 0x20;  	// Clear Watch-dog timer
}

static void WDT_init(void)
{
    // initialize Watch-dog timer
    WDTDR = 0x88;   	// period
    WDTCR = 0xC0;   	// setting
                        //WDTCR |= 0x02;	// Use WDTRC
    WDT_clear();
}

static void WT_init(void)
{
    // initialize Watch timer
    WTCR = 0x88;    	// setting
    WTDR = 0x01;    	// set duty
    WTDR = 0x81;    	// clear WT
    IE3 |= 0x04;    	// Enable WT interrupt
}

static void clock_init(void)
{
    // internal RC clock (16.000000MHz)
    OSCCR = 0x29;   	// Set Int. OSC
    SCCR = 0x00;    	// Use Int. OSC
}

static void port_init(void)
{
    // initialize ports
    //   1 : P55      in  RESET
    //   2 : P40      out STB
    //   3 : P41      out CLK
    //   4 : P42      out DIO
    //   5 : P43      out DUST_POWER
    //   6 : P37      out P1_COM3
    //   7 : P36      out P1_COM2
    //   8 : P35      out P1_COM1
    //   9 : P34      out P1_LED2
    //  10 : P33      out P1_LED1
    //  11 : P32      out P2_LED1
    //  12 : P31      out P2_LED2
    //  13 : P30      out P2_COM1
    //  14 : P27      out P2_COM2
    //  15 : P26      out P2_COM3
    //  16 : P25      in  WIFI_STA
    //  17 : P24      in  IR
    //  18 : P23      out CO
    //  19 : P22      out SQU
    //  20 : P21      in  DUST2
    //  21 : TxD1     out TX
    //  22 : RxD1     in  RX
    //  23 : P11      in  POWER
    //  24 : PWM1o    out FAN_CTR
    //  25 : BUZo     out BUZ
    //  26 : P14      out UV
    //  27 : P15      out HV
    //  28 : P16      out ION
    //  32 : P05      in  HV_TEST
    //  33 : AN2      in  AD_HUM
    //  34 : AN1      in  AD_TEM
    //  35 : AVREF    in  AVREF
    //  42 : EC0      in  FAN_FB
    //  43 : SXIN     in  SXIN
    //  44 : SXOUT    out SXOUT
    P0IO = 0xC3;    	// direction
    P0PU = 0x20;    	// pullup
    P0OD = 0x00;    	// open drain
    P0DB = 0x00;    	// bit7~6(debounce clock), bit5~0=P07~02 debounce
    P0 = 0x00;    	// port initial value

    P1IO = 0xFC;    	// direction
    P1PU = 0x00;    	// pullup
    P1OD = 0x00;    	// open drain
    P15DB = 0x00;   	// debounce : P54, 52, 17, 16, 12, 11
    P1 = 0x00;    	// port initial value

    P2IO = 0xCD;    	// direction
    P2PU = 0x10;    	// pullup
    P2OD = 0x00;    	// open drain
    P2 = 0x00;    	// port initial value

    P3IO = 0xFF;    	// direction
    P3PU = 0x00;    	// pullup
    P3 = 0x00;    	// port initial value

    P4IO = 0xFF;    	// direction
    P4PU = 0x04;    	// pullup
    P4OD = 0x00;    	// open drain
    P4 = 0x00;    	// port initial value

    P5IO = 0xD1;    	// direction
    P5PU = 0x00;    	// pullup
    P5 = 0x00;    	// port initial value

                    // Set port functions
    P0FSRH = 0x00;  	// P0 selection High
    P0FSRL = 0x52;  	// P0 selection Low
    P1FSRH = 0x00;  	// P1 selection High
    P1FSRL = 0xF3;  	// P1 selection Low
    P2FSRH = 0x00;  	// P2 selection High
    P2FSRL = 0x03;  	// P2 selection Low
    P3FSR = 0x00;   	// P3 selection
    P4FSR = 0x00;   	// P4 selection
    P5FSR = 0x28;   	// P5 selection
}

void BSP_Init(void)
{
    LVRCR = 0X01;

    cli();          	// disable INT. during peripheral setting
    port_init();    	// initialize ports
    clock_init();   	// initialize operation clock
    WT_init();
    while (1)
    {
        if (P1 & BIT1)
        {
            sei();
        }
        else
        {
            break;
        }
    }
    
    ADC_init();     	// initialize A/D convertor
    BIT_init();     	// initialize Basic interval timer
    BUZ_init();     	// initialize Buzzer
    LCD_init();     	// initialize LCD
    Timer0_init();  	// initialize Timer0
    Timer1_init();  	// initialize Timer1
    UART_init();    	// initialize UART interface
    //WDT_init();     	// initialize Watch-dog timer
    //WT_init();      	// initialize Watch timer
    //sei();          	// enable INT.
}
