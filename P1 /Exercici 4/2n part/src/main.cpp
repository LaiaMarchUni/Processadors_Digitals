#include <Arduino.h>
#include "soc/gpio_reg.h"

#define LED_BUILTIN 4

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

void loop() {

    uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG;
    *gpio_out |= (1 << LED_BUILTIN);  // Enciende el LED
    Serial.println("ON");

    *gpio_out ^= (1 << LED_BUILTIN);  // Apaga el LED
    Serial.println("OFF");
}
