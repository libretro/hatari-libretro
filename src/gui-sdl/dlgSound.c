/*
  Hatari - dlgSound.c

  This file is distributed under the GNU Public License, version 2 or at
  your option any later version. Read the file gpl.txt for details.
*/
const char DlgSound_rcsid[] = "Hatari $Id: dlgSound.c,v 1.11 2008-06-08 16:07:42 eerot Exp $";

#include "main.h"
#include "configuration.h"
#include "dialog.h"
#include "sdlgui.h"
#include "file.h"
#include "sound.h"


#define DLGSOUND_ENABLE     3
#define DLGSOUND_LOW        5
#define DLGSOUND_MEDIUM     6
#define DLGSOUND_HIGH       7
#define DLGSOUND_RECNAME    11
#define DLGSOUND_RECBROWSE  12
#define DLGSOUND_RECORD     13
#define DLGSOUND_EXIT       14


static char dlgRecordName[35];


/* The sound dialog: */
static SGOBJ sounddlg[] =
{
	{ SGBOX, 0, 0, 0,0, 38,24, NULL },
	{ SGBOX, 0, 0, 1,1, 36,11, NULL },
	{ SGTEXT, 0, 0, 13,2, 13,1, "Sound options" },
	{ SGCHECKBOX, 0, 0, 12,4, 14,1, "Enable sound" },
	{ SGTEXT, 0, 0, 11,6, 14,1, "Playback quality:" },
	{ SGRADIOBUT, 0, 0, 12,8, 15,1, "Low (11kHz)" },
	{ SGRADIOBUT, 0, 0, 12,9, 19,1, "Medium (22kHz)" },
	{ SGRADIOBUT, 0, 0, 12,10, 14,1, "High (44kHz)" },
	{ SGBOX, 0, 0, 1,13, 36,8, NULL },
	{ SGTEXT, 0, 0, 13,14, 14,1, "Capture YM/WAV" },
	{ SGTEXT, 0, 0, 2,16, 26,1, "File name (*.wav or *.ym):" },
	{ SGTEXT, 0, 0, 2,17, 34,1, dlgRecordName },
	{ SGBUTTON, 0, 0, 30,16, 6,1, "Browse" },
	{ SGBUTTON, 0, 0, 12,19, 16,1, NULL },
	{ SGBUTTON, SG_DEFAULT, 0, 10,22, 20,1, "Back to main menu" },
	{ -1, 0, 0, 0,0, 0,0, NULL }
};


/*-----------------------------------------------------------------------*/
/*
  Show and process the sound dialog.
*/
void Dialog_SoundDlg(void)
{
	int but;

	SDLGui_CenterDlg(sounddlg);

	/* Set up dialog from actual values: */

	if (ConfigureParams.Sound.bEnableSound)
		sounddlg[DLGSOUND_ENABLE].state |= SG_SELECTED;
	else
		sounddlg[DLGSOUND_ENABLE].state &= ~SG_SELECTED;

	sounddlg[DLGSOUND_LOW].state &= ~SG_SELECTED;
	sounddlg[DLGSOUND_MEDIUM].state &= ~SG_SELECTED;
	sounddlg[DLGSOUND_HIGH].state &= ~SG_SELECTED;
	if (ConfigureParams.Sound.nPlaybackQuality == PLAYBACK_LOW)
		sounddlg[DLGSOUND_LOW].state |= SG_SELECTED;
	else if (ConfigureParams.Sound.nPlaybackQuality == PLAYBACK_MEDIUM)
		sounddlg[DLGSOUND_MEDIUM].state |= SG_SELECTED;
	else
		sounddlg[DLGSOUND_HIGH].state |= SG_SELECTED;

	File_ShrinkName(dlgRecordName, ConfigureParams.Sound.szYMCaptureFileName, sounddlg[DLGSOUND_RECNAME].w);

	if ( Sound_AreWeRecording() )
		sounddlg[DLGSOUND_RECORD].txt = "Stop recording";
	else
		sounddlg[DLGSOUND_RECORD].txt = "Record sound";

	/* The sound dialog main loop */
	do
	{
		but = SDLGui_DoDialog(sounddlg, NULL);
		switch (but)
		{
		 case DLGSOUND_RECBROWSE:                    /* Choose a new record file */
			SDLGui_FileConfSelect(dlgRecordName,
			                      ConfigureParams.Sound.szYMCaptureFileName,
			                      sounddlg[DLGSOUND_RECNAME].w,
			                      TRUE);
			break;
		 case  DLGSOUND_RECORD:
			if (Sound_AreWeRecording())
			{
				sounddlg[DLGSOUND_RECORD].txt = "Record sound";
				Sound_EndRecording();
			}
			else
			{
				/* make sure that we have a valid file name... */
				if (strlen(ConfigureParams.Sound.szYMCaptureFileName) < 4)
				{
					strcpy(ConfigureParams.Sound.szYMCaptureFileName, "./hatari.wav");
				}
				sounddlg[DLGSOUND_RECORD].txt = "Stop recording";
				Sound_BeginRecording(ConfigureParams.Sound.szYMCaptureFileName);
			}
			break;
		}
	}
	while (but != DLGSOUND_EXIT && but != SDLGUI_QUIT
	        && but != SDLGUI_ERROR && !bQuitProgram );

	/* Read values from dialog */
	ConfigureParams.Sound.bEnableSound = (sounddlg[DLGSOUND_ENABLE].state & SG_SELECTED);
	if (sounddlg[DLGSOUND_LOW].state & SG_SELECTED)
		ConfigureParams.Sound.nPlaybackQuality = PLAYBACK_LOW;
	else if (sounddlg[DLGSOUND_MEDIUM].state & SG_SELECTED)
		ConfigureParams.Sound.nPlaybackQuality = PLAYBACK_MEDIUM;
	else
		ConfigureParams.Sound.nPlaybackQuality = PLAYBACK_HIGH;
}
