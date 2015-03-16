#include <pebble.h>
	#include "watchface.h"

Window *my_window;
TextLayer *text_layer;

void handle_init(void) {
	srand(time(NULL));
	show_watchface();
}

void handle_deinit(void) {
	hide_watchface();
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
