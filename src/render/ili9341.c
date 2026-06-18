#include <driverlib.h>
#include <assert.h>
#include "spi.h"
#include "ili9341.h"


/*

P3.0 MOSI
P3.1 MISO
P3.2 SPICLK
P2.6 CS
P8.1 RESET
P8.2 DC

*/

// #define assert(exp) do { \
//     if (!exp) while(1); \
// } while (0)

#define RESX_high() P8OUT |= BIT1
#define RESX_low() P8OUT &= ~BIT1

// shouldn't change DCX while transminting data
#define DCX P8OUT & BIT2
#define DCX_high() do { \
    assert(!(UCB0STAT & UCBUSY)); \
    P8OUT |= BIT2; \
} while(0)
#define DCX_low() do { \
    assert(!(UCB0STAT & UCBUSY)); \
    P8OUT &= ~BIT2; \
} while(0)
#define DCX_assign(bit) do { \
    assert(!(UCB0STAT & UCBUSY)); \
    assert(bit == 0 || bit == 1); \
    P8OUT ^= (-bit ^ P8OUT) & BIT2; \
} while(0)

#define CSX_high() P2OUT |= BIT6
#define CSX_low() P2OUT &= ~BIT6

void lcd_prepare_data() {
    spi_wait_not_busy();
    DCX_high();
}

void lcd_send_data(uint8_t byte) {
    spi_wait_not_busy();
    DCX_high();
    spi_send_byte(byte);
}

void lcd_send_data_repeat(uint8_t byte, uint16_t repetitions) {
    spi_wait_not_busy();
    DCX_high();
    while(repetitions--) {
        spi_send_byte(byte);
    }
}

void lcd_send_wdata(uint16_t word) {
    spi_wait_not_busy();
    DCX_high();
    spi_send_byte(word >> 8);
    spi_send_byte(word);
}

void lcd_send_wdata_repeat(uint16_t word, uint16_t repetitions) {
    spi_wait_not_busy();
    DCX_high();
    while(repetitions--) {
        spi_send_byte(word >> 8);
        spi_send_byte(word);
    }
}

void lcd_send_datas(const uint8_t* bytes, uint16_t count) {
    spi_wait_not_busy();
    DCX_high();
    while(count--) {
        spi_send_byte(*bytes++);
    }
}

void lcd_send_wdatas(const uint16_t* words, uint16_t count) {
    spi_wait_not_busy();
    DCX_high();
    while(count--) {
        spi_send_byte(*words >> 8);
        spi_send_byte(*words++);
    }
}

void lcd_send_command(LcdCmd command) {
    spi_wait_not_busy();
    DCX_low();
    spi_send_byte((uint8_t)command);
}

void lcd_recive_byte(uint8_t* byte) {
    spi_wait_not_busy();
    DCX_high();
    spi_recive_byte(byte);
}

void lcd_recive_long(uint32_t* l) {
    spi_wait_not_busy();
    DCX_low();
    spi_recive_long(l);
}

void lcd_spi_reset() {
    CSX_high();
    __delay_cycles(2);
    CSX_low();
    __delay_cycles(2);
}

void lcd_cmd_read_display_status(ILI9341_status* ptr) {
    lcd_send_command(READ_DISPLAY_STATUS);

    volatile ILI9341_status status;
    status.raw_data = 0;

    uint8_t byte;
    lcd_recive_byte(&byte);
    status.raw_data |= (uint32_t)byte << 25; 

    uint32_t l;
    lcd_recive_long(&l);
    status.raw_data |= l >> 7;

    // discard the extra 7 bits sent
    lcd_spi_reset();

    if (ptr) {
        ptr->raw_data = status.raw_data;
    }
}

void lcd_cmd_pixel_format_set() {
    lcd_send_command(PIXEL_FORMAT_SET);
    lcd_send_data(0x55); //pixel format 16-bit
}

void lcd_cmd_partial_mode() {
    lcd_send_command(PARTIAL_AREA);
    const static uint8_t data[] = {
        0x00, 0x00 + 32, // SR
        0x01, 0x3F - 32,  // ER
    };
    lcd_send_datas(data, 4);
    lcd_send_command(PARTIAL_MODE_ON);
}

void lcd_cmd_column_set(uint16_t sc, uint16_t ec) {
    assert(sc <= ec);
    assert(ec < 0xF0);
    lcd_send_command(COLUMN_ADDRESS_SET);
    lcd_send_wdata(sc);
    lcd_send_wdata(ec);
}

void lcd_cmd_page_set(uint16_t sp, uint16_t ep) {
    assert(sc <= ec);
    assert(ec < 0x140);
    lcd_send_command(PAGE_ADDRESS_SET);
    lcd_send_wdata(sp);
    lcd_send_wdata(ep);
}

void lcd_cmd_vertical_scrolling_definition(uint16_t tfa, uint16_t vsa, uint16_t bfa) {
    assert(tfa + vsa + bfa == 320);
    lcd_send_command(VERTICAL_SCROLLING_DEFINITION);
    lcd_send_wdata(tfa);
    lcd_send_wdata(vsa);
    lcd_send_wdata(bfa);
}

void lcd_cmd_memory_access_control(MADCTL mad) {
    lcd_send_command(MEMORY_ACCESS_CONTROL);
    lcd_send_data(mad.raw_data);
}

void lcd_cmd_vertical_scrolling_start_address(uint16_t vsp) {
    lcd_send_command(VERTICAL_SCROLLING_START_ADDRESS);
    lcd_send_wdata(vsp);
}

void lcd_reset_hw() {
    RESX_low();
    CSX_high();
    //wait at least 10 us
    __delay_cycles(25*10);
    RESX_high();

    //wait at least 5000 us
    __delay_cycles((long)25*5000);
    lcd_spi_reset();
}

void lcd_reset_sw() {
    lcd_spi_reset();
    lcd_send_command(SOFTWARE_RESET);
    //wait at least 5000 us
    __delay_cycles((long)25*5000);
}

void lcd_reset() {
    lcd_reset_hw();
    lcd_reset_sw();
}

void lcd_init() {
    spi_init();

    {
        P2DIR |= BIT6; // CS
        P8DIR |= BIT1 | BIT2; // RESET DC
        P2OUT |= BIT6;
        P8OUT |= BIT1 | BIT2; // RESET DC
    }

    ILI9341_status status;
    lcd_reset();
    lcd_cmd_read_display_status(&status);
    if (status.raw_data != 0x00610000) {
        // unexpected display status
        // while(1);
    }

    lcd_send_command(SLEEP_OUT);
    __delay_cycles((long)25*5000);
    lcd_send_command(DISPLAY_ON);

    lcd_cmd_pixel_format_set();
    // lcd_cmd_partial_mode();

    MADCTL mad = {
        .fields = {
            .bgr_order = 1, // I have no ideia why is bgr, not rgb
        }
    };
    lcd_cmd_memory_access_control(mad);

    lcd_cmd_vertical_scrolling_definition(32, lcd_width-64, 32);
    lcd_cmd_vertical_scrolling_start_address(0);

    lcd_cmd_read_display_status(&status);
    // if (status.raw_data != 0x80560400) {
    //     // unexpected display status
    //     while(1);
    // }

}
