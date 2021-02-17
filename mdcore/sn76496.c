/***************************************************************************

  sn76496.c

  Routines to emulate the Texas Instruments SN76489 / SN76496 programmable
  tone /noise generator. Also known as (or at least compatible with) TMS9919.

  Noise emulation is not accurate due to lack of documentation. The noise
  generator uses a shift register with a XOR-feedback network, but the exact
  layout is unknown. It can be set for either period or white noise; again,
  the details are unknown.

***************************************************************************/

///// commented out by starshine
//#include "driver.h"

///// added by starshine
#include "sn76496.h"


#define MAX_OUTPUT 0x7fff
#define AUDIO_CONV(A) (A)

#define STEP 0x10000


/* Formulas for noise generator */
/* bit0 = output */

/* noise feedback for white noise mode */
#define FB_WNOISE 0x12000   /* bit15.d(16bits) = bit0(out) ^ bit2 */
//#define FB_WNOISE 0x14000 /* bit15.d(16bits) = bit0(out) ^ bit1 */
//#define FB_WNOISE 0x28000 /* bit16.d(17bits) = bit0(out) ^ bit2 (same to AY-3-8910) */
//#define FB_WNOISE 0x50000 /* bit17.d(18bits) = bit0(out) ^ bit2 */

/* noise feedback for periodic noise mode */
/* it is correct maybe (it was in the Megadrive sound manual) */
//#define FB_PNOISE 0x10000 /* 16bit rorate */
#define FB_PNOISE 0x08000   /* JH 981127 - fixes Do Run Run */

/* noise generator start preset (for periodic noise) */
#define NG_PRESET 0x0f35


struct SN76496
{
    int Channel;
    int SampleRate;
    unsigned int UpdateStep;
    int VolTable[16];   /* volume table         */
    int Register[8];    /* registers */
    int LastRegister;   /* last register written */
    //int Volume[4];      /* volume of voice 0-2 and noise */
    short Volume[4];      /* volume of voice 0-2 and noise */
    unsigned int RNG;       /* noise generator      */
    int NoiseFB;        /* noise feedback mask */
    int Period[4];
    int Count[4];
    //int Output[4];
    unsigned char Output[4];
} ;


static struct SN76496 sn[MAX_76496];



void SN76496Write(int chip,int data)
{
    struct SN76496 *R = &sn[chip];


    /* update the output buffer before changing the registers */
    ///// commented out by starshine
    //stream_update(R->Channel,0);

    if (data & 0x80)
    {
        int r = (data & 0x70) >> 4;
        int c = r/2;

        R->LastRegister = r;
        R->Register[r] = (R->Register[r] & 0x3f0) | (data & 0x0f);
        switch (r)
        {
            case 0: /* tone 0 : frequency */
            case 2: /* tone 1 : frequency */
            case 4: /* tone 2 : frequency */
                R->Period[c] = R->UpdateStep * R->Register[r];
                if (R->Period[c] == 0) R->Period[c] = R->UpdateStep;
                if (r == 4)
                {
                    /* update noise shift frequency */
                    if ((R->Register[6] & 0x03) == 0x03)
                        R->Period[3] = 2 * R->Period[2];
                }
                break;
            case 1: /* tone 0 : volume */
            case 3: /* tone 1 : volume */
            case 5: /* tone 2 : volume */
            case 7: /* noise  : volume */
                R->Volume[c] = R->VolTable[data & 0x0f];
                break;
            case 6: /* noise  : frequency, mode */
                {
                    int n = R->Register[6];
                    R->NoiseFB = (n & 4) ? FB_WNOISE : FB_PNOISE;
                    n &= 3;
                    /* N/512,N/1024,N/2048,Tone #3 output */
                    R->Period[3] = (n == 3) ? 2 * R->Period[2] : (R->UpdateStep << (5+n));

                    /* reset noise shifter */
                    R->RNG = NG_PRESET;
                    R->Output[3] = R->RNG & 1;
                }
                break;
        }
    }
    else
    {
        int r = R->LastRegister;
        int c = r/2;

        switch (r)
        {
            case 0: /* tone 0 : frequency */
            case 2: /* tone 1 : frequency */
            case 4: /* tone 2 : frequency */
                R->Register[r] = (R->Register[r] & 0x0f) | ((data & 0x3f) << 4);
                R->Period[c] = R->UpdateStep * R->Register[r];
                if (R->Period[c] == 0) R->Period[c] = R->UpdateStep;
                if (r == 4)
                {
                    /* update noise shift frequency */
                    if ((R->Register[6] & 0x03) == 0x03)
                        R->Period[3] = 2 * R->Period[2];
                }
                break;
        }
    }
}


