/*
 * GPL Notice:
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Library General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *
 * Name:
 *
 *    liballtraynomap, lib for use with alltray
 *
 *
 * Copyright:
 * 
 *    Jochen Baier, 2005 (email@Jochen-Baier.de)
 *
 *
 * Based on code from:
 *
 *    xalf (Peter Åstrand <astrand@lysator.liu.se>)
 *
 *
*/


#undef DEBUG 
/* Uncomment below for debugging */
// #define DEBUG

#ifdef DEBUG
#   define DPRINTF(args) fprintf args
#else
#   define DPRINTF(args) 
#endif

#define _GNU_SOURCE

#include <dlfcn.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xatom.h>

#include "config.h"


static int window_found=0;

int 
error_handler (Display *dpy, XErrorEvent *xerr) {

#ifdef DEBUG
  if (xerr->error_code) {
    char buf[64];
    
    XGetErrorText (dpy, xerr->error_code, buf, 63);
    fprintf (stderr, "liballtraynomap X11 error **: %s\n", buf);
    fprintf (stderr, "serial %ld error_code %d request_code %d "\
    "minor_code %d\n", 
    xerr->serial, 
    xerr->error_code, 
    xerr->request_code, 
    xerr->minor_code);
  }
#endif
  
  return 0; 
} 

#ifdef __FreeBSD__
static void _init(void) __attribute__ ((section (".init")));
#endif

#if defined __GNUC__ && ( ( __GNUC__ == 2 ) && ( __GNUC_MINOR__ >= 96) || ( __GNUC__ >= 3) )
void initialize (void) __attribute__ ((constructor));
void initialize (void)
#else
void
_init () 
#endif
{
    void *dlh = NULL;
    char *old_preload=NULL;

    DPRINTF ((stderr, "liballtraynomap: _init\n"));

    old_preload=getenv ("OLD_PRELOAD");

    DPRINTF ((stderr, "old preload: %s\n", old_preload));

    if (old_preload && *old_preload != '\0')
      setenv ("LD_PRELOAD", old_preload, 1);
    else
      unsetenv ("LD_PRELOAD");
    
    dlh = dlopen (NULL, RTLD_GLOBAL | RTLD_NOW);

    if (dlsym (dlh, "XSync") == NULL) {
      DPRINTF ((stderr, "liballtraynomap: No XSync\n"));
      dlclose (dlh);
      return;
    }
  
    dlclose(dlh);

}

void sent_found_window_to_parent (Display *display, Window found_window)
{
  
  XEvent xev;
  Atom type=XInternAtom (display, "ALLTRAY_FOUND_WINDOW", False);
  char *spy_id_string=NULL;
  int spy_id=0;
    
  spy_id_string=getenv ("ALLTRAY_SPY_WINDOW");
    
  DPRINTF ((stderr, "liballtraynomap: ALLTRAY_SPY_WINDOW: %s\n", spy_id_string));

  
  if (spy_id_string && *spy_id_string != '\0') {
    spy_id=atoi (spy_id_string);
    
    xev.xclient.type = ClientMessage;
    xev.xclient.serial = 0;
    xev.xclient.send_event = True;
    xev.xclient.window = spy_id;
    xev.xclient.message_type = type;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = found_window;
    xev.xclient.data.l[1] = 0;
    xev.xclient.data.l[2] = 0;
    xev.xclient.data.l[3] = 0;
    xev.xclient.data.l[4] = 0;
    
    XSendEvent (display, spy_id, False, 0, &xev);
  }
  
}

int iconic (Display *display, Window window)
{

  XWMHints *wm_hints;
  static int first_call=1;
  static int do_nothing=0;

  int return_val=0;

  if (first_call) {
    DPRINTF ((stderr, "set error handler\n"));
    XSetErrorHandler (error_handler);
    first_call=0;
  }

  wm_hints= XGetWMHints(display, window);
     
  if (wm_hints==NULL)
    return 0;
  
  DPRINTF ((stderr, "liballtraynomap: found wm hints\n"));
  
  if ((wm_hints->flags & StateHint) == StateHint) {
    DPRINTF ((stderr, "liballtraynomap: found state hint flag\n"));
    
    if (wm_hints->initial_state==NormalState) {
      DPRINTF ((stderr, "liballtraynomap: found NormalState\n"));
    
      wm_hints->initial_state=IconicState;
      XSetWMHints(display, window, wm_hints);
      return_val=1;
    }
  
  }

  XFree(wm_hints);
  
  return return_val;

}

