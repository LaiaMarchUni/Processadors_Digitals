// Práctica 2 - B
// Interrupción por timer

#include <Arduino.h> // Librería principal de Arduino para ESP32

// Contadores de interrupciones. 
// 'volatile' se usa porque la variable puede ser modificada en cualquier momento por la interrupción,
// y siempre debe leerse directamente desde memoria (evita optimizaciones del compilador).
volatile int interruptCounter; // Contador auxiliar
int totalInterruptCounter; // Contador total


hw_timer_t * timer = NULL;  // Puntero a un temporizador por hardware de la placa para generar interrupciones periódicas.
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; // Mutex para proteger el acceso compartido a la variable entre la interrupción y el loop.

// Función que se ejecuta desde el Hardware automáticamente cada vez que el temporizador genera una interrupción. 
// IRAM_ATTR indica que debe estar en la RAM interna.
void IRAM_ATTR onTimer() {
    portENTER_CRITICAL_ISR(&timerMux); // Asegura que interruptCounter no puede ser modificada simultáneamente desde otro lugar.
    interruptCounter++; 
    portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
    // Funciones de la librería Arduino:
    Serial.begin(115200); // Inicia la comunicación con el serial monitor para enviar los mensajes.

    timer = timerBegin(0, 80, true); // Inicialización del puntero al temporizador de la placa.
    // 0 -> Usamos el temporizador 0
    // 80 -> Prescaler. Divide el reloj base de 80MHz por 80. Nos queda 1 MHz, es decir, 1 tick = 1 microsegundo.
    // true -> Cuenta ascendiente.

 
    timerAttachInterrupt(timer, &onTimer, true); // Asocia la función onTimer a este temporizador.
    timerAlarmWrite(timer, 1000000, true); // Configura el temporizador para que dispare cada segundo.
    timerAlarmEnable(timer); // Habilita la alarma para que empiece a contar.

    // Cada vez que se cumpla el tiempo de interrupción, el ESP32
    // llamará a tu función onTimer(), sin importar lo que haya en el loop(). 
}

void loop() {
// Cada repetición del código principal, se verifica si ha habido alguna interrupción. 
// (Si la ISR ha incrementado el contador)
    if (interruptCounter > 0) { // Si ha habido una interrupción,

        portENTER_CRITICAL(&timerMux); // Se resta el contador auxiliar para que vuelva a estar en 0
        interruptCounter--;
        portEXIT_CRITICAL(&timerMux); 

        totalInterruptCounter++; // Se suma el contador total

        Serial.print("An interrupt as occurred. Total number: "); //Mensaje por el serial monitor
        Serial.println(totalInterruptCounter);

    }
}