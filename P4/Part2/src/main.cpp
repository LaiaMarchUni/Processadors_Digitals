#include <Arduino.h>

// Pin al que está conectado el LED
const int led = 12;

// Declaración de tareas
void led_on(void *parameter);
void led_off(void *parameter);

// Declaración de semáforos
SemaphoreHandle_t encendido;
SemaphoreHandle_t apagado;

void setup() {
  pinMode(led, OUTPUT);              // Configura el pin del LED como salida
  Serial.begin(115200);              // Inicia la comunicación serie

  // Creación de semáforos binarios
  encendido = xSemaphoreCreateBinary();
  apagado = xSemaphoreCreateBinary();

  // Inicializamos ambos semáforos dándolos una vez.
  // Esto permite que ambas tareas se activen al menos una vez al inicio.
  xSemaphoreGive(apagado);
  xSemaphoreGive(encendido);

  // Creación de las tareas
  xTaskCreate(led_on,  "led_on",  1000, NULL, 1, NULL);
  xTaskCreate(led_off, "led_off", 1000, NULL, 1, NULL);
}

void loop() {
  // No se usa. Todo lo manejan las tareas FreeRTOS.
}

// Tarea que enciende el LED
void led_on(void *parameter) {
  (void)parameter; // Evita advertencias si no se usa el parámetro

  for (;;) {
    // Espera hasta que el semáforo "apagado" esté disponible
    if (xSemaphoreTake(apagado, portMAX_DELAY) == pdPASS) {
      digitalWrite(led, HIGH); // Enciende el LED
      Serial.println("LED ENCENDIDO");
      vTaskDelay(1000 / portTICK_PERIOD_MS); // Espera 1 segundo
      xSemaphoreGive(encendido); // Permite que la otra tarea lo apague
    }
  }

  // No se alcanza, pero se deja por buenas prácticas
  vTaskDelete(NULL);
}

// Tarea que apaga el LED
void led_off(void *parameter) {
  (void)parameter;

  for (;;) {
    // Espera hasta que el semáforo "encendido" esté disponible
    if (xSemaphoreTake(encendido, portMAX_DELAY) == pdPASS) {
      digitalWrite(led, LOW); // Apaga el LED
      Serial.println("LED APAGADO");
      vTaskDelay(1000 / portTICK_PERIOD_MS); // Espera 1 segundo
      xSemaphoreGive(apagado); // Permite que la otra tarea lo vuelva a encender
    }
  }

  vTaskDelete(NULL);
}
