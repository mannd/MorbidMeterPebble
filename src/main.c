#include <pebble.h>

#define MM_TITLE "MorbidMeter"
#define DATE "%n%b %e %Y"

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

#define KEY_BACKGROUND_COLOR 0
#define KEY_TWENTY_FOUR_HOUR_FORMAT 1
#define KEY_TIMESCALE 2
#define KEY_LOCAL_TIME_SHOW_SECONDS 3
    
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_timescale_layer;
static bool twenty_four_hour_format = false;

/// TODO this needs to be an enum for all the timescales
// enum Timescale { Local_Time, etc. }
static bool is_local_time = true;
static char time_buffer[] = MM_TITLE "\nMMM 00 0000\n00:00:00 pm";
static char timescale_buffer[] = "   " LOCAL_TIME;
static bool local_time_show_seconds = true;
static char mm_time_update_interval_buffer[] = SECONDS;

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
  TS_DEBUG
} timescale;

static timescale selected_timescale = TS_LOCAL_TIME;
static timescale displayed_timescale = TS_LOCAL_TIME;

static void set_background_and_text_color(int color) {
#ifdef PBL_SDK_3
  GColor background_color = GColorFromHEX(color);
  window_set_background_color(s_main_window, background_color);
  text_layer_set_text_color(s_time_layer, gcolor_legible_over(background_color));
  text_layer_set_text_color(s_timescale_layer, gcolor_legible_over(background_color));
#endif
}

static bool local_time_update_with_secs() {
  return local_time_show_seconds;
}

static void update_time() {
  if (is_local_time) {
    time_t temp = time(NULL); 
    struct tm *tick_time = localtime(&temp);
    // Write the current hours and minutes into the buffer
    if (clock_is_24h_style() == twenty_four_hour_format) {
      if (local_time_update_with_secs()) {
	strftime(time_buffer, sizeof(time_buffer),
		 MM_TITLE DATE "\n%H:%M:%S", tick_time);
      } else {
	strftime(time_buffer, sizeof(time_buffer),
		 MM_TITLE DATE "\n%H:%M", tick_time);
      }
    } else {
      if (local_time_update_with_secs()) {
	strftime(time_buffer, sizeof(time_buffer),
		 MM_TITLE DATE "\n%l:%M:%S %p", tick_time);
      } else {
	strftime(time_buffer, sizeof(time_buffer),
		 MM_TITLE DATE "\n%l:%M %p", tick_time);
      }
    }
  } else {
    strcpy(time_buffer, "MorbidMeter\nTime\nSoon!");
  }
  text_layer_set_text(s_time_layer, time_buffer);
}

static void set_timescale() {
  is_local_time = (strcmp(timescale_buffer, LOCAL_TIME) == 0);
  text_layer_set_text(s_timescale_layer, timescale_buffer);
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *background_color_t = dict_find(iter, KEY_BACKGROUND_COLOR);
  Tuple *twenty_four_hour_format_t = dict_find(iter, KEY_TWENTY_FOUR_HOUR_FORMAT);
  Tuple *timescale_t = dict_find(iter, KEY_TIMESCALE);
  Tuple *local_time_show_seconds_t = dict_find(iter, KEY_LOCAL_TIME_SHOW_SECONDS);

  if (background_color_t) {
    int background_color = background_color_t->value->int32;
    persist_write_int(KEY_BACKGROUND_COLOR, background_color);
    set_background_and_text_color(background_color);
  }
  if (twenty_four_hour_format_t) {
    twenty_four_hour_format = twenty_four_hour_format_t->value->int8;
    persist_write_int(KEY_TWENTY_FOUR_HOUR_FORMAT, twenty_four_hour_format);
    update_time();
  }
  if (timescale_t) {
    strncpy(timescale_buffer, timescale_t->value->cstring, sizeof(timescale_buffer));
    persist_write_string(KEY_TIMESCALE, timescale_buffer);
    set_timescale();
  }
  if (local_time_show_seconds_t) {
    local_time_show_seconds = local_time_show_seconds_t->value->int8;
    persist_write_int(KEY_LOCAL_TIME_SHOW_SECONDS, local_time_show_seconds);
    update_time();
  }
}

