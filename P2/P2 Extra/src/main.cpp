// Práctica 2 - Ejercicio extra
// Interrupción por timer
#include <Arduino.h>

// Pines
const int LED_PIN = 2; // Led
const int BUTTON_UP = 18; // Pulsador 1 (Augmenta velocidad)
const int BUTTON_DOWN = 19; // Pulsador 2 (Disminuye velocidad)

// Contadores de las veces que se pulsa cada botón
volatile uint32_t buttonUpPresses = 0;
volatile uint32_t buttonDownPresses = 0;


// Estado y tiempo de parpadeo del LED
volatile bool ledState = false;
volatile uint32_t blinkInterval = 500000; // En microsegundos (500 ms)

// Anti-rebote
volatile uint32_t lastDebounceTimeUp = 0;
volatile uint32_t lastDebounceTimeDown = 0;
const uint32_t debounceDelay = 200000; // 200 ms en microsegundos

// Timer por hardware
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// Interrupción del temporizador
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);

  // Alternar LED
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);

 // Leer botón de aumentar frecuencia
  uint32_t now = micros();

  // Pulsador para subir frecuencia
  if (digitalRead(BUTTON_UP) == LOW && (now - lastDebounceTimeUp) > debounceDelay) {
    if (blinkInterval > 100000) { // Mínimo 100ms
      blinkInterval -= 10000;   // Aumenta frecuencia
    }
    buttonUpPresses ++; // Suma pulsación
    lastDebounceTimeUp = now;
  }

  // Pulsador para bajar frecuencia
  if (digitalRead(BUTTON_DOWN) == LOW && (now - lastDebounceTimeDown) > debounceDelay) {
    if (blinkInterval < 2000000) { // Máximo 2s
      blinkInterval += 10000;     // Disminuye frecuencia
    }
    buttonDownPresses ++; // Suma pulsación
    lastDebounceTimeDown = now;
  }

  // Actualizar el periodo del temporizador
  timerAlarmWrite(timer, blinkInterval, true);

  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  Serial.begin(115200);

// Configuración de los pines
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_UP, INPUT_PULLUP); // Resistencia pull-up interna. 
  pinMode(BUTTON_DOWN, INPUT_PULLUP);

  // Configuración del temporizador
  timer = timerBegin(0, 80, true); // Temporizador por hardware en la placa.
  timerAttachInterrupt(timer, &onTimer, true); // Vincula la ISR al temporizador.
  timerAlarmWrite(timer, blinkInterval, true); // Configura la alarma para que se dispare.
  timerAlarmEnable(timer); // La activa
}

void loop() {
  static uint32_t lastPrint = 0;

  // Imprime información una vez por segundo
  if (millis() - lastPrint > 1000) {
    lastPrint = millis();

    float frequency = 1000000.0 / blinkInterval; // Calcula la frecuencia de parpadeo del LED

    Serial.printf("Frecuencia actual del LED: %.2f Hz\n", frequency);
    Serial.printf("Botón Aumentar presionado: %u veces\n", buttonUpPresses);
    Serial.printf("Botón Disminuir presionado: %u veces\n\n", buttonDownPresses);
  }
}
