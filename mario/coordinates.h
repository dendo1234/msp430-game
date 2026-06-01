/*
    I probaly spent way to much time think about the diferent cordinate systems present on this
    but regardless, the conversions are probaly as optimazed as it can get.

    I left as comments what I was thinking
*/
    // when scroll address = 33 -> address 32 must be re writeen
    // when scroll address = 34 -> address 33 must be re writeen
    // when scroll addrres = lcd_width -32 -> address lcd_width -33 must be re writeen
    // when scroll addresss = 32 -> address lcd_width -32 must be re writeen

    // scroll address = (32:lcd_widht-1-32) = (32:320-1-32) = (32:287)

    // symplify
    // scroll_addr = scroll_pos + 32
    // scroll_pos = scroll_add - 32

    // scroll = (32-32:287-32) = (0:255)

    // when scroll 0 -> the memory at 32 (memory space) is at 32 (pysical space)
    // when scroll 0 -> the memory at 40 (memory space) is at 40 (pysical space)

    // when scroll 1 -> the memory at 32 (memory space) is at 287 (pysical space) is at 255 (camera space)
    // when scroll 1 -> the memory at 33 (memory space) is at 32 (pysical space) is at 0 (camera space)
    // when scroll 1 -> the memory at 40 (memory space) is at 39 (pysical space) is at 7 (camera space)

    // if camera_pos = 10
    // when scroll 8 -> the memory at 40 (memory space) is at 32 (pysical space) is at 0 (camera space) 10 world
    // when scroll 8 -> the memory at 44 (memory space) is at 36 (pysical space) is at 4 (camera space) 14 world
    // when scroll 8 -> the memory at 32 (memory space) is at 280 (pysical space) is at 248 (camera space) 258 world
    // when scroll 8 -> the memory at 39 (memory space) is at 287 (pysical space) is at 255 (camera space) 265 world

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
    // lcd memory space: scroll_pos -> ((uint16_t)((uint8_t)(physical_x-32) + scroll) + 32); defined(32:287, 0:239)

#include "ili9341.h"

typedef uint16_t world_coord;
typedef uint8_t camera_coord;
typedef uint16_t physcal_coord;
typedef uint16_t memory_coord;


// typedef int16_t camera_coord;

// camera_coord coord_world_to_camera_notc(world_coord x, world_coord camera_pos) {
//     return x - camera_pos;
// }

// bool coord_camera_coord_cull(camera_coord x) {
//     return !(x & 0xff00);
// }

// physcal_coord coord_camera_to_physical(camera_coord x) {
//     assert(x > 0 && x < 256);
//     return x + lcd_offset;
// }

// memory_coord coord_physical_to_memory(physcal_coord x, uint16_t scroll_pos) {
//     return ((x-32+scroll_pos) & 0xff) + 32;
// }

// world_coord coord_memory_to_world(memory_coord x, uint16_t scroll_pos, camera_coord camera_pos) {
//     return uint16_t((uint8_t)(x - lcd_offset - scroll_pos)) + camera_pos;
// }

/*
@inputs: 
    x: position of pixel memory-wise (32:287)
    scroll_addr: (32:287)
    camera_pos: world_pos

*/
// world_coord coord_memory_to_world(memory_coord x, uint16_t scroll_addr, world_pos camera_pos) {

// }

bool coord_cull(world_coord x, world_coord camera_pos) {
    return (x - camera_pos) < 256;
}

/*
@input:
    x: position of pixel in camera space, if it's visible: (0:255)
    camera_pos: position of the leftmost visible in screen on world space
*/
world_coord coord_camera_to_world(camera_coord x, world_coord camera_pos) {
    return x + camera_pos;
}

/*
@input:
    x: position of pixel in camera space, if it's visible: (0:255)
    scroll_count: number of lines scrolled
*/
memory_coord coord_camera_to_memory(camera_coord x, uint8_t scroll_count) {
    return (uint16_t)(x+scroll_count) + 32;
}

/*
@input:
    x: position of pixel in camera space, if it's visible: (0:255)
    scroll_count: number of lines scrolled
    camera_pos: position of the leftmost visible in screen on world space
*/
memory_coord coord_world_to_memory(world_coord x, uint8_t scroll_count, world_coord camera_pos) {
    return (uint8_t)(x-camera_pos+scroll_count) + 32;
}