#ifndef TIMESCALES_H
#define TIMESCALES_H

/* timescales - these strings must match the strings in index.html */
#define LOCAL_TIME "Local Time"
#define SECONDS "Seconds"
#define MINUTES "Minutes"
#define HOURS "Hours"
#define DAYS "Days"
#define DAYS_HOURS_MINS_SECS "D H M S"
#define YEARS "Years"
#define DAY "Day"
#define HOUR "Hour"
#define MONTH "Month"
#define YEAR "Year"
#define UNIVERSE "Universe"
#define X_UNIVERSE "X Universe"
#define X_UNIVERSE_2 "X Universe 2"
#define PERCENT "Percent"
#define NONE "None"
#define DEBUG "Debug"

typedef enum {
  TS_LOCAL_TIME,
  TS_SECONDS,
  TS_MINUTES,
  TS_HOURS,
  TS_DAYS,
  TS_DAYS_HOURS_MINS_SECS,
  TS_YEARS,
  TS_DAY,
  TS_HOUR,
  TS_MONTH,
  TS_YEAR,
  TS_UNIVERSE,
  TS_X_UNIVERSE,
  TS_X_UNIVERSE_2,
  TS_PERCENT,
  TS_NONE,
  TS_DEBUG,
  TS_ERROR
} timescale;

timescale get_selected_timescale(char *ts);

#endif
