rem Run this from a command prompt to setup your ps2dev settings
rem set PS2DEV=C:/CMPL/ps2dev
set PS2DEV=R:/_PSP/ps2dev
set PS2SDK=%PS2DEV%/ps2sdk
set PS2SDKSRC=%PS2DEV%/ps2sdksrc
rem set PATH=%PATH%;%PS2DEV%/bin;%PS2DEV%/psp/bin;%PS2DEV%/ee/bin;%PS2DEV%/iop/bin;%PS2DEV%/dvp/bin;%PS2SDK%/bin;
set PATH=%PATH%;%PS2DEV%/bin;%PS2DEV%/psp/bin;
del *.o
del *.a
make.exe -f makefile
del *.o
pause
