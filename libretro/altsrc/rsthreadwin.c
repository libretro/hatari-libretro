//RETRO HACK TO REDO
//SDLTHREAD FOR PTHREAD (file include from rs232.c)

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SDL_OutOfMemory()

extern long GetTicks(void);
#include <unistd.h>


#include <windows.h>

#define SDL_KillThread(X)

#define ERR_MAX_STRLEN	128
#define ERR_MAX_ARGS	5
#define SDLCALL

typedef struct SDL_error
{
    /* This is a numeric value corresponding to the current error */
    int error;

    /* This is a key used to index into a language hashtable containing
       internationalized versions of the SDL error messages.  If the key
       is not in the hashtable, or no hashtable is available, the key is
       used directly as an error message format string.
     */
    char key[ERR_MAX_STRLEN];

    /* These are the arguments for the error functions */
    int argc;
    union
    {
        void *value_ptr;
#if 0                           /* What is a character anyway?  (UNICODE issues) */
        unsigned char value_c;
#endif
        int value_i;
        double value_f;
        char buf[ERR_MAX_STRLEN];
    } args[ERR_MAX_ARGS];
} SDL_error;


struct SDL_semaphore {
#if defined(_WIN32_WCE) && (_WIN32_WCE < 300)
	SYNCHHANDLE id;
#else
	HANDLE id;
#endif
	volatile LONG count;
};


#define SDL_zero(x)	memset(&(x), 0, sizeof((x)))
#define SDL_malloc	malloc
#define SDL_MUTEX_TIMEDOUT	1
#define SDL_MUTEX_MAXWAIT	(~(Uint32)0)
#define SDL_SetError printf
#define SDL_free free

struct SDL_semaphore;
typedef struct SDL_semaphore SDL_sem;

/* Create a semaphore */
SDL_sem *SDL_CreateSemaphore(Uint32 initial_value)
{
	SDL_sem *sem;

	/* Allocate sem memory */
	sem = (SDL_sem *)SDL_malloc(sizeof(*sem));
	if ( sem ) {
		/* Create the semaphore, with max value 32K */
#if defined(_WIN32_WCE) && (_WIN32_WCE < 300)
		sem->id = CreateSemaphoreCE(NULL, initial_value, 32*1024, NULL);
#else
		sem->id = CreateSemaphore(NULL, initial_value, 32*1024, NULL);
#endif
		sem->count = (LONG) initial_value;
		if ( ! sem->id ) {
			SDL_SetError("Couldn't create semaphore");
			SDL_free(sem);
			sem = NULL;
		}
	} else {
		SDL_OutOfMemory();
	}
	return(sem);
}


/* Free the semaphore */
void SDL_DestroySemaphore(SDL_sem *sem)
{
	if ( sem ) {
		if ( sem->id ) {
#if defined(_WIN32_WCE) && (_WIN32_WCE < 300)
			CloseSynchHandle(sem->id);
#else
			CloseHandle(sem->id);
#endif
			sem->id = 0;
		}
		SDL_free(sem);
	}
}

