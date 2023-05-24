#include "analog.h"
/*#include <avr/io.h>

uint16_t analogRead(uint8_t pin) {
    //adc_configure();
    adc_channel(pin);

    ADCSRA |= (1 << ADSC);

    while(ADCSRA & (1 << ADSC));

    uint16_t val = ADC;
    return val;
}

void adc_configure() {
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    ADCSRA |= (1 << ADEN);
}

void adc_channel(uint8_t pin) {
    ADMUX &= ~(0x0F);
    ADMUX |= (pin & 0x0F);
}
*/
