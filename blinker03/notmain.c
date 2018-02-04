
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
extern void ASMDELAY ( unsigned int );

#define DMTIMER2_BASE 0x48040000
#define DMTIMER2_TCLR (DMTIMER2_BASE+0x38)
#define DMTIMER2_TCRR (DMTIMER2_BASE+0x3C)

#define GPIO1_BASE              0x4804C000
#define GPIO1_OE                (GPIO1_BASE+0x134)
#define GPIO1_CLEARDATAOUT      (GPIO1_BASE+0x190)
#define GPIO1_SETDATAOUT        (GPIO1_BASE+0x194)

#define CM_PER_BASE     0x44E00000
#define CM_PER_GPIO1_CLKCTRL (CM_PER_BASE+0xAC)
#define CM_PER_TIMER2_CLKCTRL (CM_PER_BASE+0x80)

#define CM_DPLL 0x44E00500
#define CLKSEL_TIMER2_CLK (CM_DPLL+0x08)

int notmain ( void )
{
    unsigned int ra;

    PUT32(CM_PER_GPIO1_CLKCTRL,2);
    while(1)
    {
        if((GET32(CM_PER_GPIO1_CLKCTRL)&0x00030000)==0) break;
    }
    PUT32(GPIO1_OE,0xFE1FFFFF);

    PUT32(CM_PER_TIMER2_CLKCTRL,2);
    while(1)
    {
        if((GET32(CM_PER_TIMER2_CLKCTRL)&0x00030000)==0) break;
    }

    PUT32(CLKSEL_TIMER2_CLK,1);
    PUT32(DMTIMER2_TCLR,0x00000003);
    while(1)
    {
        PUT32(GPIO1_SETDATAOUT,0x01E00000);
        while(1)
        {
            ra=GET32(DMTIMER2_TCRR);
            if((ra&=0x2000000)==0) break;
        }
        PUT32(GPIO1_CLEARDATAOUT,0x01E00000);
        while(1)
        {
            ra=GET32(DMTIMER2_TCRR);
            if((ra&=0x2000000)!=0) break;
        }
    }
    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