/*
void SN76496_0_w(int offset,int data) { SN76496Write(0,data); }
void SN76496_1_w(int offset,int data) { SN76496Write(1,data); }
void SN76496_2_w(int offset,int data) { SN76496Write(2,data); }
void SN76496_3_w(int offset,int data) { SN76496Write(3,data); }


void SN76496Update_8(int chip,void *buffer,int length)
{
#define DATATYPE unsigned char
#define DATACONV(A) AUDIO_CONV((A) / (STEP * 256))
#include "sn76496u.c"
#undef DATATYPE
#undef DATACONV
}
*/

void SN76496Update_16(int chip,void *buffer,int length)
{
#define DATATYPE unsigned short
#define DATACONV(A) ((A) / STEP)
#include "sn76496u.c"
#undef DATATYPE
#undef DATACONV
}



void SN76496_set_clock(int chip,int clock)
{
    struct SN76496 *R = &sn[chip];


    /* the base clock for the tone generators is the chip clock divided by 16; */
    /* for the noise generator, it is clock / 256. */
    /* Here we calculate the number of steps which happen during one sample */
    /* at the given sample rate. No. of events = sample rate / (clock/16). */
    /* STEP is a multiplier used to turn the fraction into a fixed point */
    /* number. */
#ifdef _PSP10
    R->UpdateStep = 13296 ; // サンプルレート 44100 / clock 3478000 固定
#else //_PSP10
    R->UpdateStep = ((double)STEP * R->SampleRate * 16) / clock;
#endif // _PSP10
}

