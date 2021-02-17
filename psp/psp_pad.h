
////////////////////////////////////////
#ifndef _PSP_PAD_H
#define _PSP_PAD_H
////////////////////////////////////////

/* Button bit masks */ 
#define CTRL_SQUARE     0x8000 
#define CTRL_TRIANGLE   0x1000 
#define CTRL_CIRCLE     0x2000 
#define CTRL_CROSS      0x4000 
#define CTRL_UP         0x0010 
#define CTRL_DOWN		0x0040 
#define CTRL_LEFT		0x0080 
#define CTRL_RIGHT      0x0020 
#define CTRL_START      0x0008 
#define CTRL_SELECT     0x0001 
#define CTRL_LTRIGGER   0x0100 
#define CTRL_RTRIGGER   0x0200 
#define	CTRL_HOLD		0x00020000

#define CTRL_ANALOG_X   0
#define CTRL_ANALOG_Y   1

#define LOWER_THRESHOLD  (0x2F)
#define UPPER_THRESHOLD  (0xCF)
#define LOWER_THRESHOLD2 (0x0F)
#define UPPER_THRESHOLD2 (0xF0)

typedef struct _PSP_PAD_DATA 
{ 
   unsigned long frame     ; 
   unsigned long buttons   ; 
   unsigned char analog[4] ; 
   unsigned long unused    ; 
} PSP_PAD_DATA ;

void          PSP_Ctrlpad_Update       ( void ) ;
unsigned long PSP_Ctrlpad_GetButtons   ( void ) ;
PSP_PAD_DATA* PSP_Ctrlpad_GetPadData   ( void ) ;

////////////////////////////////////////
#endif // _PSP_PAD_H
////////////////////////////////////////
