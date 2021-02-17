/*
  File: fm.h -- header file for software emuration for FM sound genelator

*/
#ifndef _H_FM_FM_
#define _H_FM_FM_

////// starshine - one chip 4 me!
#define HAS_YM2612 1
#define BUILD_YM2612 (HAS_YM2612)       // build YM2612 emulator

#define BUILD_YM2203 (HAS_YM2203)       /* build YM2203(OPN)   emulator */
#define BUILD_YM2608 (HAS_YM2608)       /* build YM2608(OPNA)  emulator */
#define BUILD_YM2610 (HAS_YM2610)       /* build YM2610(OPNB)  emulator */
#define BUILD_YM2610B (HAS_YM2610B)     /* build YM2610B(OPNB?)emulator */
#define BUILD_YM2151 (HAS_YM2151)       /* build YM2151(OPM)   emulator */

//#define HAS_IRQ_CALLBACK 

/* stereo mixing / separate */
//#define FM_STEREO_MIX
/* output bit size 8 or 16 */
#define FM_SAMPLE_BITS 16

#define YM2203_NUMBUF 1

#ifdef FM_STEREO_MIX
  #define YM2151_NUMBUF 1
  #define YM2608_NUMBUF 1
  #define YM2612_NUMBUF 1
  #define YM2610_NUMBUF  1
#else
  #define YM2151_NUMBUF 2    /* FM L+R */
  #define YM2608_NUMBUF 2    /* FM L+R+ADPCM+RYTHM */
  #define YM2612_NUMBUF 2    /* FM L+R */
  #define YM2610_NUMBUF  2
#endif

#if (FM_SAMPLE_BITS==16)
typedef unsigned short FMSAMPLE;
typedef unsigned long FMSAMPLE_MIX;
#endif
#if (FM_SAMPLE_BITS==8)
typedef unsigned char  FMSAMPLE;
typedef unsigned short FMSAMPLE_MIX;
#endif

/* For YM2151/YM2608/YM2612 option */

struct ssg_callbacks
{
	void (*set_clock)(void *param, int clock);
	void (*write)(void *param, int address, int data);
	int (*read)(void *param);
	void (*reset)(void *param);
};

typedef void (*FM_TIMERHANDLER)(void *param,int c,int cnt,double stepTime);
typedef void (*FM_IRQHANDLER)(void *param,int irq);
/* FM_TIMERHANDLER : Stop or Start timer         */
/* int n          = chip number                  */
/* int c          = Channel 0=TimerA,1=TimerB    */
/* int count      = timer count (0=stop)         */
/* doube stepTime = step time of one count (sec.)*/

/* FM_IRQHHANDLER : IRQ level changing sense     */
/* int n       = chip number                     */
/* int irq     = IRQ level 0=OFF,1=ON            */

#ifdef BUILD_OPN
/* -------------------- YM2203/YM2608 Interface -------------------- */
/*
** 'n' : YM2203 chip number 'n'
** 'r' : register
** 'v' : value
*/
//void OPNWriteReg(int n, int r, int v);

/*
** read status  YM2203 chip number 'n'
*/
unsigned char OPNReadStatus(int n);

#endif /* BUILD_OPN */

#if BUILD_YM2203
/* -------------------- YM2203(OPN) Interface -------------------- */

/*
** Initialize YM2203 emulator(s).
**
** 'num'     is the number of virtual YM2203's to allocate
** 'rate'    is sampling rate
** return    0 = success
*/
int YM2203Init(int num, int baseclock, int rate,
               FM_TIMERHANDLER TimerHandler,FM_IRQHANDLER IRQHandler);

/*
** shutdown the YM2203 emulators .. make sure that no sound system stuff
** is touching our audio buffers ...
*/
void YM2203Shutdown(void);

/*
** reset all chip registers for YM2203 number 'num'
*/
void YM2203ResetChip(int num);

void YM2203UpdateOne(int num, void *buffer, int length);

/*
** return : InterruptLevel
*/
int YM2203Write(int n,int a,int v);
unsigned char YM2203Read(int n,int a);

/*
**  Timer OverFlow
*/
int YM2203TimerOver(int n, int c);

/*int YM2203SetBuffer(int n, FMSAMPLE *buf );*/

#endif /* BUILD_YM2203 */


#if BUILD_YM2612

void * YM2612Init(int index, int baseclock, int rate,
               FM_TIMERHANDLER TimerHandler,FM_IRQHANDLER IRQHandler);
void YM2612Shutdown(void *chip);
void YM2612ResetChip(void);
//void YM2612UpdateOne( FMSAMPLE **buffer, int length, int flags );
void YM2612UpdateOne( FMSAMPLE **buffer, int length );

int YM2612Write(int a,unsigned char v);
unsigned char YM2612Read(int a);

int YM2612TimerOver(void *chip, int c );
void YM2612Postload(void *chip);

int YM2612GetState( unsigned char* buff ) ;
int YM2612SetState( unsigned char* buff, int len ) ;
int YM2612SetRegs ( signed short* regs ) ;

#endif /* BUILD_YM2612 */


#endif /* _H_FM_FM_ */
