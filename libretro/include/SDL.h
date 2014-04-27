
#ifndef HATARI_SDL_H
#define HATARI_SDL_H

//RETRO HACK
#warning This is just an SDL wrapper for the retro core.

extern int Reset_Cold(void);
extern int Reset_Warm(void);

#include <time.h>
#include "SDL_types.h"
#include "SDL_keyboard.h"
#include "SDL_video.h"

#define SDL_GetTicks  GetTicks 
#include <SDL_types.h>

#define RGB565(r, g, b)  (((r) << (5+6)) | ((g) << 6) | (b))
#define SDL_MapRGB(a, r, g, b) RGB565( (r)>>3, (g)>>3, (b)>>3)

typedef struct SDL_color{
int r;
int g;
int b;
int pixel;
} SDL_Color;

typedef struct SDL_Event{
Uint8 type;
} SDL_Event;

//SOME SDL_FUNC WRAPPER
//GLOBALS
#define SDL_ShowCursor(a) 
#define SDL_GRAB_OFF 0
#define SDL_GRAB_ON 1
#define SDL_WM_GrabInput(a)
#define SDL_WM_IconifyWindow()
#define SDL_WM_SetCaption(...)
#define SDL_HWSURFACE 0
#define SDL_FULLSCREEN 1
#define SDL_SWSURFACE 2
#define SDL_HWPALETTE 4
#define SDL_INIT_NOPARACHUTE 1
#define SDL_DISABLE 0
#define SDL_Quit(...)
#define SDL_InitSubSystem(...) 1
#define SDL_Init(...) 1
//TIME
#define SDL_Delay(a) usleep((a)*1000)
//SURFACE
#define SDL_SetColors(a, b, c, d)
#define SDL_MUSTLOCK(a) 0
#define SDL_LockSurface(a) 0
#define SDL_UnlockSurface(a) 0
#define SDL_FillRect(...)
#define SDL_UpdateRects(a, b,c)
#define SDL_UpdateRect(...)
#define SDL_SetVideoMode(w, h, b, f) prepare_texture((w),(h),(b))
//KEY
#define SDL_GetError() "RetroWrapper"
#define SDL_GetModState() 0
#define SDL_GetKeyName(...) "RetroWrapper"
//SOUND
#define SDL_CloseAudio();
#define SDL_LockAudio();
#define SDL_UnlockAudio();
#define SDL_PauseAudio(a);
//JOY
#define SDL_Joystick int
#define SDL_NumJoysticks() 0
#define SDL_JoystickOpen(i) NULL
#define SDL_JoystickName(i) "RetroWrapper"
#define SDL_JoystickClose
#define SDL_JoystickGetAxis(...) 0
#define SDL_JoystickGetButton(...) 0
#define SDL_JoystickNumAxes(...) 0
#define SDL_JoystickNumButtons(a) 16

//PS3 HACK
#ifdef PS3PORT
#if defined(__CELLOS_LV2__) 
#include <unistd.h> //stat() is defined here
#define S_ISDIR(x) (x & CELL_FS_S_IFDIR)
#define F_OK 0
#endif

#include "sys/sys_time.h"
#include "sys/timer.h"
#define usleep  sys_timer_usleep
#endif


#endif
