#ifndef MM_TIMESCALES_H
#define MM_TIMESCALES_H

/* timescale names
 * TRES IMPORTANT ! these strings MUST match the strings in index.html
 */
#define LOCAL_TIME "Local Time"
#define SECONDS "Seconds"
#define MINUTES "Minutes"
#define HOURS "Hours"
#define DAYS "Days"
#define DAYS_HOURS_MINS_SECS "D H M S"
#define YEARS "Years"
#define DAY "One Day"
#define HOUR "One Hour"
#define MONTH "One Month"
#define YEAR "One Year"
#define UNIVERSE "Universe"
#define X_UNIVERSE "X Universe"
#define X_UNIVERSE_2 "X Universe 2"
#define PERCENT "Percent"
#define ALT_TZ "Alt Timezone"
#define NONE "None"
#define DEBUG "Debug"
#define RANDOM "Random"

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
  TS_ALT_TZ,
  TS_NONE,
  TS_DEBUG,
  TS_RANDOM,
  TS_ERROR
} timescale;

timescale get_timescale_from_string(char *ts_name);
char *get_string_from_timescale(timescale ts);

#endif
