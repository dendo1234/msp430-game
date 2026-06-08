#include <assert.h>
#include "tileset.h"

void tile_ret_copy(Tile8x8 tile, uint16_t* destination, uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2) {
    assert(x1 < 8);
    assert(y1 < 8);
    assert(x2 < 8);
    assert(y2 < 8);

    for (uint8_t j = y1; j <= y2; j++) {
        for (uint8_t i = x1; i <= x2; i++) {
            *destination++ = tile[j*8+i];
        }
    }
}
