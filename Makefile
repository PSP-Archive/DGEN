##################################################################################
###  DGEN for PSP                                                       makefile
##################################################################################

#NON_KERNEL = 1
#USE_PSPSDK = 1
#USE_ME     = 1
#USE_ASM    = 1

ifdef NON_KERNEL
TITLE = "DGEN for PSP Ver1.70 Lite"
else
TITLE = "DGEN for PSP Ver1.70"
endif 

BINARY   = dgen.out
BINARY_P = dgen.outp
MAPFILE  = dgen.map
ICON0    = ICON0.PNG

CC     = psp-gcc
CCC    = psp-g++
LINKER = psp-gcc
#LINKER = psp-ld
STRIP  = psp-strip

OUTPATCH = ./outpatch.exe
ELF2PBP  = ./elf2pbp.exe

AS_OPT   = -mgp32 -mlong32 -msingle-float -mabi=eabi -c
CC_OPT   = -fomit-frame-pointer -fno-exceptions -mabi=eabi -mgp32 -mlong32 -c
CCC_OPT  = -fomit-frame-pointer -fno-exceptions -mabi=eabi -mgp32 -mlong32 -c -fno-rtti 
LINK_OPT = -nostartfiles -Wl,-Ttext=0x08900000
#LINK_OPT = -nostartfiles -Ttext=0x08900000 -M
DEFINES  = -D_PSP10 -D_USE_ZLIB -D_SCE_IO ##-D_USE_ASM
MDCORE   = 1

ifdef NON_KERNEL
DEFINES += -D_NON_KERNEL
endif

ifdef USE_PSPSDK
OBJECTS  = 
else
OBJECTS  = psp/startup.o
endif

OBJECTS += psp/psp_main.o

ifdef USE_ME
DEFINES += -D_USE_ME
OBJECTS += psp/psp_me.o psp/mestub.o
endif

OBJECTS += psp/psp_std.o psp/psp_screen.o
#OBJECTS += psp/psp_screen.o

ifdef MDCORE ##########################################################

#USE_MUSA   = 1
USE_C68K   = 1
USE_MUSA33 = 1

#USE_MZ80 = 1
USE_CZ80 = 1
#USE_PZ80 = 1

ifdef USE_MUSA
DEFINES += -D_USE_68K=1
endif 

ifdef USE_C68K
ifdef USE_MUSA33
DEFINES += -D_USE_68K=3
else
DEFINES += -D_USE_68K=2
endif
else
ifdef USE_MUSA33
DEFINES += -D_USE_68K=1
endif 
endif

ifdef USE_MZ80
DEFINES += -D_USE_Z80=1
endif
ifdef USE_CZ80
DEFINES += -D_USE_Z80=2
endif
ifdef USE_PZ80
DEFINES += -D_USE_Z80=3
endif

DEFINES += -D_USE_MD
OBJECTS += psp/mdcore_main.o psp/emu_core.o mdcore/md.o
OBJECTS += mdcore/ras.o mdcore/vdp.o

ifdef USE_ASM
DEFINES += -D_USE_ASM
OBJECTS += mdcore/ras_asm.a 
endif

ifdef USE_MUSA
OBJECTS += mdcore/musa/m68kcpu.o mdcore/musa/m68kopac.o mdcore/musa/m68kopdm.o\
		   mdcore/musa/m68kops.o mdcore/musa/m68kopnz.o
endif 

ifdef USE_C68K
OBJECTS += mdcore/c68k/c68k.o mdcore/c68k/c68kexec.o
endif

ifdef USE_MUSA33
OBJECTS += mdcore/m68k/m68kcpu.o mdcore/m68k/m68kopac.o mdcore/m68k/m68kopdm.o\
		   mdcore/m68k/m68kops.o mdcore/m68k/m68kopnz.o
endif 

OBJECTS += mdcore/fm.o mdcore/sn76496.o
#OBJECTS += mdcore/fm_vfpu.a mdcore/sn76496.o

ifdef USE_MZ80
OBJECTS += mdcore/zz80.o
endif
ifdef USE_CZ80
OBJECTS += mdcore/cz80/cz80.o
endif
ifdef USE_PZ80
OBJECTS += mdcore/pz80/z80.o
endif 

OBJECTS += mdcore/romload.o


endif  ### MDCORE #################################################################

ifndef NON_KERNEL
OBJECTS += psp/psp_comm.o psp/psp_adhoc.o
endif

OBJECTS += psp/psp_menu.o psp/psp_bitmap.o psp/psp_cfg_file.o\
		   psp/emu_config.o psp/emu_state.o psp/emu_cheat.o
		   #psp/psp_irda.o

LIBRARY = lib/libz.a lib/libpng.a lib/unziplib.a -lc

##################################################################
ifdef USE_PSPSDK
##################################################################

#USE_PSPSDK_LIBC = 1

DEFINES += -D_USE_PSPSDK

TARGET = dgen_psp
OBJS = $(OBJECTS) psp/adhoc_stub.o

CC_OPT   = -O3 -fomit-frame-pointer -mgp32 -mlong32

INCDIR = 
CFLAGS   = $(CC_OPT) $(DEFINES)
CXXFLAGS = $(CCC_OPT) $(DEFINES)
ASFLAGS  = $(AS_OPT) $(DEFINES)
#CFLAGS   = -O2 -G0 -Wall $(DEFINES)
#CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti $(DEFINES)
#ASFLAGS  = $(CFLAGS)

LIBDIR  =
LDFLAGS = 
#LIBS    = $(LIBRARY) -lpspaudio -lpsppower -lpsprtc -lpspwlan -lpspnet
LIBS    = lib/libz.a lib/libpng.a lib/unziplib.a -lpspaudio -lpsppower -lpsprtc -lpspwlan -lpspnet

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = $(TITLE)
PSP_EBOOT_ICON  = ICON0.PNG

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

##################################################################
else ### USE_PSPSDK ##############################################
##################################################################

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(LINKER) $(OBJECTS) $(LIBRARY) $(LINK_OPT) -o $@ > $(MAPFILE)
	$(STRIP) $(BINARY)
	$(OUTPATCH) $(BINARY) $(BINARY_P) "USERPROG"
	$(ELF2PBP) $(BINARY_P) $(TITLE) $(ICON0)

psp/psp_adhoc.o : psp/psp_adhoc.c
	$(CC) -G0 -O3 $(CC_OPT) $(DEFINES) $< -o $@

mdcore/fm.o : mdcore/fm.c
	$(CC) -O3 $(CC_OPT) $(DEFINES) $< -o $@

%.o : %.c
	$(CC) -O3 $(CC_OPT) $(DEFINES) $< -o $@

%.o : %.cpp
	$(CCC) -O3 $(CCC_OPT) $(DEFINES) $< -o $@

%.o : %.S
	$(CC) $(AS_OPT) $(DEFINES) $< -o $@	

clean:
	del /s /f psp/*.o

##################################################################
endif ### USE_PSPSDK #############################################
##################################################################

