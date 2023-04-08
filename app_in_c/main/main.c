#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "display.h"

#define TAG "PyLaserC"

#define SENSOR1 34
#define SENSOR2 35
#define SENSOR3 32


void app_main(void)
{
	int8_t energia = 100;
	display_init();

	gpio_set_direction(SENSOR1, GPIO_MODE_INPUT);
	gpio_set_direction(SENSOR2, GPIO_MODE_INPUT);
	gpio_set_direction(SENSOR3, GPIO_MODE_INPUT);

	while (true) {
		if (!gpio_get_level(SENSOR1)) energia--;
		if (!gpio_get_level(SENSOR2)) energia--;
		if (!gpio_get_level(SENSOR3)) energia--;

		if (energia >= 0) {
			display_set((uint8_t)energia);
		} else {
			energia = 0;
		}
		vTaskDelay(10);
	}
}
