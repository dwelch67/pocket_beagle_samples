
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
extern void ASMDELAY ( unsigned int );

#define CM_PER_BASE     0x44E00000
#define CM_WKUP_UART0_CLKCTRL (CM_PER_BASE+0xB4+0x400)

#define UART0_BASE 0x44E09000
#define UART0_THR  (UART0_BASE+0x00)
#define UART0_RHR  (UART0_BASE+0x00)
#define UART0_DLL  (UART0_BASE+0x00)
#define UART0_DLH  (UART0_BASE+0x04)
#define UART0_FCR  (UART0_BASE+0x08)
#define UART0_LCR  (UART0_BASE+0x0C)
#define UART0_MCR  (UART0_BASE+0x10)
#define UART0_LSR  (UART0_BASE+0x14)

#define UART0_MDR1 (UART0_BASE+0x20)
#define UART0_SYSC (UART0_BASE+0x54)
#define UART0_SYSS (UART0_BASE+0x58)

#define CONF_BASE 0x44E10000
#define CONF_UART0_RXD (CONF_BASE+0x970)
#define CONF_UART0_TXD (CONF_BASE+0x974)

int notmain ( void )
{
    unsigned int rx;

    PUT32(CM_WKUP_UART0_CLKCTRL,2);
    while(1)
    {
        if((GET32(CM_WKUP_UART0_CLKCTRL)&0x00030000)==0) break;
    }
    PUT32(CONF_UART0_RXD,0x20);
    PUT32(CONF_UART0_TXD,0x00);

    PUT32(UART0_SYSC,0x2);
    while(1)
    {
        if(GET32(UART0_SYSS)&1) break;
    }
    PUT32(UART0_MDR1,0x07);

    PUT32(UART0_LCR,0x83);
    PUT32(UART0_DLL,0);
    PUT32(UART0_DLH,0);
    PUT32(UART0_LCR,0x03);
    PUT32(UART0_MCR,0x00);
    PUT32(UART0_FCR,0x07);

    PUT32(UART0_LCR,0x83);
    PUT32(UART0_DLL,26);
    PUT32(UART0_DLH,0);
    PUT32(UART0_LCR,0x03);
    PUT32(UART0_MCR,0x00);

    PUT32(UART0_MDR1,0x00);

    for(rx=0x30;;rx++)
    {
        rx&=0x37;
        while(1)
        {
            if(GET32(UART0_LSR)&0x20) break;
        }
        PUT32(UART0_THR,rx);
    }

    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
