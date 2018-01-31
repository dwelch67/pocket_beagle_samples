
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
extern void ASMDELAY ( unsigned int );

#define GPIO1_BASE      0x4804C000
#define GPIO1_OE        (GPIO1_BASE+0x134)
#define GPIO1_DATAOUT   (GPIO1_BASE+0x13C)

#define CM_PER_BASE     0x44E00000
#define CM_PER_GPIO1_CLKCTRL (CM_PER_BASE+0xAC)
#define CM_PER_TIMER2_CLKCTRL (CM_PER_BASE+0x80)
#define CM_WKUP_UART0_CLKCTRL (CM_PER_BASE+0xB4+0x400)

#define CM_DPLL 0x44E00500
#define CLKSEL_TIMER2_CLK (CM_DPLL+0x08)

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

    PUT32(CM_PER_GPIO1_CLKCTRL,2);
    PUT32(GPIO1_OE,(~(1<<24)));
        PUT32(GPIO1_DATAOUT,1<<24);

    PUT32(CM_WKUP_UART0_CLKCTRL,2);
    PUT32(CONF_UART0_RXD,0);
    PUT32(CONF_UART0_TXD,0);
    
    //PUT32(UART0_SYSC,0x2);
    //while(1)
    //{
        //if(GET32(UART0_SYSS)&1) break;
    //}

    //PUT32(UART0_MDR1,0x07);

    //PUT32(UART0_LCR,0x83);
    //PUT32(UART0_DLL,0);
    //PUT32(UART0_DLH,0);
    //PUT32(UART0_LCR,0x03);
    //PUT32(UART0_MCR,0x00);
    //PUT32(UART0_FCR,0x07);

    PUT32(UART0_LCR,0x83);
    PUT32(UART0_DLL,26);
    PUT32(UART0_DLH,0);
    PUT32(UART0_LCR,0x03);
    PUT32(UART0_MCR,0x00);

    PUT32(UART0_MDR1,0x00);

    rx=0x30;
    while(1)
    {
        PUT32(GPIO1_DATAOUT,1<<24);
        while(1)
        {
            if(GET32(UART0_LSR)&0x20) break;
        }
        PUT32(UART0_THR,rx);
        rx=(rx+1)&0x37;
        PUT32(GPIO1_DATAOUT,0);
        while(1)
        {
            if(GET32(UART0_LSR)&0x20) break;
        }
        PUT32(UART0_THR,rx);
        rx=(rx+1)&0x37;
    }

    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
