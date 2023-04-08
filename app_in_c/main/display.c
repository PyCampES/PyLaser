#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ssd1306.h"

/*
 You have to set this config value with menuconfig
 CONFIG_INTERFACE

 for i2c
 CONFIG_MODEL
 CONFIG_SDA_GPIO
 CONFIG_SCL_GPIO
 CONFIG_RESET_GPIO

 for SPI
 CONFIG_CS_GPIO
 CONFIG_DC_GPIO
 CONFIG_RESET_GPIO
*/

#define TAG "SSD1306"

#define IMAGES 10

uint8_t segmentDisplay[IMAGES][192] = {
	{// https://www.iconspng.com/image/5656/seven-segment-display-gray-0
	 // 'seven-segment-display-gray-0', 32x48px
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x06, 0xff, 0xff, 0x60,
	 0x07, 0x7f, 0xfe, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x02, 0x00, 0x00, 0x40,
	 0x02, 0x00, 0x00, 0x40, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0xff, 0xff, 0xe0,
	 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{// https://www.iconspng.com/image/5657/seven-segment-display-gray-1
	 // 'seven-segment-display-gray-1', 32x48px
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x60,
	 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x40,
	 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xe0,
	 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{// https://www.iconspng.com/image/5658/seven-segment-display-gray-2
	 // 'seven-segment-display-gray-2', 32x48px
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0x60,
	 0x00, 0xff, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x7f, 0xfe, 0xe0, 0x01, 0xff, 0xff, 0xc0,
	 0x03, 0xff, 0xff, 0x00, 0x07, 0x7f, 0xfe, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00,
	 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00,
	 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00,
	 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0xff, 0xff, 0x00,
	 0x07, 0xff, 0xff, 0x80, 0x07, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{// https://www.iconspng.com/image/5659/seven-segment-display-gray-3
	 // 'seven-segment-display-gray-3', 32x48px
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0x60,
	 0x00, 0xff, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x7f, 0xfe, 0xe0, 0x01, 0xff, 0xff, 0x80,
	 0x01, 0xff, 0xff, 0x40, 0x00, 0x7f, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0xff, 0xff, 0xe0,
	 0x01, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{// https://www.iconspng.com/image/5660/seven-segment-display-gray-4
	 // 'seven-segment-display-gray-4', 32x48px
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x20, 0x06, 0x00, 0x00, 0x60,
	 0x07, 0x00, 0x00, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x7f, 0xfe, 0xe0, 0x03, 0xff, 0xff, 0x80,
	 0x00, 0xff, 0xff, 0x40, 0x00, 0x7f, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xe0,
	 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{// https://www.iconspng.com/image/5661/seven-segment-display-gray-5
	 // 'seven-segment-display-gray-5', 32x48px
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xc0, 0x06, 0xff, 0xff, 0x80,
	 0x07, 0x7f, 0xff, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00,
	 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00,
	 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00,
	 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x7f, 0xfe, 0x00, 0x03, 0xff, 0xff, 0x80,
	 0x00, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0xff, 0xff, 0xe0,
	 0x01, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{// https://www.iconspng.com/image/5662/seven-segment-display-gray-6
	 // 'seven-segment-display-gray-6', 32x48px
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xc0, 0x06, 0xff, 0xff, 0x80,
	 0x07, 0x7f, 0xff, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00,
	 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00,
	 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00,
	 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x7f, 0xfe, 0x00, 0x01, 0xff, 0xff, 0x80,
	 0x02, 0xff, 0xff, 0xc0, 0x07, 0x7f, 0xfe, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0xff, 0xff, 0xe0,
	 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{// https://www.iconspng.com/image/5663/seven-segment-display
	 // 'seven-segment-display-gray-7', 32x48px
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0x60,
	 0x00, 0xff, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x40,
	 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xe0,
	 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{// https://www.iconspng.com/image/5664/seven-segment-display-gray-8
	 // 'seven-segment-display-gray-8', 32x48px
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x06, 0xff, 0xff, 0x60,
	 0x07, 0x7f, 0xfe, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x7f, 0xfe, 0xe0, 0x01, 0xff, 0xff, 0x80,
	 0x02, 0xff, 0xff, 0x40, 0x07, 0x7f, 0xfe, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0xff, 0xff, 0xe0,
	 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{// https://www.iconspng.com/image/5665/seven-segment-display
	 // 'seven-segment-display-gray-9', 32x48px
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x06, 0xff, 0xff, 0x60,
	 0x07, 0x7f, 0xfe, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
	 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x7f, 0xfe, 0xe0, 0x03, 0xff, 0xff, 0x80,
	 0x00, 0xff, 0xff, 0x40, 0x00, 0x7f, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
	 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0xff, 0xff, 0xe0,
	 0x01, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

uint16_t current_value;

void display_task(void *a)
{
	SSD1306_t dev;

#if CONFIG_I2C_INTERFACE
	ESP_LOGI(TAG, "INTERFACE is i2c");
	ESP_LOGI(TAG, "CONFIG_SDA_GPIO=%d", CONFIG_SDA_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCL_GPIO=%d", CONFIG_SCL_GPIO);
	ESP_LOGI(TAG, "CONFIG_RESET_GPIO=%d", CONFIG_RESET_GPIO);
	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_I2C_INTERFACE

#if CONFIG_SPI_INTERFACE
	ESP_LOGI(TAG, "INTERFACE is SPI");
	ESP_LOGI(TAG, "CONFIG_MOSI_GPIO=%d", CONFIG_MOSI_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCLK_GPIO=%d", CONFIG_SCLK_GPIO);
	ESP_LOGI(TAG, "CONFIG_CS_GPIO=%d", CONFIG_CS_GPIO);
	ESP_LOGI(TAG, "CONFIG_DC_GPIO=%d", CONFIG_DC_GPIO);
	ESP_LOGI(TAG, "CONFIG_RESET_GPIO=%d", CONFIG_RESET_GPIO);
	spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_SPI_INTERFACE

#if CONFIG_FLIP
	dev._flip = true;
	ESP_LOGW(TAG, "Flip upside down");
#endif

#if CONFIG_SSD1306_128x64
	ESP_LOGI(TAG, "Panel is 128x64");
	ssd1306_init(&dev, 128, 64);
#endif // CONFIG_SSD1306_128x64
#if CONFIG_SSD1306_128x32
	ESP_LOGE(TAG, "Panel is 128x32. This demo cannot be run.");
	while (1)
	{
		vTaskDelay(1);
	}
#endif // CONFIG_SSD1306_128x32
	ssd1306_contrast(&dev, 0xff);

	// Allocate memory
	uint8_t *buffer = (uint8_t *)malloc(8 * 128); // 8 page 128 pixel
	if (buffer == NULL)
	{
		ESP_LOGE(TAG, "malloc failed");
		while (1)
		{
			vTaskDelay(1);
		}
	}
	uint8_t *segmentImage = (uint8_t *)malloc(IMAGES * 8 * 32); // 10 image 8 page 32 pixel
	if (segmentImage == NULL)
	{
		ESP_LOGE(TAG, "malloc failed");
		while (1)
		{
			vTaskDelay(1);
		}
	}

	// Convert from segmentDisplay to segmentImage
	for (int imageIndex = 0; imageIndex < IMAGES; imageIndex++)
	{
		ssd1306_bitmaps(&dev, 0, 8, segmentDisplay[imageIndex], 32, 48, false);

		// Get from internal buffer to local buffer
		// buffer is [8][128] 8 page 128 pixel
		ssd1306_get_buffer(&dev, buffer);

		// Save from buffer to segmentImage
		// segmentImage is [10][8][32] 10 image 8 page 32 pixel
		int segmentImageIndex = imageIndex * 256;
		for (int page = 0; page < 8; page++)
		{
			memcpy(&segmentImage[segmentImageIndex + page * 32], &buffer[page * 128], 32);
		}
	}

	// I don't use this anymore
	free(buffer);

	int digit1 = 0;
	int digit2 = 0;
	int digit3 = 0;
	int digit4 = 0;
	int number = 0;

	ssd1306_clear_screen(&dev, false);
	for (int page = 0; page < 8; page++)
	{
		ssd1306_display_image(&dev, page, 0, &segmentImage[page * 32], 32);
		ssd1306_display_image(&dev, page, 32, &segmentImage[page * 32], 32);
		ssd1306_display_image(&dev, page, 64, &segmentImage[page * 32], 32);
		ssd1306_display_image(&dev, page, 96, &segmentImage[page * 32], 32);
		vTaskDelay(2);
	}

	while (1)
	{
		number = current_value;

		digit4 = number % 10; // Get the last digit
		number /= 10;		  // Remove the last digit from the number

		digit3 = number % 10; // Get the next digit
		number /= 10;		  // Remove the next digit from the number

		digit2 = number % 10; // Get the next digit
		number /= 10;		  // Remove the next digit from the number

		digit1 = number % 10; // Get the first digit

		// Update digit1
		int segmentImageIndex1 = digit1 * 256;
		for (int page = 0; page < 8; page++)
		{
			ssd1306_display_image(&dev, page, 00, &segmentImage[segmentImageIndex1 + page * 32], 32);
		}
		// Update digit2
		int segmentImageIndex2 = digit2 * 256;
		for (int page = 0; page < 8; page++)
		{
			ssd1306_display_image(&dev, page, 32, &segmentImage[segmentImageIndex2 + page * 32], 32);
		}
		// Update digit3
		int segmentImageIndex3 = digit3 * 256;
		for (int page = 0; page < 8; page++)
		{
			ssd1306_display_image(&dev, page, 64, &segmentImage[segmentImageIndex3 + page * 32], 32);
		}
		// Update digit4
		int segmentImageIndex4 = digit4 * 256;
		for (int page = 0; page < 8; page++)
		{
			ssd1306_display_image(&dev, page, 96, &segmentImage[segmentImageIndex4 + page * 32], 32);
		}
		vTaskDelay(10);
	} // end while
}

int32_t display_init(void)
{
	xTaskCreate(display_task, "display_task", 4096, NULL, uxTaskPriorityGet(NULL), NULL);
	return EXIT_SUCCESS;
}

void display_set(uint8_t value)
{
	current_value = value;
}