#ifdef _PSP10
static short gain_table[256][15] =
{
{10922, 8675, 6891, 5474, 4348, 3453, 2743, 2179, 1731, 1375, 1092,  867,  689,  547,  434,},
{10922, 8878, 7052, 5601, 4449, 3534, 2807, 2230, 1771, 1407, 1117,  887,  705,  560,  444,},
{10922, 9084, 7216, 5732, 4553, 3616, 2872, 2281, 1812, 1439, 1143,  908,  721,  573,  455,},
{10922, 9296, 7384, 5865, 4659, 3700, 2939, 2335, 1854, 1473, 1170,  929,  738,  586,  465,},
{10922, 9512, 7556, 6002, 4767, 3787, 3008, 2389, 1898, 1507, 1197,  951,  755,  600,  476,},
{10922, 9734, 7732, 6142, 4878, 3875, 3078, 2445, 1942, 1542, 1225,  973,  773,  614,  487,},
{10922, 9961, 7912, 6285, 4992, 3965, 3150, 2502, 1987, 1578, 1254,  996,  791,  628,  499,},
{10922,10193, 8096, 6431, 5108, 4058, 3223, 2560, 2033, 1615, 1283, 1019,  809,  643,  510,},
{10922,10430, 8285, 6581, 5227, 4152, 3298, 2620, 2081, 1653, 1313, 1043,  828,  658,  522,},
{10922,10673, 8478, 6734, 5349, 4249, 3375, 2681, 2129, 1691, 1343, 1067,  847,  673,  534,},
{10922,10922, 8675, 6891, 5474, 4348, 3453, 2743, 2179, 1731, 1375, 1092,  867,  689,  547,},
{10922,10922, 8878, 7052, 5601, 4449, 3534, 2807, 2230, 1771, 1407, 1117,  887,  705,  560,},
{10922,10922, 9084, 7216, 5732, 4553, 3616, 2872, 2281, 1812, 1439, 1143,  908,  721,  573,},
{10922,10922, 9296, 7384, 5865, 4659, 3700, 2939, 2335, 1854, 1473, 1170,  929,  738,  586,},
{10922,10922, 9512, 7556, 6002, 4767, 3787, 3008, 2389, 1898, 1507, 1197,  951,  755,  600,},
{10922,10922, 9734, 7732, 6142, 4878, 3875, 3078, 2445, 1942, 1542, 1225,  973,  773,  614,},
{10922,10922, 9961, 7912, 6285, 4992, 3965, 3150, 2502, 1987, 1578, 1254,  996,  791,  628,},
{10922,10922,10193, 8096, 6431, 5108, 4058, 3223, 2560, 2033, 1615, 1283, 1019,  809,  643,},
{10922,10922,10430, 8285, 6581, 5227, 4152, 3298, 2620, 2081, 1653, 1313, 1043,  828,  658,},
{10922,10922,10673, 8478, 6734, 5349, 4249, 3375, 2681, 2129, 1691, 1343, 1067,  847,  673,},
{10922,10922,10922, 8675, 6891, 5474, 4348, 3453, 2743, 2179, 1731, 1375, 1092,  867,  689,},
{10922,10922,10922, 8878, 7052, 5601, 4449, 3534, 2807, 2230, 1771, 1407, 1117,  887,  705,},
{10922,10922,10922, 9084, 7216, 5732, 4553, 3616, 2872, 2281, 1812, 1439, 1143,  908,  721,},
{10922,10922,10922, 9296, 7384, 5865, 4659, 3700, 2939, 2335, 1854, 1473, 1170,  929,  738,},
{10922,10922,10922, 9512, 7556, 6002, 4767, 3787, 3008, 2389, 1898, 1507, 1197,  951,  755,},
{10922,10922,10922, 9734, 7732, 6142, 4878, 3875, 3078, 2445, 1942, 1542, 1225,  973,  773,},
{10922,10922,10922, 9961, 7912, 6285, 4992, 3965, 3150, 2502, 1987, 1578, 1254,  996,  791,},
{10922,10922,10922,10193, 8096, 6431, 5108, 4058, 3223, 2560, 2033, 1615, 1283, 1019,  809,},
{10922,10922,10922,10430, 8285, 6581, 5227, 4152, 3298, 2620, 2081, 1653, 1313, 1043,  828,},
{10922,10922,10922,10673, 8478, 6734, 5349, 4249, 3375, 2681, 2129, 1691, 1343, 1067,  847,},
{10922,10922,10922,10922, 8675, 6891, 5474, 4348, 3453, 2743, 2179, 1731, 1375, 1092,  867,},
{10922,10922,10922,10922, 8878, 7052, 5601, 4449, 3534, 2807, 2230, 1771, 1407, 1117,  887,},
{10922,10922,10922,10922, 9084, 7216, 5732, 4553, 3616, 2872, 2281, 1812, 1439, 1143,  908,},
{10922,10922,10922,10922, 9296, 7384, 5865, 4659, 3700, 2939, 2335, 1854, 1473, 1170,  929,},
{10922,10922,10922,10922, 9512, 7556, 6002, 4767, 3787, 3008, 2389, 1898, 1507, 1197,  951,},
{10922,10922,10922,10922, 9734, 7732, 6142, 4878, 3875, 3078, 2445, 1942, 1542, 1225,  973,},
{10922,10922,10922,10922, 9961, 7912, 6285, 4992, 3965, 3150, 2502, 1987, 1578, 1254,  996,},
{10922,10922,10922,10922,10193, 8096, 6431, 5108, 4058, 3223, 2560, 2033, 1615, 1283, 1019,},
{10922,10922,10922,10922,10430, 8285, 6581, 5227, 4152, 3298, 2620, 2081, 1653, 1313, 1043,},
{10922,10922,10922,10922,10673, 8478, 6734, 5349, 4249, 3375, 2681, 2129, 1691, 1343, 1067,},
{10922,10922,10922,10922,10922, 8675, 6891, 5474, 4348, 3453, 2743, 2179, 1731, 1375, 1092,},
{10922,10922,10922,10922,10922, 8878, 7052, 5601, 4449, 3534, 2807, 2230, 1771, 1407, 1117,},
{10922,10922,10922,10922,10922, 9084, 7216, 5732, 4553, 3616, 2872, 2281, 1812, 1439, 1143,},
{10922,10922,10922,10922,10922, 9296, 7384, 5865, 4659, 3700, 2939, 2335, 1854, 1473, 1170,},
{10922,10922,10922,10922,10922, 9512, 7556, 6002, 4767, 3787, 3008, 2389, 1898, 1507, 1197,},
{10922,10922,10922,10922,10922, 9734, 7732, 6142, 4878, 3875, 3078, 2445, 1942, 1542, 1225,},
{10922,10922,10922,10922,10922, 9961, 7912, 6285, 4992, 3965, 3150, 2502, 1987, 1578, 1254,},
{10922,10922,10922,10922,10922,10193, 8096, 6431, 5108, 4058, 3223, 2560, 2033, 1615, 1283,},
{10922,10922,10922,10922,10922,10430, 8285, 6581, 5227, 4152, 3298, 2620, 2081, 1653, 1313,},
{10922,10922,10922,10922,10922,10673, 8478, 6734, 5349, 4249, 3375, 2681, 2129, 1691, 1343,},
{10922,10922,10922,10922,10922,10922, 8675, 6891, 5474, 4348, 3453, 2743, 2179, 1731, 1375,},
{10922,10922,10922,10922,10922,10922, 8878, 7052, 5601, 4449, 3534, 2807, 2230, 1771, 1407,},
{10922,10922,10922,10922,10922,10922, 9084, 7216, 5732, 4553, 3616, 2872, 2281, 1812, 1439,},
{10922,10922,10922,10922,10922,10922, 9296, 7384, 5865, 4659, 3700, 2939, 2335, 1854, 1473,},
{10922,10922,10922,10922,10922,10922, 9512, 7556, 6002, 4767, 3787, 3008, 2389, 1898, 1507,},
{10922,10922,10922,10922,10922,10922, 9734, 7732, 6142, 4878, 3875, 3078, 2445, 1942, 1542,},
{10922,10922,10922,10922,10922,10922, 9961, 7912, 6285, 4992, 3965, 3150, 2502, 1987, 1578,},
{10922,10922,10922,10922,10922,10922,10193, 8096, 6431, 5108, 4058, 3223, 2560, 2033, 1615,},
{10922,10922,10922,10922,10922,10922,10430, 8285, 6581, 5227, 4152, 3298, 2620, 2081, 1653,},
{10922,10922,10922,10922,10922,10922,10673, 8478, 6734, 5349, 4249, 3375, 2681, 2129, 1691,},
{10922,10922,10922,10922,10922,10922,10922, 8675, 6891, 5474, 4348, 3453, 2743, 2179, 1731,},
{10922,10922,10922,10922,10922,10922,10922, 8878, 7052, 5601, 4449, 3534, 2807, 2230, 1771,},
{10922,10922,10922,10922,10922,10922,10922, 9084, 7216, 5732, 4553, 3616, 2872, 2281, 1812,},
{10922,10922,10922,10922,10922,10922,10922, 9296, 7384, 5865, 4659, 3700, 2939, 2335, 1854,},
{10922,10922,10922,10922,10922,10922,10922, 9512, 7556, 6002, 4767, 3787, 3008, 2389, 1898,},
{10922,10922,10922,10922,10922,10922,10922, 9734, 7732, 6142, 4878, 3875, 3078, 2445, 1942,},
{10922,10922,10922,10922,10922,10922,10922, 9961, 7912, 6285, 4992, 3965, 3150, 2502, 1987,},
{10922,10922,10922,10922,10922,10922,10922,10193, 8096, 6431, 5108, 4058, 3223, 2560, 2033,},
{10922,10922,10922,10922,10922,10922,10922,10430, 8285, 6581, 5227, 4152, 3298, 2620, 2081,},
{10922,10922,10922,10922,10922,10922,10922,10673, 8478, 6734, 5349, 4249, 3375, 2681, 2129,},
{10922,10922,10922,10922,10922,10922,10922,10922, 8675, 6891, 5474, 4348, 3453, 2743, 2179,},
{10922,10922,10922,10922,10922,10922,10922,10922, 8878, 7052, 5601, 4449, 3534, 2807, 2230,},
{10922,10922,10922,10922,10922,10922,10922,10922, 9084, 7216, 5732, 4553, 3616, 2872, 2281,},
{10922,10922,10922,10922,10922,10922,10922,10922, 9296, 7384, 5865, 4659, 3700, 2939, 2335,},
{10922,10922,10922,10922,10922,10922,10922,10922, 9512, 7556, 6002, 4767, 3787, 3008, 2389,},
{10922,10922,10922,10922,10922,10922,10922,10922, 9734, 7732, 6142, 4878, 3875, 3078, 2445,},
{10922,10922,10922,10922,10922,10922,10922,10922, 9961, 7912, 6285, 4992, 3965, 3150, 2502,},
{10922,10922,10922,10922,10922,10922,10922,10922,10193, 8096, 6431, 5108, 4058, 3223, 2560,},
{10922,10922,10922,10922,10922,10922,10922,10922,10430, 8285, 6581, 5227, 4152, 3298, 2620,},
{10922,10922,10922,10922,10922,10922,10922,10922,10673, 8478, 6734, 5349, 4249, 3375, 2681,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922, 8675, 6891, 5474, 4348, 3453, 2743,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922, 8878, 7052, 5601, 4449, 3534, 2807,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922, 9084, 7216, 5732, 4553, 3616, 2872,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922, 9296, 7384, 5865, 4659, 3700, 2939,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922, 9512, 7556, 6002, 4767, 3787, 3008,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922, 9734, 7732, 6142, 4878, 3875, 3078,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922, 9961, 7912, 6285, 4992, 3965, 3150,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10193, 8096, 6431, 5108, 4058, 3223,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10430, 8285, 6581, 5227, 4152, 3298,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10673, 8478, 6734, 5349, 4249, 3375,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 8675, 6891, 5474, 4348, 3453,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 8878, 7052, 5601, 4449, 3534,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9084, 7216, 5732, 4553, 3616,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9296, 7384, 5865, 4659, 3700,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9512, 7556, 6002, 4767, 3787,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9734, 7732, 6142, 4878, 3875,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9961, 7912, 6285, 4992, 3965,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10193, 8096, 6431, 5108, 4058,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10430, 8285, 6581, 5227, 4152,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10673, 8478, 6734, 5349, 4249,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 8675, 6891, 5474, 4348,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 8878, 7052, 5601, 4449,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9084, 7216, 5732, 4553,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9296, 7384, 5865, 4659,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9512, 7556, 6002, 4767,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9734, 7732, 6142, 4878,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9961, 7912, 6285, 4992,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10193, 8096, 6431, 5108,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10430, 8285, 6581, 5227,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10673, 8478, 6734, 5349,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 8675, 6891, 5474,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 8878, 7052, 5601,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9084, 7216, 5732,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9296, 7384, 5865,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9512, 7556, 6002,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9734, 7732, 6142,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9961, 7912, 6285,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10193, 8096, 6431,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10430, 8285, 6581,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10673, 8478, 6734,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 8675, 6891,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 8878, 7052,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9084, 7216,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9296, 7384,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9512, 7556,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9734, 7732,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9961, 7912,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10193, 8096,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10430, 8285,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10673, 8478,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 8675,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 8878,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9084,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9296,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9512,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9734,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922, 9961,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10193,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10430,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10673,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,},
{10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,10922,}
} ;
#endif //_PSP10


