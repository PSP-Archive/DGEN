////////////////////////////////////////////////////////////
/// psp_adhoc.h
////////////////////////////////////////////////////////////
#ifndef _PSP_ADHOC_H
#define _PSP_ADHOC_H
////////////////////////////////////////////////////////////

enum
{ 
	PSP_ADHOC_ERROR_OK =   0 ,
	PSP_ADHOC_ERROR_NG =  -1 ,
	PSP_ADHOC_CANCEL   =  -2 ,
	PSP_ADHOC_TIMEOUT  =  -3 ,
	PSP_ADHOC_NOTLOAD  = -99 
} ;

int PSP_Adhoc_Init            ( ) ;
int PSP_Adhoc_Term            ( ) ;

int PSP_Adhoc_IsLoad          ( ) ;
int PSP_Adhoc_IsConnect       ( ) ;

int PSP_Adhoc_Create          ( char *MatchingData ) ;
int PSP_Adhoc_Reconnect       ( char *ssid         ) ;
int PSP_Adhoc_Connect         ( ) ;

int PSP_Adhoc_SendRecvAck     ( void *buffer, int  length ) ;
int PSP_Adhoc_RecvSendAck     ( void *buffer, int *length ) ;

int PSP_Adhoc_Send            ( void *buffer, int  length ) ;
int PSP_Adhoc_Recv            ( void *buffer, int *length ) ;
int PSP_Adhoc_RecvBlocked     ( void *buffer, int *length, unsigned long timeout ) ;

void PSP_Adhoc_SetDelay       ( unsigned long delay ) ;


////////////////////////////////////////////////////////////
#endif //_PSP_ADHOC_H
////////////////////////////////////////////////////////////
