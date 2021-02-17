rem Run this from a command prompt to setup your ps2dev settings
set PS2DEV=R:/_PSP/ps2dev
set PATH=%PATH%;%PS2DEV%/bin;%PS2DEV%/psp/bin;
del *.o
del *.a
make.exe -f makefile
del *.o
copy libz.a ..\libz.a
del libz.a
pause
