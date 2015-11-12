#include <pebble.h>
#include "timescales.h"

timescale get_selected_timescale(char *ts) {
  if (strcmp(ts, LOCAL_TIME)) {
    return TS_LOCAL_TIME;
  }
  else if (strcmp(ts, SECONDS)) {
    return TS_SECONDS;
  }
  else if (strcmp(ts, MINUTES)) {
    return TS_MINUTES;
  }
  else if (strcmp(ts, HOURS)) {
    return TS_HOURS;
  }
  else if (strcmp(ts, DAYS)) {
    return TS_DAYS;
  }
  else if (strcmp(ts, YEARS)) {
    return TS_YEARS;
  }
  else if (strcmp(ts, DAY)) {
    return TS_DAY;
  }
  else if (strcmp(ts, HOUR)) {
    return TS_HOUR;
  }
  else if (strcmp(ts, MONTH)) {
    return TS_MONTH;
  }
  else if (strcmp(ts, YEAR)) {
    return TS_YEAR;
  }
  else if (strcmp(ts, UNIVERSE)) {
    return TS_UNIVERSE;
  }
  else if (strcmp(ts, X_UNIVERSE)) {
    return TS_X_UNIVERSE;
  }
  else if (strcmp(ts, PERCENT)) {
    return TS_PERCENT;
  }
  else if (strcmp(ts, NONE)) {
    return TS_NONE;
  }
  else if (strcmp(ts, DEBUG)) {
    return TS_DEBUG;
  }
  else {
    return TS_ERROR;
  }
}
