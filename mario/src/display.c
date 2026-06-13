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

    volatile uint16_t time_coords = 0;
    volatile uint16_t time_spi =  0;
    volatile uint16_t time_buffer = 0;

    #pragma MUST_ITERATE(1,3,1)
    do {
        TA0CTL |= TACLR;
        // 1 compute tile boundaries
        memory_coord mem1 = coord_camera_to_memory(pos, display.scroll_count);
        memory_coord mem2 = coord_camera_to_memory(255, display.scroll_count);

        if (mem1 > mem2) {
            // hit a memory barrier
            mem2 = 287;
        }

        camera_coord pos2 = coord_memory_to_camera(mem2, display.scroll_count);

        world_coord world_start = coord_camera_to_world(pos, display.camera_pos);
        world_coord world_end = coord_camera_to_world(pos2, display.camera_pos);
        world_coord tile_next_start = (world_start + 0x10) & ~0xf;

        uint16_t x1 = world_start & 0xf;
        uint16_t x2;

        if ((world_start ^ world_end) & 0x10) {
            // direfent tiles, move pos2 back
            world_end = tile_next_start-1;
            pos2 = coord_world_to_camera(world_end, display.camera_pos);
            mem2 = coord_camera_to_memory(pos2, display.scroll_count);
            x2 = 0xf;
        } else {
            x2 = world_end & 0xf;
        }

        lcd_cmd_column_set(mem1, mem2);
        lcd_send_command(MEMORY_WRITE);

        uint16_t buffer_size = (x2-x1+1)*16;

        const Metatile* metatile_current = metamap_tile_getref(&metamap1, world_start, 0);

        time_coords += TA0R;

        for (int i = 0; i < 15; i++) {
            TA0CTL |= TACLR;
            metatile_ret_copy_vert(*metatile_current, display.buffer, x1, x2);
            metatile_current += metamap1.width;
            time_buffer += TA0R;

            TA0CTL |= TACLR;
            // send buffer
            lcd_send_wdatas(display.buffer, buffer_size);
            time_spi += TA0R;
        }

        pos = pos2+1;

    } while (pos != 0);
}

// void display_render_new_columns_metatilemap() {
//     assert(display.new_columns <= 16);

//     memory_coord mem_coord_end = coord_camera_to_memory(255, display.scroll_count);
//     memory_coord mem_coord_start = coord_camera_to_memory(255-display.new_columns, display.scroll_count);

//     Metatile_Rect rect;
//     rect.y1 = 0;
//     rect.y2 = 15;

//     if (mem_coord_end < mem_coord_start) {
//         // bad ending
//         lcd_cmd_column_set(mem_coord_start, mem_coord_max);

//         world_coord coord_end = coord_memory_to_world(mem_coord_max, display.scroll_count, display.camera_pos);
//         world_coord coord_start = coord_end - (mem_coord_max - mem_coord_start);

//         lcd_send_command(MEMORY_WRITE);

//         #pragma MUST_ITERATE(1,2,1)
//         for (int i = coord_start; i < coord_end; i += 1 << 4) {
//             if (i == coord_start) {
//                 rect.x1 = coord_start & 0xf;
//                 rect.x2 = coord_end & 0xf < coord_start & 0xf ? 0xf : coord_end & 0xf;
//             } else {
//                 rect.x1 = 0;
//                 rect.x2 = coord_end & 0xf;
//             }

//             uint8_t bufsize = (rect.x2 - rect.x1 + 1) * 16;
//             for (int j = 0; j < 240; j += 16) {
//                 Metatile tile = metamap_tile_get(&metamap1, coord_start, j);
//                 metatile_ret_copy(tile, display.buffer, rect);
//                 lcd_send_wdatas(display.buffer, bufsize);
//             }
//         }
//         mem_coord_start = mem_coord_min;
//     }

//     world_coord coord_end = coord_camera_to_world(255, display.camera_pos);
//     world_coord coord_start = coord_memory_to_world(mem_coord_start, display.scroll_count, display.camera_pos);

//     lcd_cmd_column_set(mem_coord_start, mem_coord_end);

//     uint16_t div1 = mem_coord_start & 0xf;
//     if ((coord_end & 0xf) < (coord_start & 0xf)) {
        
//     }

//     world_coord world_div = (coord_start & 0xfff0) + 0x10;
//     memory_coord mem_div = coord_camera_to_memory(coord_world_to_camera(world_div, display.camera_pos), display.scroll_count);

//     // for every tile in range (coord_start -> coord_end)
//     // get memory bounds (assume that no memory border can occour)
//     // copy data


//     #pragma MUST_ITERATE(1,2,1)
//     for (int i = coord_start; i < coord_end; i = (i + (1 << 4)) & 0xfff0) {
//         lcd_cmd_column_set(mem_coord_start, mem_div);
//         if (i == coord_start) {

//             rect.x1 = coord_start & 0xf;
//             rect.x2 = (coord_end & 0xf) < (coord_start & 0xf) ? 0xf : coord_end & 0xf;
//         } else {
//             rect.x1 = 0;
//             rect.x2 = coord_end & 0xf;
//         }

//         lcd_send_command(MEMORY_WRITE);

//         uint8_t bufsize = (rect.x2 - rect.x1 + 1) * 16;
//         for (int j = 0; j < 240; j += 16) {
//             Metatile tile = metamap_tile_get(&metamap1, coord_start, j);
//             metatile_ret_copy(tile, display.buffer, rect);
//             lcd_send_wdatas(display.buffer, bufsize);
//         }
//         mem_coord_start = mem_div;
//         mem_div = mem_coord_end;
//     }
// }

color test_color_picker(world_coord x, uint8_t y) {
    static const color colors[] = {
        0x001f,
        0x07E0,
        0xF100,
        0xffff,
    };

    return colors[(x >> 2) & 3];
}
