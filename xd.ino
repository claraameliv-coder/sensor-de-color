#include <Servo.h>

#include <SparkFun_APDS9960.h>

#include <Wire.h>


#define salidaTCS 8 
#define alarma 12  
#define ledRojo 11   
#define ledVerde 10  

Servo miservo;
SparkFun_APDS9960 apds;

uint16_t r, g, b, c;


void setup() {
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(alarma, OUTPUT);

  miservo.attach(9);
  miservo.write(90);  // posición neutra (puerta cerrada al inicio)

  Serial.begin(9600);
  Serial.println("Iniciando APDS-9960...");

  if (apds.init()) {
    Serial.println("Sensor inicializado correctamente!");
  } else {
    Serial.println("Error al iniciar el sensor");
    while (1);
  }

  if (apds.enableLightSensor(false)) {
    Serial.println("Sensor de color habilitado!");
  } else {
    Serial.println("Error al habilitar sensor de color");
    while (1);
  }

  delay(500);
}

void loop() {
  if (!apds.readRedLight(r) || !apds.readGreenLight(g) || !apds.readBlueLight(b) || !apds.readAmbientLight(c)) {
    Serial.println("Error leyendo valores de color");
  } else {
    Serial.print("Rojo: "); Serial.print(r);
    Serial.print(" | Verde: "); Serial.print(g);
    Serial.print(" | Azul: "); Serial.print(b);
    Serial.print(" | Luz ambiente: "); Serial.println(c);

    // --- ZOMBIE detectado (verde dominante) ---
    if (g > 25 && g < 90 && g >= (r * 0.8) && g >= (b * 0.8)) {
      Serial.println(">>> ZOMBIE DETECTADO <<<");
      digitalWrite(alarma, HIGH);    // alarma ON
      digitalWrite(ledRojo, HIGH);   // LED rojo ON
      digitalWrite(ledVerde, LOW);   // LED verde OFF


  
    }
    // --- HUMANO detectado (rojo predominante, tono piel) ---
    else if (r > g * 1.1 && r > b * 1.1 && r > 80){
      Serial.println(">>> HUMANO DETECTADO <<<");
      digitalWrite(alarma, LOW);     // alarma OFF
      digitalWrite(ledRojo, LOW);    // LED rojo OFF
      digitalWrite(ledVerde, HIGH);  // LED verde ON
      miservo.write(180);            // abre la puerta
      delay(4000);
      miservo.write(90);
    }
    // --- Caso neutro ---
    else {
      Serial.println("Nada claro detectado...");
      digitalWrite(alarma, LOW);
      digitalWrite(ledRojo, LOW);
      digitalWrite(ledVerde, LOW);
    }
  }

  delay(1000);
}
