#include <pebble.h>
#include "timescales.h"

static char* timescale_name[] = {
  LOCAL_TIME,
  SECONDS,
  MINUTES,
  HOURS,
  DAYS, 
  DAYS_HOURS_MINS_SECS,
  YEARS,
  DAY,
  HOUR, 
  MONTH,
  YEAR,
  UNIVERSE,
  X_UNIVERSE,
  X_UNIVERSE_2,
  PERCENT,
  NONE,
  DEBUG
};

char *get_string_from_timescale(timescale ts) {
  return timescale_name[ts];
}

timescale get_timescale_from_string(char *ts_name) {
  if (strcmp(ts_name, LOCAL_TIME) == 0) {
    return TS_LOCAL_TIME;
  }
  else if (strcmp(ts_name, SECONDS) == 0) {
    return TS_SECONDS;
  }
  else if (strcmp(ts_name, MINUTES) == 0) {
    return TS_MINUTES;
  }
  else if (strcmp(ts_name, HOURS) == 0) {
    return TS_HOURS;
  }
  else if (strcmp(ts_name, DAYS) == 0) {
    return TS_DAYS;
  }
  else if (strcmp(ts_name, DAYS_HOURS_MINS_SECS) == 0) {
    return TS_DAYS_HOURS_MINS_SECS;
  }
  else if (strcmp(ts_name, YEARS) == 0) {
    return TS_YEARS;
  }
  else if (strcmp(ts_name, DAY) == 0) {
    return TS_DAY;
  }
  else if (strcmp(ts_name, HOUR) == 0) {
    return TS_HOUR;
  }
  else if (strcmp(ts_name, MONTH) == 0) {
    return TS_MONTH;
  }
  else if (strcmp(ts_name, YEAR) == 0) {
    return TS_YEAR;
  }
  else if (strcmp(ts_name, UNIVERSE) == 0) {
    return TS_UNIVERSE;
  }
  else if (strcmp(ts_name, X_UNIVERSE) == 0) {
    return TS_X_UNIVERSE;
  }
  else if (strcmp(ts_name, X_UNIVERSE_2) == 0) {
    return TS_X_UNIVERSE_2;
  }
  else if (strcmp(ts_name, PERCENT) == 0) {
    return TS_PERCENT;
  }
  else if (strcmp(ts_name, ALT_TZ) == 0) {
    return TS_ALT_TZ;
  }
  else if (strcmp(ts_name, NONE) == 0) {
    return TS_NONE;
  }
  else if (strcmp(ts_name, DEBUG) == 0) {
    return TS_DEBUG;
  }
  else {
    return TS_ERROR;
  }
}
