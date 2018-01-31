
ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi

AOPS = --warn --fatal-warnings
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding

all : sdraw.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.srec
	rm -f *.elf
	rm -f *.list
	rm -f mkraw

mkraw : mkraw.c
	gcc mkraw.c -o mkraw

vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) vectors.s -o vectors.o

notmain.o : notmain.c
	$(ARMGNU)-gcc $(COPS) -c notmain.c -o notmain.o

notmain.elf : memmap vectors.o notmain.o
	$(ARMGNU)-ld vectors.o notmain.o -T memmap -o notmain.elf
	$(ARMGNU)-objdump -D notmain.elf > notmain.list

notmain.bin : notmain.elf
	$(ARMGNU)-objcopy --srec-forceS3 notmain.elf -O srec notmain.srec
	$(ARMGNU)-objcopy notmain.elf -O binary notmain.bin

sdraw.bin : mkraw notmain.bin
	./mkraw

