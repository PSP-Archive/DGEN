rem Run this from a command prompt to setup your ps2dev settings
rem set PS2DEV=C:/CMPL/ps2dev
set PS2DEV=R:/_PSP/ps2dev
rem set PS2DEV=R:/_PSP/pspdev
set PATH=%PATH%;%PS2DEV%/bin;%PS2DEV%/psp/bin;
del *.o
del gbcore\*.o
del mdcore\*.o
del pcecore\*.o
del pce\*.o
del psp\*.o
del *.out
del *.outp
del *.elf
del eboot.pbp
make.exe -f makefile
del *.o
del gbcore\*.o
del mdcore\*.o
del pcecore\*.o
del pce\*.o
del psp\*.o
pause