int SDL_SemWaitTimeout(SDL_sem *sem, Uint32 timeout)
{
	int retval;
	DWORD dwMilliseconds;

	if ( ! sem ) {
		SDL_SetError("Passed a NULL sem");
		return -1;
	}

	if ( timeout == SDL_MUTEX_MAXWAIT ) {
		dwMilliseconds = INFINITE;
	} else {
		dwMilliseconds = (DWORD)timeout;
	}
#if defined(_WIN32_WCE) && (_WIN32_WCE < 300)
	switch (WaitForSemaphoreCE(sem->id, dwMilliseconds)) {
#else
	switch (WaitForSingleObject(sem->id, dwMilliseconds)) {
#endif
	    case WAIT_OBJECT_0:
		InterlockedDecrement(&sem->count);
		retval = 0;
		break;
	    case WAIT_TIMEOUT:
		retval = SDL_MUTEX_TIMEDOUT;
		break;
	    default:
		SDL_SetError("WaitForSingleObject() failed");
		retval = -1;
		break;
	}
	return retval;
}

int SDL_SemTryWait(SDL_sem *sem)
{
	return SDL_SemWaitTimeout(sem, 0);
}

int SDL_SemWait(SDL_sem *sem)
{
	return SDL_SemWaitTimeout(sem, SDL_MUTEX_MAXWAIT);
}

/* Returns the current count of the semaphore */
Uint32 SDL_SemValue(SDL_sem *sem)
{
	if ( ! sem ) {
		SDL_SetError("Passed a NULL sem");
		return 0;
	}
	return (Uint32) sem->count;
}

int SDL_SemPost(SDL_sem *sem)
{
	if ( ! sem ) {
		SDL_SetError("Passed a NULL sem");
		return -1;
	}
	/* Increase the counter in the first place, because
	 * after a successful release the semaphore may
	 * immediately get destroyed by another thread which
	 * is waiting for this semaphore.
	 */
	InterlockedIncrement(&sem->count);
#if defined(_WIN32_WCE) && (_WIN32_WCE < 300)
	if ( ReleaseSemaphoreCE(sem->id, 1, NULL) == FALSE ) {
#else
	if ( ReleaseSemaphore(sem->id, 1, NULL) == FALSE ) {
#endif
		InterlockedDecrement(&sem->count);	/* restore */
		SDL_SetError("ReleaseSemaphore() failed");
		return -1;
	}
	return 0;
}
typedef struct SDL_Thread SDL_Thread;
typedef unsigned long SDL_threadID;

/* The SDL thread priority
 *
 * Note: On many systems you require special privileges to set high priority.
 */
typedef enum {
    SDL_THREAD_PRIORITY_LOW,
    SDL_THREAD_PRIORITY_NORMAL,
    SDL_THREAD_PRIORITY_HIGH
} SDL_ThreadPriority;

typedef int (SDLCALL * SDL_ThreadFunction) (void *data);
typedef HANDLE SYS_ThreadHandle;

/* This is the system-independent thread info structure */
struct SDL_Thread
{
    SDL_threadID threadid;
    SYS_ThreadHandle handle;
    int status;
    SDL_error errbuf;
    void *data;
};
/*
struct SDL_mutex {
	pthread_mutex_t id;
};

typedef sys_mutex_t pthread_mutex_t;
*/

struct SDL_mutex {
	HANDLE id;
};

typedef struct SDL_mutex SDL_mutex;
extern SDL_mutex *SDL_CreateMutex(void);




/* Create a mutex */
SDL_mutex *SDL_CreateMutex(void)
{
	SDL_mutex *mutex;

	/* Allocate mutex memory */
	mutex = (SDL_mutex *)SDL_malloc(sizeof(*mutex));
	if ( mutex ) {
		/* Create the mutex, with initial value signaled */
		mutex->id = CreateMutex(NULL, FALSE, NULL);
		if ( ! mutex->id ) {
			SDL_SetError("Couldn't create mutex");
			SDL_free(mutex);
			mutex = NULL;
		}
	} else {
		SDL_OutOfMemory();
	}
	return(mutex);
}

/* Free the mutex */
void SDL_DestroyMutex(SDL_mutex *mutex)
{
	if ( mutex ) {
		if ( mutex->id ) {
			CloseHandle(mutex->id);
			mutex->id = 0;
		}
		SDL_free(mutex);
	}
}

/* Lock the mutex */
int SDL_mutexP(SDL_mutex *mutex)
{
	if ( mutex == NULL ) {
		SDL_SetError("Passed a NULL mutex");
		return -1;
	}
	if ( WaitForSingleObject(mutex->id, INFINITE) == WAIT_FAILED ) {
		SDL_SetError("Couldn't wait on mutex");
		return -1;
	}
	return(0);
}

/* Unlock the mutex */
int SDL_mutexV(SDL_mutex *mutex)
{
	if ( mutex == NULL ) {
		SDL_SetError("Passed a NULL mutex");
		return -1;
	}
	if ( ReleaseMutex(mutex->id) == FALSE ) {
		SDL_SetError("Couldn't release mutex");
		return -1;
	}
	return(0);
}
/* vi: set ts=4 sw=4 expandtab: */




#define ARRAY_CHUNKSIZE	32
/* The array of threads currently active in the application
   (except the main thread)
   The manipulation of an array here is safer than using a linked list.
*/
static int SDL_maxthreads = 0;
static int SDL_numthreads = 0;
static SDL_Thread **SDL_Threads = NULL;
static SDL_mutex *thread_lock = NULL;

static int
SDL_ThreadsInit(void)
{
    int retval;

    retval = 0;
    thread_lock = SDL_CreateMutex();
    if (thread_lock == NULL) {
        retval = -1;
    }
    return (retval);
}

/* This should never be called...
   If this is called by SDL_Quit(), we don't know whether or not we should
   clean up threads here.  If any threads are still running after this call,
   they will no longer have access to any per-thread data.
 */
#if 0
static void
SDL_ThreadsQuit(void)
{
    SDL_mutex *mutex;

    mutex = thread_lock;
    thread_lock = NULL;
    if (mutex != NULL) {
        SDL_DestroyMutex(mutex);
    }
}
#endif

/* Routines for manipulating the thread list */
static void
SDL_AddThread(SDL_Thread * thread)
{
    /* WARNING:
       If the very first threads are created simultaneously, then
       there could be a race condition causing memory corruption.
       In practice, this isn't a problem because by definition there
       is only one thread running the first time this is called.
     */
    if (!thread_lock) {
        if (SDL_ThreadsInit() < 0) {
            return;
        }
    }
    SDL_mutexP(thread_lock);

    /* Expand the list of threads, if necessary */
#ifdef DEBUG_THREADS
    printf("Adding thread (%d already - %d max)\n",
           SDL_numthreads, SDL_maxthreads);
#endif
    if (SDL_numthreads == SDL_maxthreads) {
        SDL_Thread **threads;
        threads = (SDL_Thread **) /*SDL_*/realloc(SDL_Threads,
                                              (SDL_maxthreads +
                                               ARRAY_CHUNKSIZE) *
                                              (sizeof *threads));
        if (threads == NULL) {
            //SDL_OutOfMemory();
            goto done;
        }
        SDL_maxthreads += ARRAY_CHUNKSIZE;
        SDL_Threads = threads;
    }
    SDL_Threads[SDL_numthreads++] = thread;
  done:
    SDL_mutexV(thread_lock);
}

static void
SDL_DelThread(SDL_Thread * thread)
{
    int i;

    if (!thread_lock) {
        return;
    }
    SDL_mutexP(thread_lock);
    for (i = 0; i < SDL_numthreads; ++i) {
        if (thread == SDL_Threads[i]) {
            break;
        }
    }
    if (i < SDL_numthreads) {
        if (--SDL_numthreads > 0) {
            while (i < SDL_numthreads) {
                SDL_Threads[i] = SDL_Threads[i + 1];
                ++i;
            }
        } else {
            SDL_maxthreads = 0;
            /*SDL_*/free(SDL_Threads);
            SDL_Threads = NULL;
        }
#ifdef DEBUG_THREADS
        printf("Deleting thread (%d left - %d max)\n",
               SDL_numthreads, SDL_maxthreads);
#endif
    }
    SDL_mutexV(thread_lock);

#if 0   /* There could be memory corruption if another thread is starting */
    if (SDL_Threads == NULL) {
        SDL_ThreadsQuit();
    }
#endif
}
Uint32 SDL_ThreadID(void)
{
	return((Uint32)GetCurrentThreadId());
}

/* The default (non-thread-safe) global error variable */
static SDL_error SDL_global_error;

/* Routine to get the thread-specific error variable */
SDL_error *
SDL_GetErrBuf(void)
{
    SDL_error *errbuf;

    errbuf = &SDL_global_error;
    if (SDL_Threads) {
        int i;
        SDL_threadID this_thread;

        this_thread = SDL_ThreadID();
        SDL_mutexP(thread_lock);
        for (i = 0; i < SDL_numthreads; ++i) {
            if (this_thread == SDL_Threads[i]->threadid) {
                errbuf = &SDL_Threads[i]->errbuf;
                break;
            }
        }
        SDL_mutexV(thread_lock);
    }
    return (errbuf);
}


/* Arguments and callback to setup and run the user thread function */
typedef struct
{
    int (SDLCALL * func) (void *);
    void *data;
    SDL_Thread *info;
    SDL_sem *wait;
} thread_args;


void SDL_SYS_SetupThread(void)
{
	return;
}

void
SDL_RunThread(void *data)
{
    thread_args *args;
    int (SDLCALL * userfunc) (void *);
    void *userdata;
    int *statusloc;

    /* Perform any system-dependent setup
       - this function cannot fail, and cannot use SDL_SetError()
     */
    SDL_SYS_SetupThread();

    /* Get the thread id */
    args = (thread_args *) data;
    args->info->threadid = SDL_ThreadID();

    /* Figure out what function to run */
    userfunc = args->func;
    userdata = args->data;
    statusloc = &args->info->status;

    /* Wake up the parent thread */
    SDL_SemPost(args->wait);

    /* Run the function */
    *statusloc = userfunc(userdata);
}
typedef unsigned long (__cdecl *pfnSDL_CurrentBeginThread) (void *, unsigned,
        unsigned (__stdcall *func)(void *), void *arg, 
        unsigned, unsigned *threadID);
typedef void (__cdecl *pfnSDL_CurrentEndThread)(unsigned code);


typedef struct ThreadStartParms
{
  void *args;
  pfnSDL_CurrentEndThread pfnCurrentEndThread;
} tThreadStartParms, *pThreadStartParms;



static DWORD RunThread(void *data)
{
  pThreadStartParms pThreadParms = (pThreadStartParms)data;
  pfnSDL_CurrentEndThread pfnCurrentEndThread = NULL;

  // Call the thread function!
  SDL_RunThread(pThreadParms->args);

  // Get the current endthread we have to use!
  if (pThreadParms)
  {
    pfnCurrentEndThread = pThreadParms->pfnCurrentEndThread;
    SDL_free(pThreadParms);
  }
  // Call endthread!
  if (pfnCurrentEndThread)
    (*pfnCurrentEndThread)(0);
  return(0);
}

static DWORD WINAPI RunThreadViaCreateThread(LPVOID data)
{
  return RunThread(data);
}

static unsigned __stdcall RunThreadViaBeginThreadEx(void *data)
{
  return (unsigned) RunThread(data);
}

int SDL_SYS_CreateThread(SDL_Thread *thread, void *args)
{
#ifdef _WIN32_WCE
	pfnSDL_CurrentBeginThread pfnBeginThread = NULL;
	pfnSDL_CurrentEndThread pfnEndThread = NULL;
#else
	pfnSDL_CurrentBeginThread pfnBeginThread = _beginthreadex;
	pfnSDL_CurrentEndThread pfnEndThread = _endthreadex;
#endif
//#endif /* SDL_PASSED_BEGINTHREAD_ENDTHREAD */
	pThreadStartParms pThreadParms = (pThreadStartParms)SDL_malloc(sizeof(tThreadStartParms));
	if (!pThreadParms) {
		SDL_OutOfMemory();
		return(-1);
	}

	// Save the function which we will have to call to clear the RTL of calling app!
	pThreadParms->pfnCurrentEndThread = pfnEndThread;
	// Also save the real parameters we have to pass to thread function
	pThreadParms->args = args;

	if (pfnBeginThread) {
		unsigned threadid = 0;
		thread->handle = (SYS_ThreadHandle)
				((size_t) pfnBeginThread(NULL, 0, RunThreadViaBeginThreadEx,
										 pThreadParms, 0, &threadid));
	} else {
		DWORD threadid = 0;
		thread->handle = CreateThread(NULL, 0, RunThreadViaCreateThread, pThreadParms, 0, &threadid);
	}
	if (thread->handle == NULL) {
		SDL_SetError("Not enough resources to create thread");
		return(-1);
	}
	return(0);
}


void SDL_SYS_WaitThread(SDL_Thread *thread)
{
	WaitForSingleObject(thread->handle, INFINITE);
	CloseHandle(thread->handle);
}

/*
int
SDL_SYS_SetThreadPriority(SDL_ThreadPriority priority)
{
    int value;

    if (priority == SDL_THREAD_PRIORITY_LOW) {
        value = 3000;
    } else if (priority == SDL_THREAD_PRIORITY_HIGH) {
        value = 1;
    } else {
        value = 1500;
    }

	sys_ppu_thread_t id;
	sysThreadGetId(&id);
	return sysThreadSetPriority(id, value);
}
*/
//////////////////

#define DECLSPEC

#ifdef SDL_PASSED_BEGINTHREAD_ENDTHREAD
#undef SDL_CreateThread
DECLSPEC SDL_Thread *SDLCALL
SDL_CreateThread(int (SDLCALL * fn) (void *), void *data,
                 pfnSDL_CurrentBeginThread pfnBeginThread,
                 pfnSDL_CurrentEndThread pfnEndThread)
#else
DECLSPEC SDL_Thread *SDLCALL
SDL_CreateThread(int (SDLCALL * fn) (void *), void *data)
#endif
{
    SDL_Thread *thread;
    thread_args *args;
    int ret;

    /* Allocate memory for the thread info structure */
    thread = (SDL_Thread *) /*SDL_*/malloc(sizeof(*thread));
    if (thread == NULL) {
        //SDL_OutOfMemory();
        return (NULL);
    }
    /*SDL_*/memset(thread, 0, (sizeof *thread));
    thread->status = -1;

    /* Set up the arguments for the thread */
    args = (thread_args *) SDL_malloc(sizeof(*args));
    if (args == NULL) {
        //SDL_OutOfMemory();
        /*SDL_*/free(thread);
        return (NULL);
    }
    args->func = fn;
    args->data = data;
    args->info = thread;
    args->wait = SDL_CreateSemaphore(0);
    if (args->wait == NULL) {
        /*SDL_*/free(thread);
        /*SDL_*/free(args);
        return (NULL);
    }

    /* Add the thread to the list of available threads */
    SDL_AddThread(thread);

    /* Create the thread and go! */
#ifdef SDL_PASSED_BEGINTHREAD_ENDTHREAD
    ret = SDL_SYS_CreateThread(thread, args, pfnBeginThread, pfnEndThread);
#else
    ret = SDL_SYS_CreateThread(thread, args);
#endif
    if (ret >= 0) {
        /* Wait for the thread function to use arguments */
        SDL_SemWait(args->wait);
    } else {
        /* Oops, failed.  Gotta free everything */
        SDL_DelThread(thread);
        /*SDL_*/free(thread);
        thread = NULL;
    }
    SDL_DestroySemaphore(args->wait);
    /*SDL_*/free(args);

    /* Everything is running now */
    return (thread);
}

SDL_threadID
SDL_GetThreadID(SDL_Thread * thread)
{
    SDL_threadID id;

    if (thread) {
        id = thread->threadid;
    } else {
        id = SDL_ThreadID();
    }
    return id;
}

int
SDL_SetThreadPriority(SDL_ThreadPriority priority)
{
    return 0;//SDL_SYS_SetThreadPriority(priority);
}

void
SDL_WaitThread(SDL_Thread * thread, int *status)
{
    if (thread) {
        SDL_SYS_WaitThread(thread);
        if (status) {
            *status = thread->status;
        }
        SDL_DelThread(thread);
        /*SDL_*/free(thread);
    }
}

/* vi: set ts=4 sw=4 expandtab: */



