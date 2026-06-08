#pragma once
#include <stdint.h>
#include <stdbool.h>

const static unsigned pixel_count = 61440;
const static unsigned lcd_width = 320;
const static unsigned lcd_height = 240;
const static unsigned window_width = 256;
const static unsigned window_height = 240;
const static unsigned lcd_offset = 32;

typedef enum {
    SOFTWARE_RESET = 0x01,
    READ_DISPLAY_STATUS = 0x09,
    SLEEP_OUT = 0x11, // wait 5ms after command
    PARTIAL_MODE_ON = 0x12,
    DISPLAY_INVERSION_ON = 0x21,
    DISPLAY_ON = 0x29,
    COLUMN_ADDRESS_SET = 0x2A,
    PAGE_ADDRESS_SET = 0x2B,
    MEMORY_WRITE = 0x2C,
    PARTIAL_AREA = 0x30,
    VERTICAL_SCROLLING_DEFINITION = 0x33,
    MEMORY_ACCESS_CONTROL = 0x36,
    VERTICAL_SCROLLING_START_ADDRESS = 0x37,
    PIXEL_FORMAT_SET = 0x3A,

} LcdCmd;

typedef struct {
    unsigned padding3 : 5;
    bool tearing_effect_line_mode : 1;
    unsigned gamma_curve : 3;
    bool tearing_effect_line : 1;
    bool display_on : 1;
    bool all_pixel_off : 1;
    bool all_pixel_on : 1;
    bool inversion_status : 1;
    bool padding2 : 1;
    bool vertical_scrolling_status : 1;
    bool display_normal_mode : 1;
    bool sleep_out : 1;
    bool partial_mode : 1;
    bool idle_mode : 1;
    unsigned pixel_format : 3;
    unsigned padding1 : 2;
    bool horizontal_refresh_order : 1;
    bool rgb_bgr_order : 1;
    bool vertical_refresh : 1;
    bool row_column_exchange : 1;
    bool column_address_order : 1;
    bool row_address_order : 1;
    bool booster_voltage_status : 1;
} ILI9341_status_fields;

typedef union {
    uint32_t raw_data;
    ILI9341_status_fields fields;
} ILI9341_status;

void lcd_init();

void lcd_send_data(uint8_t byte);
void lcd_send_datas(const uint8_t* bytes, uint16_t count);
void lcd_send_data_repeat(uint8_t byte, uint16_t repetitions);
void lcd_send_wdata(uint16_t word);
void lcd_send_wdata_repeat(uint16_t word, uint16_t repetitions);
void lcd_send_command(LcdCmd command);

void lcd_recive_byte(uint8_t* byte);
void lcd_recive_long(uint32_t* l);

void lcd_cmd_read_display_status(ILI9341_status* ptr);
void lcd_cmd_column_set(uint16_t sc, uint16_t ec);
void lcd_cmd_page_set(uint16_t sp, uint16_t ep);
void lcd_cmd_vertical_scrolling_definition(uint16_t tfa, uint16_t vsa, uint16_t bfa);
void lcd_cmd_vertical_scrolling_start_address(uint16_t vsp);
