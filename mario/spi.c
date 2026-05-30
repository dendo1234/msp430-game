#include <driverlib.h>
#include <assert.h>
#include <limits.h>
#include "spi.h"

// #define assert(exp) do { \
//     if (!exp) while(1); \
// } while (0)

/*

P3.0 MOSI
P3.1 MISO
P3.2 SPICLK
P2.6 CS
P8.1 RESET
P8.2 DC

*/

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
    P8OUT &= ~BIT2 \
} while(0)
#define DCX_assign(bit) do { \
    assert(!(UCB0STAT & UCBUSY)); \
    assert(bit == 0 || bit == 1); \
    P8OUT ^= (-bit ^ P8OUT) & BIT2; \
} while(0)

#define CSX_high() P2OUT |= BIT6
#define CSX_low() P2OUT &= ~BIT6

typedef enum {
    SOFTWARE_RESET = 0x01,
    READ_DISPLAY_STATUS = 0x09,
    SLEEP_OUT = 0x11, // wait 5ms after command
    PARTIAL_MODE_ON = 0x12,
    DISPLAY_INVERSION_ON = 0x21,
    DISPLAY_ON = 0x29,
    MEMORY_WRITE = 0x2C,
    PARTIAL_AREA = 0x30,
    PIXEL_FORMAT_SET = 0x3A,

} LcdCmd;

static const uint8_t dummy_byte = 0;

void spi_init() {
    UCB0CTL1 = UCSWRST; {
        UCB0CTL0 = 0
        | UCCKPH
        // | UCCKPL
        | UCMSB
        // | UC7BIT
        | UCMST
        | UCMODE_0
        | UCSYNC
        ;

        UCB0CTL1 = 0
        | UCSSEL__SMCLK 
        | UCSWRST
        ;

        UCB0BRW = 0; // fsmclk = fspiclk
    } UCB0CTL1 &= ~UCSWRST;

    UCB0IE = 0
    //   | UCTXIE
    //   | UCRXIE
      ;

    // ports
    { 
        P3SEL |= BIT0 | BIT1 | BIT2; // MOSI MISO SCLK
    }
}

void spi_wait_not_busy() {
    while (UCB0STAT & UCBUSY);
}

void spi_send_byte(uint8_t byte) {
    while ((UCB0IFG & UCTXIFG) == 0); //waiting data to transmit
    UCB0TXBUF = byte;
}

void spi_recive_byte(uint8_t* byte) {
    volatile uint8_t flush = UCB0RXBUF;
    spi_send_byte(dummy_byte); // send to recive

    while ((UCB0IFG & UCRXIFG) == 0); // wait to be full
    *byte = UCB0RXBUF;
}

void spi_recive_long(uint32_t* l) {
    volatile uint8_t flush = UCB0RXBUF;
    // msb first
    uint8_t* byte_ptr = (uint8_t*)l;
    int i = 3;
    while (i >= 0) {
        spi_send_byte(dummy_byte);

        while ((UCB0IFG & UCRXIFG) == 0);
        byte_ptr[i--] = UCB0RXBUF;
    }
}

void lcd_send_byte(uint8_t byte, bool is_data) {
    DCX_assign(is_data);
    spi_send_byte(byte);
}

void lcd_send_bytes(uint8_t* bytes, uint16_t count, bool is_data) {
    DCX_assign(is_data);
    while(count--) {
        spi_send_byte(*bytes++);
    }
}

void lcd_send_command(LcdCmd command) {
    lcd_send_byte((uint8_t)command, false);
}

void lcd_recive_byte(uint8_t* byte) {
    assert(DCX);
    spi_recive_byte(byte);
}

void lcd_recive_long(uint32_t* l) {
    assert(DCX);
    spi_recive_long(l);
}

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

void lcd_cmd_read_display_status() {
    lcd_send_command(READ_DISPLAY_STATUS);

    volatile ILI9341_status status;
    status.raw_data = 0;

    spi_wait_not_busy();
    DCX_high();

    uint8_t byte;
    lcd_recive_byte(&byte);
    status.raw_data |= (uint32_t)byte << 25; 

    uint32_t l;
    spi_recive_long(&l);
    status.raw_data |= l >> 7;

    // discard the extra 7 bits sent
    CSX_high();
    volatile int i = 100;
    while(i--);
    CSX_low();
}

void lcd_cmd_pixel_format_set() {
    lcd_send_command(PIXEL_FORMAT_SET);
    spi_wait_not_busy();
    lcd_send_byte(0x55, true);
}

void lcd_cmd_partial_mode() {
    lcd_send_command(PARTIAL_AREA);
    spi_wait_not_busy();
    const static uint8_t data[] = {
        0x00, 0x00 + 32, // SR
        0x01, 0x3F - 32,  // ER
    };
    lcd_send_bytes(data, 4, true);
    spi_wait_not_busy();
    lcd_send_command(PARTIAL_MODE_ON);
}

void lcd_reset() {
    volatile unsigned int i = 500; // 20 us
    RESX_low();
    CSX_high();
    //wait at least 10 us
    while (i--);
    RESX_high();

    //wait at least 5000 us
    i = UINT16_MAX; // > 2500 us
    while (i--);

    CSX_low();
    lcd_send_command(SOFTWARE_RESET);
    //wait at least 5000 us
    i = UINT16_MAX; // 2500 us
    while (i--);
}

void lcd_init() {
    spi_init();

    {
        P2DIR |= BIT6; // CS
        P8DIR |= BIT1 | BIT2; // RESET DC
        P2OUT |= BIT6;
        P8OUT |= BIT1 | BIT2; // RESET DC
    }

    lcd_reset();
    lcd_cmd_read_display_status();
    lcd_send_command(SLEEP_OUT);
    volatile unsigned i = UINT16_MAX;
    while(i--);
    lcd_cmd_read_display_status();
    lcd_send_command(DISPLAY_ON);
    lcd_cmd_read_display_status();

    lcd_cmd_pixel_format_set();
    lcd_cmd_partial_mode();

    lcd_cmd_read_display_status();

    uint16_t colors[] = {
        0x001f,
        0x07E0,
        0xF100,
        0xffff,
    };
    uint16_t current_color;
    int a = 0;

    while (1) {
        CSX_high();
        i = 100;
        while (i--);
        CSX_low();
        current_color = colors[a & 3];
        spi_wait_not_busy();
        lcd_send_command(MEMORY_WRITE);
        spi_wait_not_busy();
        DCX_high();
        unsigned int j;
        for (j = 0; j < 61440; j++) {
            spi_send_byte(current_color >> 8);
            spi_send_byte(current_color);
        }
        a++;
    }
}
