#include <Arduino.h>
#include <FreeRTOS.h>

// Declaración de la función de la segunda tarea
void tareaSecundaria(void *parameter);

void setup() {
  Serial.begin(115200);  // Inicializa el puerto serie

  // Crear una nueva tarea que se ejecuta en paralelo con loop()
  xTaskCreate(
    tareaSecundaria,     // Función que ejecutará la tarea
    "Tarea Secundaria",  // Nombre para depuración
    1000,                // Tamaño de la pila (bytes)
    NULL,                // Parámetro que se le pasa a la tarea (no se usa aquí)
    1,                   // Prioridad de la tarea
    NULL                 // Handle de la tarea (no se necesita aquí)
  );
}

void loop() {
  // Esta es la tarea principal que ejecuta Arduino (loopTask en FreeRTOS)
  Serial.println("Esta es una tarea del ESP32-S3");
  delay(1000); // Pausa 1 segundo (internamente permite multitarea con vTaskDelay)
}

// Esta es la segunda tarea, creada manualmente
void tareaSecundaria(void *parameter) {
  (void) parameter; // Evita advertencia si no se usa el parámetro

  for (;;) {
    Serial.println("Esta es otra tarea");
    delay(1000); // Pausa 1 segundo
    // Nota: delay() está permitido en ESP32 porque internamente usa vTaskDelay
  }

  vTaskDelete(NULL); // Por buenas prácticas, aunque no se ejecuta por el bucle infinito
}
