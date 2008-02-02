/*
  Hatari - video.h

  This file is distributed under the GNU Public License, version 2 or at
  your option any later version. Read the file gpl.txt for details.
*/

#ifndef HATARI_VIDEO_H
#define HATARI_VIDEO_H

/*
  All the following processor timings are based on a bog standard 8MHz 68000 as
  found in all standard STs:

  Clock cycles per line (50Hz)      : 512
  NOPs per scan line (50Hz)         : 128
  Scan lines per VBL (50Hz)         : 313 (64 at top,200 screen,49 bottom)

  Clock cycles per line (60Hz)      : 508
  NOPs per scan line (60Hz)         : 127
  Scan lines per VBL (60Hz)         : 263

  Clock cycles per VBL (50Hz)       : 160256
  NOPs per VBL (50Hz)               : 40064

  Pixels per clock cycle (low res)  : 1
  Pixels per clock cycle (med res)  : 2
  Pixels per clock cycle (high res) : 4
  Pixels per NOP (low res)          : 4
  Pixels per NOP (med res)          : 8
  Pixels per NOP (high res)         : 16
*/

/* Scan lines per frame */
#define SCANLINES_PER_FRAME_50HZ 313    /* Number of scan lines per frame in 50 Hz */
#define SCANLINES_PER_FRAME_60HZ 263    /* Number of scan lines per frame in 60 Hz */
#define SCANLINES_PER_FRAME_71HZ 501    /* could also be 500 ? */
#define MAX_SCANLINES_PER_FRAME  313    /* Max. number of scan lines per frame */

/* Cycles per line */
#define CYCLES_PER_LINE_50HZ  512
#define CYCLES_PER_LINE_60HZ  508
#define CYCLES_PER_LINE_71HZ  224

/* Vertical border/display enable/disable:
 * Normal screen starts 63 lines in, top border is 29 lines */
#define SCREEN_START_HBL_50HZ   63      /* Usually the first line of the displayed screen in 50 Hz */
#define SCREEN_START_HBL_60HZ   34      /* The first line of the displayed screen in 60 Hz */
#define SCREEN_START_HBL_71HZ   34      /* FIXME: 34 is not verified */
#define FIRST_VISIBLE_HBL_50HZ  34      /* At this line we start rendering our screen in 50 Hz */
#define FIRST_VISIBLE_HBL_60HZ  (34-29) /* At this line we start rendering our screen in 60 Hz (29 = 63-34) */
#define FIRST_VISIBLE_HBL_71HZ  34      /* FIXME: 34 is not verified */

#define SCREEN_HEIGHT_HBL_COLOR  200    /* This is usually the height of the screen */
#define SCREEN_HEIGHT_HBL_MONO   400

#define SCREEN_END_HBL_50HZ	( SCREEN_START_HBL_50HZ + SCREEN_HEIGHT_HBL_COLOR )	/* 263 */
#define SCREEN_END_HBL_60HZ	( SCREEN_START_HBL_60HZ + SCREEN_HEIGHT_HBL_COLOR )	/* 234 */

/* FIXME: SCREEN_START_CYCLE should rather be 52 or so, but this breaks a lot of other things at the moment... */
#define SCREEN_START_CYCLE  56          /* Cycle first normal pixel appears on */

#define LINE_REMOVE_TOP_CYCLE    504    /* switch to 60 Hz on line 33 should not occur after cycle 504 to remove top border */
                                        /* switch to 50 Hz should occur after cycle 504 on line 33 */
#define LINE_REMOVE_BOTTOM_CYCLE 504    /* same value than top border, but on line 262 (50 Hz) or 233 (60 Hz) */


#define LINE_START_CYCLE_50	56
#define LINE_START_CYCLE_60	52
#define LINE_START_CYCLE_70	0
#define LINE_END_CYCLE_50	376
#define LINE_END_CYCLE_60	372
#define LINE_END_CYCLE_70	160
#define LINE_END_CYCLE_NO_RIGHT	460
#define LINE_END_CYCLE_50_2	(LINE_END_CYCLE_50+44*2)	/* used in enchanted lands */
#define LINE_END_CYCLE_FULL	512				/* used in enchanted lands */
#define LINE_SCROLL_13_CYCLE_50	20	/* 13 pixels right "hardware" scrolling */
#define LINE_SCROLL_9_CYCLE_50	24	/*  9 pixels right "hardware" scrolling */
#define LINE_SCROLL_5_CYCLE_50	28	/*  5 pixels right "hardware" scrolling */
#define LINE_SCROLL_1_CYCLE_50	32	/*  1 pixels right "hardware" scrolling */
#define LINE_LEFT_MID_CYCLE_1	20	/* mid res overscan, shifts display by 0 byte */
#define LINE_LEFT_MID_CYCLE_2	28	/* mid res overscan, shifts display by 2 bytes */
#define	LINE_EMPTY_CYCLE_70	28	/* switch to hi/lo will create an empty line */

