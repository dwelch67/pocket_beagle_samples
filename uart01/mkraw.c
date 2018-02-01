
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int data[130];
#define PROGMAX 0x1000
unsigned char myprog[PROGMAX+0x1000];
int main ( void )
{
    unsigned int rb;

    FILE *fp;

    fp=fopen("notmain.bin","rb");
    if(fp==NULL)
    {
        fprintf(stderr,"Error opening file notmain.bin\n");
        return(1);
    }
    rb=fread(myprog,1,sizeof(myprog),fp);
    fclose(fp);
    if(rb>PROGMAX)
    {
        fprintf(stderr,"program too big!?\n");
        return(1);
    }

    memset(data,0,sizeof(data));

    data[ 0]=0x00000040; // ----
    data[ 1]=0x0000000C; // TOC 1
    data[ 2]=0x00000000; //
    data[ 3]=0x00000000; //
    data[ 4]=0x00000000; //
    data[ 5]=0x45534843; // CHSETTINGS
    data[ 6]=0x4E495454; //
    data[ 7]=0x00005347; // ----

    data[ 8]=0xFFFFFFFF;// ----
    data[ 9]=0xFFFFFFFF;// TOC 2
    data[10]=0xFFFFFFFF;//
    data[11]=0xFFFFFFFF;//
    data[12]=0xFFFFFFFF;//
    data[13]=0xFFFFFFFF;//
    data[14]=0xFFFFFFFF;//
    data[15]=0xFFFFFFFF;// ----

    data[16]=0xC0C0C0C1;// ---  magic numbers for MMC RAW mode
    data[17]=0x00000100;// ---

    rb+=  0xFF;
    rb&=(~0xFF);
    data[128]=rb;//0x00001000;// length of program
    data[129]=0x402F1000; // destination address
    fp=fopen("sdraw.bin","wb");
    if(fp==NULL)
    {
        fprintf(stderr,"Error creating file sdraw.bin\n");
        return(1);
    }
    fwrite(data,1,sizeof(data),fp);
    fwrite(myprog,1,rb,fp);
    fclose(fp);
    fprintf(stderr,"Done.\n");
    return(0);
}
