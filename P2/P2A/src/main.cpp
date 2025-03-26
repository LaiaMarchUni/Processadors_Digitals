// Práctica 2 - A
// Interrupción por GPIO (pulsador)

#include<Arduino.h> // Librería principal de Arduino para ESP32
 
// Estructura para el pulsador
struct Button { 
  const uint8_t PIN; // Pin al que se conecta
  uint32_t numberKeyPresses; // Núm de veces que se ha pulsado
  bool pressed; // Booleano para saber si se ha pulsado
};

// Se declara el botón en el pin GPIO 18
Button button1 = {18, 0, false};
  
// Función de interrupción (ISR) que se llama automáticamente cuando se aprieta el pulsador.
void IRAM_ATTR isr() { 
  button1.numberKeyPresses += 1; // Aumenta el contador
  button1.pressed = true; // Cambia el estado del booleano
} 
 

void setup() {
  Serial.begin(115200); // Inicia la comunicación serie
  delay(1000); // Espera 1 segundo
  
  pinMode(button1.PIN, INPUT_PULLUP); // Configura el pin del botón como entrada
  attachInterrupt(button1.PIN, isr, FALLING); // Asocia la interrupción al pin del botón y se activa en flanco descendiente
  
  Serial.println("ESP32-S3 iniciado corectamnte!"); // Mensaje al monitor serie
}

void loop() {
  // Si el booleano del pulsador está true (se ha apretado): 
  if (button1.pressed) {
        Serial.printf("Button 1 has been pressed %u times\n",button1.numberKeyPresses); // Se muestra el número de pulsaciones
        button1.pressed = false; // Se vuelve a dejar a false
  }

  // Desactiva la interrupción automáticamente después de 1 minuto
  static uint32_t lastMillis = 0;
  if (millis() - lastMillis > 60000) {
    lastMillis = millis();
    detachInterrupt(button1.PIN);
     Serial.println("Interrupt Detached!");
  }
}