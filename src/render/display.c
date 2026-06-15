#include "ili9341.h"
#include "display.h"

#include <driverlib.h>

display_data display;

void display_init() {
    lcd_init();
    display.camera_pos = 0;
    display.scroll_count = 0;
    display.new_columns = 0;
}

void display_draw_solid_frame(uint16_t color) {
    lcd_send_command(MEMORY_WRITE);
    lcd_send_wdata_repeat(color, pixel_count);
}

void display_test1() {
    uint16_t colors[] = {
        0x001f,
        0x07E0,
        0xF800,
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

    lcd_cmd_column_set(32, lcd_width/2 - 1);
    lcd_send_command(MEMORY_WRITE);
    lcd_send_wdata_repeat(colors[0], lcd_height/2*(lcd_width-64));

    lcd_cmd_column_set(lcd_width/2, lcd_width-33);
    lcd_send_command(MEMORY_WRITE);
    lcd_send_wdata_repeat(colors[1], lcd_height/2*(lcd_width-64));

    lcd_cmd_column_set(0, 0);
    while (1) {
        current_color = colors[a & 3];
        for (int scroll_pos = 32; scroll_pos < lcd_width-32; scroll_pos +=3) {
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

        lcd_cmd_column_set(coord, coord);
        lcd_send_command(MEMORY_WRITE);
        lcd_send_wdata_repeat(colors[scroll_count & 15], 2*lcd_height);

        scroll_count += 2;
    }
}

void display_camera_add(uint16_t x) {
    display.camera_pos += x;
    display.scroll_count += x;
    display.new_columns = x;

    lcd_cmd_vertical_scrolling_start_address(32+display.scroll_count);
}

void display_render_new_columns(color_picker fun) {
    for (int i = 0; i < display.new_columns; i++) {
        world_coord coord = coord_camera_to_world(255-i, display.camera_pos);

        memory_coord mem_coord = coord_camera_to_memory(255-i, display.scroll_count);
        lcd_cmd_column_set(mem_coord, mem_coord);

        lcd_send_command(MEMORY_WRITE);

        int j = 240;
        while (j--) {
            lcd_send_wdata(fun(coord, j));
        }
    }
}

void display_render_new_columns16(color_picker fun) {
    for (int i = 0; i < display.new_columns; i++) {
        world_coord coord = coord_camera_to_world(255-i, display.camera_pos);

        memory_coord mem_coord = coord_camera_to_memory(255-i, display.scroll_count);
        lcd_cmd_column_set(mem_coord, mem_coord);

        lcd_send_command(MEMORY_WRITE);

        int j = 240/16;
        while (j--) {
            lcd_send_wdata_repeat(fun(coord, j), 16);
        }
    }
}

void display_render_all(color_picker fun) {
    for (int i = 0; i <= 255; i++) {
        world_coord coord = coord_camera_to_world(255-i, display.camera_pos);

        memory_coord mem_coord = coord_camera_to_memory(255-i, display.scroll_count);
        lcd_cmd_column_set(mem_coord, mem_coord);

        lcd_send_command(MEMORY_WRITE);

        int j = 240;
        while (j--) {
            lcd_send_wdata(fun(coord, j));
        }
    }
}

#include "map/metatileset.h"
#include "map/metatilemap.h"
#include "coordinates.h"

void display_render_new_columns_metatilemap() {
    assert(display.new_columns <= 16);

    if (display.new_columns == 0) {
        return;
    }

    camera_coord pos = 255-display.new_columns+1;
    world_coord world_pos = coord_camera_to_world(pos, display.camera_pos);

    volatile uint16_t time_coords = 0;
    volatile uint16_t time_spi =  0;
    volatile uint16_t time_buffer = 0;

    for (uint16_t i = pos; i <= 255; i++) {
        TA0CTL |= TACLR;
        memory_coord mem = coord_camera_to_memory(i, display.scroll_count);

        lcd_cmd_column_set(mem, mem);

        const Metatile* metatile_current = metamap_tile_getref(&metamap1, world_pos, 0);

        uint8_t col = world_pos & 0xf;
        uint16_t* buf = display.buffer;

        lcd_send_command(MEMORY_WRITE);

        time_coords += TA0R;

        for (int i = 0; i < 15; i++) {
            TA0CTL |= TACLR;
            metatile_col_copy(*metatile_current, buf, col);
            metatile_current += metamap1.width;
            time_buffer += TA0R;
            buf += 16;
        }

        // send buffer
        TA0CTL |= TACLR;
        lcd_send_wdatas(display.buffer, 240);
        time_spi += TA0R;

        world_pos++;
    }
}

color test_color_picker(world_coord x, uint8_t y) {
    static const color colors[] = {
        0x001f,
        0x07E0,
        0xF100,
        0xffff,
    };

    return colors[(x >> 2) & 3];
}
