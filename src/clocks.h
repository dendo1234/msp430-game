#pragma  once
#include <driverlib.h>
#include <stdint.h>
#include <assert.h>

static const uint32_t SMCLK_FREQUENCY = 1048576l * 8;
static const uint16_t ACLK_FREQUENCY = 32768;

static const uint32_t DCOCLKDIV_TARGET = 1l << 24; // 16 MHz
static const uint16_t REFOCLK = 1l << 15;
static const uint16_t FLLNREFCLK = REFOCLK;
static const uint16_t FLLN_VALUE = (DCOCLKDIV_TARGET/FLLNREFCLK) - 1;
static_assert(FLLN_VALUE < 512, "FLLN can only be up to 511. Consider rethinking your decisions");

static const uint16_t FLLD_VALUE = FLLD__2;
static const uint32_t DCOCLK_TARGET = DCOCLKDIV_TARGET * FLLD_VALUE;

inline void overclock() {
    // stop fll
    __bis_SR_register(SCG0);

    UCSCTL0_H = 0;

    UCSCTL2 = FLLD_VALUE + FLLN_VALUE;
    UCSCTL1 = DCORSEL_6;

    UCSCTL5 &= ~0x70;
    UCSCTL5 |= DIVS__2;
    __bic_SR_register(SCG0);

    while (UCSCTL7 & DCOFFG)
    {
        //Clear OSC flaut Flags
        UCSCTL7 &= ~(DCOFFG);

        //Clear OFIFG fault flag
        SFRIFG1 &= ~OFIFG;
    }
}