static void SN76496_set_volume(int chip,int volume,int gain)
{
#ifdef _PSP10
    struct SN76496 *R = &sn[chip];
    int i;

    gain &= 0xff;

    for (i = 0;i < 15;i++)
    {
        R->VolTable[i] = gain_table[gain][i];
    }
    R->VolTable[15] = 0;

#else //_PSP10
    struct SN76496 *R = &sn[chip];
    int i;
    double out;


    ///// commented out by starshine
    //stream_set_volume(R->Channel,volume);

    gain &= 0xff;

    /* increase max output basing on gain (0.2 dB per step) */
    out = MAX_OUTPUT / 3;
    while (gain-- > 0)
        out *= 1.023292992; /* = (10 ^ (0.2/20)) */

    /* build volume table (2dB per step) */
    for (i = 0;i < 15;i++)
    {
        /* limit volume to avoid clipping */
        if (out > MAX_OUTPUT / 3) R->VolTable[i] = MAX_OUTPUT / 3;
        else R->VolTable[i] = out;

        out /= 1.258925412; /* = 10 ^ (2/20) = 2dB */
    }
    R->VolTable[15] = 0;
#endif //_PSP10
}


void SN76496_reset()
{
    int i;
    struct SN76496 *R = &sn[0];

    for (i = 0;i < 4;i++) R->Volume[i] = 0;

    R->LastRegister = 0;
    for (i = 0;i < 8;i+=2)
    {
        R->Register[i] = 0;
        R->Register[i + 1] = 0x0f;  /* volume = 0 */
    }

    for (i = 0;i < 4;i++)
    {
        R->Output[i] = 0;
        R->Period[i] = R->Count[i] = R->UpdateStep;
    }
    R->RNG = NG_PRESET;
    R->Output[3] = R->RNG & 1;
}

