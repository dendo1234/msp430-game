#pragma once
void adc_init();

void adc_start_conversion();

typedef struct {
    int8_t x;
    int8_t y;
} ADC_result;

ADC_result adc_read();