/* Bytes for opened left and right border: */
#define BORDERBYTES_NORMAL	160	/* size of a "normal" line */
#define BORDERBYTES_LEFT	26
#define BORDERBYTES_RIGHT	44
#define BORDERBYTES_RIGHT_FULL	22

/* Legacy defines: */
#define CYCLES_PER_FRAME    (nScanlinesPerFrame*nCyclesPerLine)  /* Cycles per VBL @ 50fps = 160256 */


#define VBL_VIDEO_CYCLE_OFFSET		(60+4)
#define HBL_VIDEO_CYCLE_OFFSET		(12-12)			/* cycles after end of current line */
#define TIMERB_VIDEO_CYCLE_OFFSET	(96+12)			/* cycles before end of current line (28 cycles after display off) */
#define RESTART_VIDEO_COUNTER_CYCLE	( (MAX_SCANLINES_PER_FRAME-3) * CYCLES_PER_LINE_50HZ + 48 )


extern int STRes;
extern int TTRes;
extern BOOL bUseSTShifter;
extern BOOL bUseHighRes;
extern int nVBLs;
extern int nHBL;
extern int nStartHBL;
extern int OverscanMode;
extern Uint16 HBLPalettes[];
extern Uint16 *pHBLPalettes;
extern Uint32 HBLPaletteMasks[];
extern Uint32 *pHBLPaletteMasks;
extern Uint32 VideoBase;
extern int nScreenRefreshRate;

extern int nScanlinesPerFrame;
extern int nCyclesPerLine;


extern void Video_Reset(void);
extern void Video_MemorySnapShot_Capture(BOOL bSave);
extern void Video_GetTTRes(int *width, int *height, int *bpp);
extern void Video_StartInterrupts(void);
extern void Video_InterruptHandler_VBL(void);
extern void Video_InterruptHandler_EndLine(void);
extern void Video_InterruptHandler_HBL(void);
extern void Video_SetScreenRasters(void);

extern void Video_ScreenCounterHigh_ReadByte(void);
extern void Video_ScreenCounterMed_ReadByte(void);
extern void Video_ScreenCounterLow_ReadByte(void);
extern void Video_Sync_ReadByte(void);
extern void Video_BaseLow_ReadByte(void);
extern void Video_LineWidth_ReadByte(void);
extern void Video_ShifterMode_ReadByte(void);
extern void Video_HorScroll_Read(void);

extern void Video_ScreenBaseSTE_WriteByte(void);
extern void Video_ScreenCounter_WriteByte(void);
extern void Video_Sync_WriteByte(void);
extern void Video_LineWidth_WriteByte(void);
extern void Video_Color0_WriteWord(void);
extern void Video_Color1_WriteWord(void);
extern void Video_Color2_WriteWord(void);
extern void Video_Color3_WriteWord(void);
extern void Video_Color4_WriteWord(void);
extern void Video_Color5_WriteWord(void);
extern void Video_Color6_WriteWord(void);
extern void Video_Color7_WriteWord(void);
extern void Video_Color8_WriteWord(void);
extern void Video_Color9_WriteWord(void);
extern void Video_Color10_WriteWord(void);
extern void Video_Color11_WriteWord(void);
extern void Video_Color12_WriteWord(void);
extern void Video_Color13_WriteWord(void);
extern void Video_Color14_WriteWord(void);
extern void Video_Color15_WriteWord(void);
extern void Video_ShifterMode_WriteByte(void);
extern void Video_HorScroll_Write(void);
extern void Video_TTShiftMode_WriteWord(void);
extern void Video_TTColorRegs_WriteWord(void);
extern void Video_TTColorSTRegs_WriteWord(void);

#endif  /* HATARI_VIDEO_H */
