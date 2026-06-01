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

void display_write_column_shift(uint8_t index, uint16_t scroll_pos) {
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

    // when scroll address = 33 -> address 32 must be re writeen
    // when scroll address = 34 -> address 33 must be re writeen
    // when scroll addrres = lcd_width -32 -> address lcd_width -33 must be re writeen
    // when scroll addresss = 32 -> address lcd_width -32 must be re writeen

    // symplify

    // when scroll = 0 -> address 255 must be re written
    // when scroll = 8 -> address 7 must be re writeen

    // when scroll 0 -> the memory at 32 (memory space) is at 32 (pysical space)
    // when scroll 0 -> the memory at 40 (memory space) is at 40 (pysical space)

    // when scroll 1 -> the memory at 32 (memory space) is at 287 (pysical space)
    // when scroll 1 -> the memory at 33 (memory space) is at 32 (pysical space)
    // when scroll 1 -> the memory at 40 (memory space) is at 39 (pysical space)

    // when scroll 8 -> the memory at 40 (memory space) is at 32 (pysical space)
    // when scroll 8 -> the memory at 44 (memory space) is at 36 (pysical space)
    // when scroll 8 -> the memory at 32 (memory space) is at 280 (pysical space)
    // when scroll 8 -> the memory at 39 (memory space) is at 287 (pysical space)

    // translation
    // memory space (scroll) -> physical space : phy_x = mem_x - scroll ------------> mem_x = phy_x + scroll -> mem_x = (camera_x + offset) + scroll

    /* 
    phy_x = mem_x - scroll
    if (phy_x < 0) {
        phy_x = 288 - phy_x

    }
    */ 
    
    /*
    mem_x = phy_x + scroll
    if (mem_x > 287) {
    mem_x = mem_x - 288

    }
    */

    /*
    temp = cam_x + scroll
    if (temp > 255) {
    temp = temp - 255

    }
    mem_x = temp + 32;
    */

    /*
    uint8_t temp = cam_x + scroll
    uint16_t mem_x = (uint16_t)temp + 32;
    */

    // camera space -> 


    // address translation:
    // world space -> camera space -> lcd physical space -> lcd memory space
    // world space: (world_x, world_y) -> (400, 200); 
    // camera space: camera_pox_x -> (world_x - camera_pos_x, world_y); camera space is defined (0:255,0:239), everything else is culled;
    // lcd physical space: lcd_offset_x -> (camera_x + lcd_offset_x, camera_y); lcd_physcal space is defined (32:287,0:239)
    // lcd memory space: scroll_pos -> ()
    

    
    uint16_t current_color = colors[index];
    lcd_cmd_page_set(lcd_width-32, lcd_width-32);
    
    

}