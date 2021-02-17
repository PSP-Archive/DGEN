BINARY = out

OBJECTS = adler32.o crc32.o infback.o inffast.o inflate.o inftrees.o\
	ioapi.o uncompr.o unzip.o zlibFileMemory.o zlibInterface.o\
	zutil.o

all: $(BINARY)

$(BINARY): $(OBJECTS)
#	ee-ar rcs unziplib.a $(OBJECTS)
	psp-ar rcs unziplib.a $(OBJECTS)


%.o : %.c
#	ee-gcc -march=r4000 -O3 -fomit-frame-pointer -g -mgp32 -mlong32 -c $< -o $@
	psp-gcc -O3 -fomit-frame-pointer -fno-exceptions -mgp32 -mlong32 -mabi=eabi -c $< -o $@

clean:
	del /s /f *.o *.map
