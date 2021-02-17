////////////////////////////////////////
#ifndef _PSP_IRDA_H
#define _PSP_IRDA_H
////////////////////////////////////////

int PSP_IrDA_Open   () ;
int PSP_IrDA_Close  () ;
int PSP_IrDA_IsOpen () ;
int PSP_IrDA_Write  ( void* pData, int nLen ) ;
int PSP_IrDA_Read   ( void* pData, int nLen ) ;
int PSP_IrDA_ReadEx ( void* pData, int nLen, unsigned long key ) ;

////////////////////////////////////////
#endif // _PSP_IRDA_H
////////////////////////////////////////
