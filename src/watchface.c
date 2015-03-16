#include "watchface.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_42_medium_numbers;
static GFont s_res_bitham_30_black;
static InverterLayer *s_inverterlayer_1;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_3;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, true);
  
  s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  s_res_bitham_30_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
  // s_inverterlayer_1
  s_inverterlayer_1 = inverter_layer_create(GRect(0, 88, 144, 80));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_1);
  
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(0, 104, 144, 42));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text(s_textlayer_1, "18:21");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_bitham_42_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(0, 29, 144, 38));
  text_layer_set_background_color(s_textlayer_2, GColorClear);
  text_layer_set_text_color(s_textlayer_2, GColorWhite);
  text_layer_set_text(s_textlayer_2, " ");
  text_layer_set_text_alignment(s_textlayer_2, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_2, s_res_bitham_30_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
  
  // s_textlayer_3
  s_textlayer_3 = text_layer_create(GRect(0, 0, 144, 36));
  text_layer_set_background_color(s_textlayer_3, GColorClear);
  text_layer_set_text_color(s_textlayer_3, GColorWhite);
  text_layer_set_text(s_textlayer_3, " ");
  text_layer_set_text_alignment(s_textlayer_3, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_3, s_res_bitham_30_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  inverter_layer_destroy(s_inverterlayer_1);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_2);
  text_layer_destroy(s_textlayer_3);
}
// END AUTO-GENERATED UI CODE

typedef enum {
	FacialExpressionBuckteeth	= 0,
	FacialExpressionClosedSmile	= 1,
	FacialExpressionColonP		= 2,
	FacialExpressionOpenSmile	= 3,
	FacialExpressionSmirk		= 4
} FacialExpression;
typedef struct {
	const char *top;
	const char *bottom;
} facial_expr_t;
static const facial_expr_t face_buckteeth = {
	.top	= "_____",
	.bottom	= "\\_|_/"
};
static const facial_expr_t face_closedSmile = {
	.top	= "       ",
	.bottom	= "\\_____/"
};
static const facial_expr_t face_colonP = {
	.top	= "______",
	.bottom	= "     \\__/"
};
static const facial_expr_t face_openSmile = {
	.top	= "______",
	.bottom	= "\\____/"
};
static const facial_expr_t face_smirk = {
	.top	= "        ",
	.bottom	= "    ___/"
};
static facial_expr_t facial_expressions[5];

static unsigned int rand_in_range(unsigned int min, unsigned int max) {
	float frac_norm = (float)rand() / ((float)RAND_MAX + 1); // range [0,1)
	float frac_scaled = frac_norm * (max - min + 1); // range [0,max)
	return (unsigned int)(frac_scaled + min);
}

static void test_rand_in_range() {
	unsigned int results[10] = { 0 };
	for (int i = 0; i < 1000; i++)
		results[rand_in_range(0, 4)]++;
	for (int i = 0; i < 10; i++)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "%d: %u", i, results[i]);
}

static void randomize_facial_expression() {
	static int num_facial_exprs = sizeof(facial_expressions) / sizeof(*facial_expressions);
	// choose a random expression
	int expr_idx = rand_in_range(0, num_facial_exprs - 1);
	facial_expr_t expression = facial_expressions[expr_idx];
	// set the facial expression
//	APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting expression #%d", expr_idx);
	text_layer_set_text(s_textlayer_3, expression.top);
	text_layer_set_text(s_textlayer_2, expression.bottom);
}

static void display_time(struct tm *tick_time) {
	static char string[6];
	snprintf(string, sizeof(string), "%02d:%02d", tick_time->tm_hour, tick_time->tm_min);
	text_layer_set_text(s_textlayer_1, string);
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
	display_time(tick_time);
	
	randomize_facial_expression();
}

static void main_window_load(Window *window) {
	time_t now = time(NULL);
	struct tm *tick_time = localtime(&now);
	display_time(tick_time);
	
	tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_watchface(void) {
	facial_expressions[0] = face_buckteeth;
	facial_expressions[1] = face_closedSmile;
	facial_expressions[2] = face_colonP;
	facial_expressions[3] = face_openSmile;
	facial_expressions[4] = face_smirk;
	
//	test_rand_in_range();
	
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
	  .load = main_window_load,
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_watchface(void) {
  window_stack_remove(s_window, true);
}
