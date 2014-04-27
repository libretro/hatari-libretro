#include "libco/libco.h"
extern cothread_t mainThread;
extern cothread_t emuThread;
extern int pauseg;

#ifdef PS3PORT
#undef HAVE_GETTIMEOFDAY
#undef HAVE_NANOSLEEP
#endif

extern int fmousex,fmousey;
extern int SND;

#ifndef sysconf
//#define sysconf(a) a
#endif

extern char RPATH[512];
extern long GetTicks(void);
#include <unistd.h>

#ifdef AND
#define CONFDIR "/mnt/sdcard/st"
    //#warning "conf dir for android"
#else
#ifdef PS3PORT
#define CONFDIR "/dev_hdd0/HOMEBREW/ST"
//#warning "conf dir for ps3"
#elif defined(WIIPORT)
#define CONFDIR "sd:/HOMEBREW/ST"
#else
#define CONFDIR "."
//#warning "conf dir for linux"
#endif //PS3PORT
#endif //AND

