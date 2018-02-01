
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

extern void PUT8 ( unsigned int, unsigned int );
extern void BRANCHTO ( unsigned int );

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

static unsigned int uart_recv ( void )
{
    while(1)
    {
        if(GET32(UART0_LSR)&0x01) break;
    }
    return(GET32(UART0_RHR)&0xFF);
}

static void uart_send ( unsigned int c )
{
    while(1)
    {
        if(GET32(UART0_LSR)&0x20) break;
    }
    PUT32(UART0_THR,c);
}

static void uart_flush ( void )
{
    while(1)
    {
        if(GET32(UART0_LSR)&0x40) break;
    }
}

static void uart_init ( void )
{
    PUT32(CM_WKUP_UART0_CLKCTRL,2);
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
}

static void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}

static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}

static unsigned int ctonib ( unsigned int c )
{
    if(c>0x39) c-=7;
    return(c&0xF);
}

static unsigned int minmax ( unsigned int a )
{
    if(a>=0x402FF000) return(1);
    if(a<=0x402F0400) return(1);
    return(0);
}



int notmain ( void )
{
    unsigned int state;
    unsigned int ra;
    unsigned int type;
    unsigned int count;
    unsigned int sum;
    unsigned int entry;
    unsigned int addr;
    unsigned int data;

    //leds_off();

    uart_init();
    hexstring(0x12345678);
    //hexstring(GETPC());
    uart_send(0x0D);
    uart_send(0x0A);
    uart_send('S');
    uart_send('R');
    uart_send('E');
    uart_send('C');
    uart_send(0x0D);
    uart_send(0x0A);

    data=0;
    state=0;
    count=0;
    sum=0;
    addr=0;
    type=0;
    entry=0x00000000;
    while(1)
    {
        ra=uart_recv();
        switch(state)
        {
            case 0:
            {

                if(ra=='S')
                {
                    sum=0;
                    state++;
                }
                if((ra=='g')||(ra=='G'))
                {
                    hexstring(entry);
                    uart_flush();
                    BRANCHTO(entry);
                }
                break;
            }
            case 1:
            {
                switch(ra)
                {
                    case '0':
                    {
                        state=0;
                        break;
                    }
                    case '3':
                    {
                        type=3;
                        state++;
                        break;
                    }
                    case '7':
                    {
                        type=7;
                        state++;
                        break;
                    }
                    default:
                    {
                        hexstring(ra);
                        hexstring(0xBADBAD00);
                        return(1);
                    }
                }
                break;
            }

            case 2:
            {
                count=ctonib(ra);
                state++;
                break;
            }
            case 3:
            {
                count<<=4;
                count|=ctonib(ra);
                if(count<5)
                {
                    hexstring(0xBADBAD01);
                    return(1);
                }
                sum+=count&0xFF;
                addr=0;
                state++;
                break;
            }
            case  4:
            case  6:
            case  8:
            case 10:
            {
                addr<<=4;
                addr|=ctonib(ra);
                state++;
                break;
            }
            case  5:
            case  7:
            case  9:
            {
                count--;
                addr<<=4;
                addr|=ctonib(ra);
                sum+=addr&0xFF;
                state++;
                break;
            }
            case 11:
            {
                count--;
                addr<<=4;
                addr|=ctonib(ra);
                sum+=addr&0xFF;
                state++;
                break;
            }
            case 12:
            {
                data=ctonib(ra);
                state++;
                break;
            }
            case 13:
            {
                data<<=4;
                data|=ctonib(ra);
                sum+=data&0xFF;
                count--;
                if(count==0)
                {
                    if(type==7)
                    {
                        entry=addr;
                        hexstring(entry);
                    }
                    sum&=0xFF;
                    if(sum!=0xFF)
                    {
                        hexstrings(0xBADBAD02);
                        hexstring(addr);
                        return(1);
                    }
                    state=0;
                }
                else
                {
                    if(minmax(addr))
                    {
                        hexstrings(0xBADBAD03);
                        hexstring(addr);
                    }
                    else
                    {
                        PUT8(addr,data);
                    }
                    addr++;
                    state=12;
                }
                break;
            }
        }
    }
    return(0);
}