static void set_selected_timescale() {
  if (strcmp(timescale_buffer, LOCAL_TIME)) {
    selected_timescale = TS_LOCAL_TIME;
  }
  else if (strcmp(timescale_buffer, SECONDS)) {
    selected_timescale = TS_SECONDS;
  }
  else if (strcmp(timescale_buffer, MINUTES)) {
    selected_timescale = TS_MINUTES;
  }
  else if (strcmp(timescale_buffer, HOURS)) {
    selected_timescale = TS_HOURS;
  }
  else if (strcmp(timescale_buffer, DAYS)) {
    selected_timescale = TS_DAYS;
  }
  else if (strcmp(timescale_buffer, YEARS)) {
    selected_timescale = TS_YEARS;
  }
  else if (strcmp(timescale_buffer, DAY)) {
    selected_timescale = TS_DAY;
  }
  else if (strcmp(timescale_buffer, HOUR)) {
    selected_timescale = TS_HOUR;
  }
  else if (strcmp(timescale_buffer, MONTH)) {
    selected_timescale = TS_MONTH;
  }
  else if (strcmp(timescale_buffer, YEAR)) {
    selected_timescale = TS_YEAR;
  }
  else if (strcmp(timescale_buffer, UNIVERSE)) {
    selected_timescale = TS_UNIVERSE;
  }
  else if (strcmp(timescale_buffer, X_UNIVERSE)) {
    selected_timescale = TS_X_UNIVERSE;
  }
  else if (strcmp(timescale_buffer, PERCENT)) {
    selected_timescale = TS_PERCENT;
  }
  else if (strcmp(timescale_buffer, NONE)) {
    selected_timescale = TS_NONE;
  }
  else if (strcmp(timescale_buffer, DEBUG)) {
    selected_timescale = TS_DEBUG;
  }
}

static void main_window_load(Window *window) {

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 30, bounds.size.w, 100));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, MM_TITLE "\n00:00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(
  FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Create timescale TextLayer
#if defined(PBL_RECT)
  s_timescale_layer = text_layer_create(GRect(0, bounds.size.h - 20,
					      bounds.size.w, 20));
#elif defined(PBL_ROUND)
  s_timescale_layer = text_layer_create(GRect(0, bounds.size.h - 50,
					      bounds.size.w, 20));
#endif
  text_layer_set_background_color(s_timescale_layer, GColorClear);
  text_layer_set_text_color(s_timescale_layer, GColorBlack);
  text_layer_set_font(s_timescale_layer, fonts_get_system_font(
  FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_timescale_layer, GTextAlignmentCenter);
  strncpy(timescale_buffer, LOCAL_TIME, sizeof(timescale_buffer));

  if (persist_read_int(KEY_BACKGROUND_COLOR)) {
    int background_color = persist_read_int(KEY_BACKGROUND_COLOR);
    set_background_and_text_color(background_color);
  }
  if (persist_read_bool(KEY_TWENTY_FOUR_HOUR_FORMAT)) {
    twenty_four_hour_format = persist_read_bool(KEY_TWENTY_FOUR_HOUR_FORMAT);
  }
  char tmp_buffer[sizeof(timescale_buffer)];
  if (persist_read_string(KEY_TIMESCALE, tmp_buffer,
    			  sizeof(tmp_buffer)) > 0) {
    strncpy(timescale_buffer, tmp_buffer, sizeof(timescale_buffer));
    set_selected_timescale();
  }
  if (persist_read_bool(KEY_LOCAL_TIME_SHOW_SECONDS)) {
      local_time_show_seconds = persist_read_bool(KEY_LOCAL_TIME_SHOW_SECONDS);
  }
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_timescale_layer));

  update_time();
  set_timescale();
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_timescale_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  /* (void)axis; */
  /* (void)direction; */
  APP_LOG(APP_LOG_LEVEL_DEBUG, "tap_handler() called");
  // return if Local Time or config doesn't allow shaking
  is_local_time = !is_local_time;
  // need to actually change timescales here
}
  
static void init() {
  s_main_window = window_create();
 
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);
  
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  accel_tap_service_subscribe(tap_handler);

  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
