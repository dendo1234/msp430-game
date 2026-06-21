#include <driverlib.h>
// #include "math.h"
#include "adc.h"

// static const double RS = 5000;
// static const double RI = 1800.0;
// static const double CI = (25.0 * 1E-12);
// static const double N_BITS = 8;

void adc_init() {
    // volatile double t_cap = (RS + RI)*CI*log(pow(2, N_BITS+1)) + 800E-9;

    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 = 0 
    | ADC12SHT0_2 // 16 clock cycles = 0.2*16 = 3.2 us
    | ADC12MSC
    | ADC12ON;

    ADC12CTL1 = ADC12CSTARTADD_0
    | ADC12SHS_0 //ADC12SC
    | ADC12SHP 
    | ADC12DIV_0
    | ADC12SSEL_0 // modclk
    | ADC12CONSEQ_1 //repeat
    ;

    ADC12CTL2 = ADC12TCOFF // temp sensor off
    // | ADC12DF
    | ADC12RES_0 // 8 bit
    ;

    uint16_t register_config = ADC12SREF_0 | ADC12INCH_0;
    uint16_t register_config2 = ADC12SREF_0 | ADC12INCH_1;
    ADC12MCTL0 = register_config;
    ADC12MCTL1 = register_config;
    ADC12MCTL2 = register_config;
    ADC12MCTL3 = register_config;
    ADC12MCTL4 = register_config2;
    ADC12MCTL5 = register_config2;
    ADC12MCTL6 = register_config2;
    ADC12MCTL7 = register_config2 | ADC12EOS;

    ADC12CTL0 |= ADC12ENC;

    P6SEL |= BIT0 | BIT1;
}

void adc_start_conversion() {
    ADC12CTL0 |= ADC12SC;
}

ADC_result adc_read() {
    while (ADC12CTL1 & ADC12BUSY);

    uint16_t* ptr = ((uint16_t*)ADC12MEM);
    uint16_t x = 0;
    uint16_t y = 0;
    for (int i = 0; i < 4; i++) {
        x += ptr[i];
        y += ptr[i+4];
    }

    x /= 4;
    y /= 4;

    if (x > 120 && x < 136) {
        x = 128;
    }

    if (y > 120 && y < 136) {
        y = 128;
    }

    ADC_result result;
    result.x = x - 128;
    result.y = y - 128;

    return result;
}
