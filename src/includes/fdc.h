/*
  Hatari
*/

/*-----------------------------------------------------------------------*/
/* FDC Emulation commands */
enum {
  FDCEMU_CMD_NULL=0,
  // Type I
  FDCEMU_CMD_RESTORE,
  FDCEMU_CMD_SEEK,
  FDCEMU_CMD_STEP,
  FDCEMU_CMD_STEPIN,
  FDCEMU_CMD_STEPOUT,
  // Type II
  FDCEMU_CMD_READSECTORS,
  FDCEMU_CMD_READMULTIPLESECTORS,
  FDCEMU_CMD_WRITESECTORS,
  FDCEMU_CMD_WRITEMULTIPLESECTORS
};

/* FDC Emulation commands */
#define  FDCEMU_RUN_NULL      0

/* FDC Running Restore commands */
enum {
  FDCEMU_RUN_RESTORE_SEEKTOTRACKZERO,
  FDCEMU_RUN_RESTORE_COMPLETE
};

/* FDC Running Seek commands */
enum {
  FDCEMU_RUN_SEEK_TOTRACK,
  FDCEMU_RUN_SEEK_COMPLETE
};

/* FDC Running Step commands */
enum {
  FDCEMU_RUN_STEP_ONCE,
  FDCEMU_RUN_STEP_COMPLETE
};

/* FDC Running Step In commands */
enum {
  FDCEMU_RUN_STEPIN_ONCE,
  FDCEMU_RUN_STEPIN_COMPLETE
};

/* FDC Running Step Out commands */
enum {
  FDCEMU_RUN_STEPOUT_ONCE,
  FDCEMU_RUN_STEPOUT_COMPLETE
};

/* FDC Running Read Sector/s commands */
enum {
  FDCEMU_RUN_READSECTORS_READDATA,
  FDCEMU_RUN_READSECTORS_COMPLETE
};

/* FDC Running write Sector/s commands */
enum {
  FDCEMU_RUN_WRITESECTORS_WRITEDATA,
  FDCEMU_RUN_WRITESECTORS_COMPLETE
};

extern void FDC_Reset(void);
extern void FDC_MemorySnapShot_Capture(BOOL bSave);
extern void FDC_ResetDMAStatus(void);
extern void FDC_SetDMAStatus(BOOL bError);
extern long FDC_ReadDMAStatus(void);
extern int FDC_FindFloppyDrive(void);
extern void FDC_AcknowledgeInterrupt(void);
extern void FDC_UpdateHBL(void);
extern void FDC_UpdateRestoreCmd(void);
extern void FDC_UpdateSeekCmd(void);
extern void FDC_UpdateStepCmd(void);
extern void FDC_UpdateStepInCmd(void);
extern void FDC_UpdateStepOutCmd(void);
extern void FDC_UpdateReadSectorsCmd(void);
extern void FDC_UpdateWriteSectorsCmd(void);
extern unsigned long FDC_ReadDMAAddress(void);
extern void FDC_WriteDMAAddress(unsigned long Address);
extern BOOL FDC_ReadSectorFromFloppy(void);
extern BOOL FDC_WriteSectorFromFloppy(void);
extern void FDC_DMADataFromFloppy(void);
extern void FDC_WriteDiscController(unsigned short v);
extern short FDC_ReadDiscControllerStatus(void);
extern void FDC_WriteDMAModeControl(unsigned short v);
