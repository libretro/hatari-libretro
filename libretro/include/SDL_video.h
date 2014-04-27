#ifndef _SDL_video_h
#define _SDL_video_h

//#include "SDL.h" 

#include "retroscreen.h"

typedef struct{
     Sint16 x, y;
     Uint16 w, h;
} SDL_Rect;

typedef struct {
      //SDL_Palette *palette;
      Uint8  BitsPerPixel;
      Uint8  BytesPerPixel;
      Uint8  Rloss, Gloss, Bloss, Aloss;
      Uint8  Rshift, Gshift, Bshift, Ashift;
      Uint32 Rmask, Gmask, Bmask, Amask;
      Uint32 colorkey;
      Uint8  alpha;
} SDL_PixelFormat;

typedef struct SDL_Surface {
        Uint32 flags;                           /* Read-only */
        SDL_PixelFormat *format;                /* Read-only */
        int w, h;                               /* Read-only */
        Uint16 pitch;                           /* Read-only */
        void *pixels;                           /* Read-write */

        /* clipping information */
        SDL_Rect clip_rect;                     /* Read-only */

        /* Reference count -- used when freeing surface */
        int refcount;                           /* Read-mostly */

	/* This structure also contains private fields not shown here */
} SDL_Surface;

/*
typedef struct
{
    int w, h;
    int stride;    
    unsigned char * bitmap;
} SDL_Surface;
*/

#endif