extern int 
XMapWindow (Display* display, Window w)
{

  static int (*fptr)() = 0;
  static int first_call=1;
  int value;
  
  
  if (window_found)
    return (*fptr)(display, w);
  
  DPRINTF ((stderr, "liballtraynomap: XMapWindow %d\n", w));
    
  
  if (first_call) {
    DPRINTF ((stderr, "liballtraynomap: set error handler\n"));
    XSetErrorHandler (error_handler);
    first_call=0;
  }

  if (fptr == 0) {
  
    #ifdef RTLD_NEXT
      fptr = (int (*)())dlsym (RTLD_NEXT, "XMapWindow");
    #else
     
      DPRINTF ((stderr, "liballtraynomap: no RTLD_NEXT\n"));
      
     
      void *dlh = NULL;
  
      dlh = dlopen ("libX11.so", RTLD_GLOBAL | RTLD_NOW);
      if (dlh == NULL) 
        dlh = dlopen ("libX11.so.6", RTLD_GLOBAL | RTLD_NOW); 
      if (dlh == NULL)
        fprintf (stderr, "liballtraynomap: %s\n", dlerror ());
      
      if (dlh != NULL) {
        fptr = (int (*)())dlsym (dlh, "XMapWindow");
        dlclose (dlh);
      }
      
      DPRINTF ((stderr, "liballtraynomap: XMapWindow is at %p\n", fptr));
    #endif
    
    if (fptr == NULL) {
      fprintf (stderr, "liballtraynomap: dlsym: %s\n", dlerror());
      return 0;
    }
  
  }
      
  window_found=iconic (display,w);
  
  value=(*fptr)(display, w);

  if (window_found) {
    DPRINTF ((stderr, "liballtraynomap: withdraw !\n"));
    XWithdrawWindow (display, w,0);
    sent_found_window_to_parent (display, w);
  }

  return value;

}


extern int 
XMapRaised (Display* display, Window w)
{
  
  static int (*fptr)() = 0;
  static int first_call=1;
  int value;
  
  
  if (window_found)
    return (*fptr)(display, w);
  
  DPRINTF ((stderr, "liballtraynomap: XMapRaised %d\n", w));
    
  
  if (first_call) {
    DPRINTF ((stderr, "liballtraynomap: set error handler\n"));
    XSetErrorHandler (error_handler);
    first_call=0;
  }

  if (fptr == 0) {
  
    #ifdef RTLD_NEXT
      fptr = (int (*)())dlsym (RTLD_NEXT, "XMapRaised");
    #else
     
      DPRINTF ((stderr, "liballtraynomap: no RTLD_NEXT\n"));
      
     
      void *dlh = NULL;
  
      dlh = dlopen ("libX11.so", RTLD_GLOBAL | RTLD_NOW);
      if (dlh == NULL) 
        dlh = dlopen ("libX11.so.6", RTLD_GLOBAL | RTLD_NOW); 
      if (dlh == NULL)
        fprintf (stderr, "liballtraynomap: %s\n", dlerror ());
      
      if (dlh != NULL) {
        fptr = (int (*)())dlsym (dlh, "XMapRaised");
        dlclose (dlh);
      }
      
      DPRINTF ((stderr, "liballtraynomap: XMapRaised is at %p\n", fptr));
    #endif
    
    if (fptr == NULL) {
      fprintf (stderr, "liballtraynomap: dlsym: %s\n", dlerror());
      return 0;
    }
  
  }
      
  window_found=iconic (display,w);
  
  value=(*fptr)(display, w);

  if (window_found) {
    DPRINTF ((stderr, "liballtraynomap: withdraw !\n"));
    XWithdrawWindow (display, w,0);
    sent_found_window_to_parent (display, w);
  }

  return value;

 
}
