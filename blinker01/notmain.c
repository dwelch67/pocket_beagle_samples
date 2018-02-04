
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define GPIO1_BASE              0x4804C000
#define GPIO1_OE                (GPIO1_BASE+0x134)
#define GPIO1_CLEARDATAOUT      (GPIO1_BASE+0x190)
#define GPIO1_SETDATAOUT        (GPIO1_BASE+0x194)
#define CM_PER_BASE             0x44E00000
#define CM_PER_GPIO1_CLKCTRL    (CM_PER_BASE+0xAC)

int notmain ( void )
{
    unsigned int ra;

    PUT32(CM_PER_GPIO1_CLKCTRL,2);
    while(1)
    {
        if((GET32(CM_PER_GPIO1_CLKCTRL)&0x00030000)==0) break;
    }
    PUT32(GPIO1_OE,0xFE1FFFFF);
    while(1)
    {
        PUT32(GPIO1_SETDATAOUT,0x01E00000);
        for(ra=0;ra<0x200000;ra++) dummy(ra);
        PUT32(GPIO1_CLEARDATAOUT,0x01E00000);
        for(ra=0;ra<0x200000;ra++) dummy(ra);
    }
    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