int SN76496_init(int chip,int clock,int sample_rate,int sample_bits)
{
    int i;
    struct SN76496 *R = &sn[chip];
    char name[40];

    ////// commented out by starshine
    //sprintf(name,"SN76496 #%d",chip);
    //R->Channel = stream_init(msound,
    //        name,sample_rate,sample_bits,
    //        chip,(sample_bits == 16) ? SN76496Update_16 : SN76496Update_8);
    
    if (R->Channel == -1)
        return 1;

    R->SampleRate = sample_rate;
    SN76496_set_clock(chip,clock);
    SN76496_set_volume(chip,255,0);

#if 0
    for (i = 0;i < 4;i++) R->Volume[i] = 0;

    R->LastRegister = 0;
    for (i = 0;i < 8;i+=2)
    {
        R->Register[i] = 0;
        R->Register[i + 1] = 0x0f;  /* volume = 0 */
    }

    for (i = 0;i < 4;i++)
    {
        R->Output[i] = 0;
        R->Period[i] = R->Count[i] = R->UpdateStep;
    }
    R->RNG = NG_PRESET;
    R->Output[3] = R->RNG & 1;
#else
	SN76496_reset() ;
#endif // 0

    return 0;
}


int SN76496_sh_start()
{
    ///// total commenting out by starshine
    //int chip;
    //const struct SN76496interface *intf = msound->sound_interface;


    //for (chip = 0;chip < intf->num;chip++)
    //{
    //    if (SN76496_init(msound,chip,intf->baseclock,Machine->sample_rate,Machine->sample_bits) != 0)
    //        return 1;

    //    SN76496_set_volume(chip,intf->volume[chip] & 0xff,(intf->volume[chip] >> 8) & 0xff);
    //}
    return 0;
}

int SN76496_get_state( unsigned char* buff )
{
	memcpy( buff, sn[0].Register , sizeof(int) * 8 ) ;
	buff += sizeof(int) * 8 ;
	return sizeof(int) * 8 ;
}

int SN76496_set_state( unsigned char* buff, int buff_len )
{
	int n, m ;
	int Register[8]; 

	if( sizeof(int) * 8 <= buff_len )
	{
		SN76496_reset() ;

		memcpy( Register, buff, sizeof(int) * 8 ) ;
		buff += sizeof(int) * 8 ;

		for( m=0; m < 8 ; m++ )
		{
			SN76496Write( 0, 0x80 | (m << 4) | Register[m] & 0x0F ) ;
			SN76496Write( 0, (Register[m] >> 4) & 0x3F ) ;
		}

		return sizeof(int) * 8 ;
	}

	return 0 ;
}
