#include <driverlib.h>
#include "display.h"
#include "ili9341.h"
#include "coordinates.h"


display_data display;

void display_init() {
    lcd_init();
    display.camera_pos = 0;
    display.scroll_count = 0;
    display.new_columns = 0;
        
    DMACTL0 = DMA0TSEL_19;
    DMACTL4 = DMARMWDIS; // makes debugging more stable
    DMA0CTL = 0 
      | DMADT_1
      | DMADSTINCR_0
      | DMASRCINCR_3
      | DMADSTBYTE
      | DMASRCBYTE
      | DMALEVEL
      ;

    __data20_write_long((uintptr_t) &DMA0DA,(uintptr_t) &UCB0TXBUF);
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

    volatile uint16_t time_spi = 0;
    volatile uint16_t time_buffer = 0;
    uint16_t temp = 0;

    while (DMA0CTL & DMAEN);
    __data20_write_long((uintptr_t) &DMA0SA,(uintptr_t) display.buffer);
    DMA0SZ = 480;
    
    lcd_cmd_page_set(0, 239);

    for (uint16_t i = pos; i <= 255; i++) {
        temp = TA0R;
        while(DMA0CTL & DMAEN);
        time_spi += TA0R - temp;

        memory_coord mem = coord_camera_to_memory(i, display.scroll_count);

        lcd_cmd_column_set(mem, mem);

        const Metatile* metatile_current = metamap_metatile_getref(&metamap1, world_pos, 0);

        uint8_t col = world_pos & 0xf;
        uint16_t* buf = display.buffer;

        lcd_send_command(MEMORY_WRITE);


        for (int i = 0; i < 15; i++) {
            temp = TA0R;
            metatile_col_copy(*metatile_current, buf, col);
            metatile_current += metamap1.width;
            time_buffer += TA0R - temp;
            buf += 16;
        }

        // send buffer
        lcd_prepare_data();
        DMA0CTL |= DMAEN;

        world_pos++;
    }
    __no_operation();
}

#include "sprite.h"

void display_render_dirty_sprites() {
    display.dirty_8x8[3][2] = 0xff;
    display.dirty_8x8[3][3] = 0xff;

    while (DMA0CTL & DMAEN);
    __data20_write_long((uintptr_t) &DMA0SA,(uintptr_t) display.buffer);
    DMA0SZ = 16;

    for (uint8_t i = 0, y = 0; i < 30; i++, y += 8) {
        while(DMA0CTL & DMAEN);
        lcd_cmd_page_set(y, y+7);

        for (int j = 0; j < 4; j++) {
            uint8_t offset_x = j*64;
            uint8_t offset2_x = 0;
            uint8_t index = 1;

            while (display.dirty_8x8[j][i]) {
                // we have something to render
                if (display.dirty_8x8[j][i] & index) {
                    // process
                    camera_coord x = offset_x + offset2_x*8;

                    for (int k = 0; k < 8; k++) {
                        memory_coord mem = coord_camera_to_memory(x+k, display.scroll_count);

                        while(DMA0CTL & DMAEN);
                        lcd_cmd_column_set(mem, mem);

                        lcd_prepare_data();
                        DMA0CTL |= DMAEN;
                    }

                    display.dirty_8x8[j][i] &= ~index;
                }
                index = index << 1;
                offset2_x++;
            }
        }
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
