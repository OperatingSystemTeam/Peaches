# commands/Makefile

#ENTRYPOINT	= 0x1000
HD		= ../80m.img

ASM		= sudo nasm
DASM		= objdump
CC		= sudo gcc -m32
LD		= sudo ld -m elf_i386
ASMFLAGS	= -I ../include/ -f elf
CFLAGS		= -I ../include/ -c -fno-builtin -fno-stack-protector -Wall
LDFLAGS		= -Ttext 0x1000
DASMFLAGS	= -D
LIB		= ../lib/orangescrt.a
BIN		= echo cd create rm mkdir read write ls pwd

# All Phony Targets
.PHONY : everything final clean realclean disasm all install

# Default starting position
everything : $(BIN)

install : all clean
	cp ../kernel.bin ./ -v
	tar vcf inst.tar kernel.bin $(BIN)
	dd if=inst.tar of=$(HD) seek=`echo "obase=10;ibase=16;(\`egrep -e '^ROOT_BASE' ../boot/include/load.inc | sed -e 's/.*0x//g'\`+\`egrep -e '#define[[:space:]]*INSTALL_START_SECT' ../include/sys/config.h | sed -e 's/.*0x//g'\`)*200" | bc` bs=1 count=`ls -l inst.tar | awk -F " " '{print $$5}'` conv=notrunc

all : realclean everything

final : all clean

clean :
	rm -f *.o

realclean :
	rm -f $(BIN) *.o

kernel.bin :
	cp ../kernel.bin ./

start.o : start.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

echo.o: echo.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

echo : echo.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

	
cd.o: cd.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

cd : cd.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

rm.o: rm.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

rm : rm.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

write.o: write.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

write : write.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?
	$(CC) $(CFLAGS) -o $@ $<


read.o: read.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<


read : read.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

mkdir.o:mkdir.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

mkdir : mkdir.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

create.o: create.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

create : create.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

ls.o: ls.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

ls : ls.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?



pwd.o: pwd.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

pwd : pwd.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?
