#include "ili9341.h"


void display_init() {
    lcd_init();
}

void display_draw_solid_frame(uint16_t color) {
    lcd_send_command(MEMORY_WRITE);
    lcd_send_wdata_repeat(color, pixel_count);
}

void display_test1() {
    uint16_t colors[] = {
        0x001f,
        0x07E0,
        0xF100,
        0xffff,
    };
    uint16_t current_color;
    int a = 0;

    while (1) {
        current_color = colors[a & 3];
        lcd_send_command(MEMORY_WRITE);
        lcd_send_wdata_repeat(current_color, pixel_count);
        a++;
    }
}

void display_test2() {
    uint16_t colors[] = {
        0x001f,
        0x07E0,
        0xF100,
        0xffff,
    };
    uint16_t current_color;
    int a = 0;

    lcd_cmd_page_set(32, lcd_width/2 - 1);
    lcd_send_command(MEMORY_WRITE);
    lcd_send_wdata_repeat(colors[0], lcd_height/2*(lcd_width-64));

    lcd_cmd_page_set(lcd_width/2, lcd_width-33);
    lcd_send_command(MEMORY_WRITE);
    lcd_send_wdata_repeat(colors[1], lcd_height/2*(lcd_width-64));

    lcd_cmd_page_set(0, 0);
    while (1) {
        current_color = colors[a & 3];
        for (int scroll_pos = 32; scroll_pos < lcd_width-32; scroll_pos++) {
            lcd_cmd_vertical_scrolling_start_address(scroll_pos);
            // lcd_send_command(MEMORY_WRITE);
            __delay_cycles(5000);
        }
    }

}

#include "coordinates.h"

void display_test3() {
    const uint16_t colors[] = {
        0xF800, // Red
        0xFAA0, // Orange-red
        0xFD20, // Orange
        0xFF80, // Yellow-orange
        0xFFE0, // Yellow
        0xAFE0, // Yellow-green
        0x57E0, // Green
        0x07E8, // Green-cyan
        0x07FF, // Cyan
        0x055F, // Cyan-blue
        0x02BF, // Blue
        0x501F, // Blue-violet
        0xA81F, // Violet
        0xD81F, // Magenta
        0xF90F, // Pink
        0xF800, // Back to red
    };

    uint8_t scroll_count = 0;

    while (1) {
        lcd_cmd_vertical_scrolling_start_address(32+scroll_count);

        memory_coord coord = coord_camera_to_memory(255, scroll_count);

        lcd_cmd_page_set(coord, coord);
        lcd_send_command(MEMORY_WRITE);
        lcd_send_wdata_repeat(colors[scroll_count & 15], 2*lcd_height);

        scroll_count += 2;
    }
}
