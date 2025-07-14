#include "hal.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "simpleserial.h"

//This is the flag, but the value was redacted
static const uint8_t password[] = "REDACTED"; 

static const uint8_t password_len = sizeof(password) - 1;

uint8_t verify(uint8_t* data, uint8_t dlen) {
    uint8_t correct = 1;
    uint8_t matched_chars = 0;

    trigger_high();

    for(uint8_t i = 0; i < password_len; i++) {
        if(data[i] == password[i]) {
            matched_chars++;
            for(volatile uint32_t i = 0; i < 5000; i++);
        } else {
            break;
        }
    }

    trigger_low();

    if(matched_chars != password_len) {
        correct = 0;
    }
    simpleserial_put('r', 1, &correct);
    return 0;
}

int main(void) {
    platform_init();
    init_uart();
    trigger_setup();
    simpleserial_init();

    simpleserial_addcmd('a', password_len, verify);

    while(1) {
        simpleserial_get();
    }

    return 0;
}
