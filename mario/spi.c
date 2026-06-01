#include <driverlib.h>
#include "spi.h"

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

void spi_send_bytes(const uint8_t* bytes, uint16_t count) {
    while (count--) {
        spi_send_byte(*bytes++);
    }
}

static const uint8_t dummy_byte = 0;

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
