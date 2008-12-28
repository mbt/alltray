

#ifndef __EVENT_FILTER_H__
#define __EVENT_FILTER_H__

#include "common.h"

GdkFilterReturn event_filter (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data);



#endif
