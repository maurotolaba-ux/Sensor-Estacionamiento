
 // Libreria para comunicacionn I2C.
#include <Wire.h>    
// Libreria para graficos y textos.
#include <Adafruit_GFX.h> 
// Libreria para controlar el display OLED.
#include <Adafruit_SSD1306.h>  

// Dimensiones de la pantalla OLED: 128 pixeles de ancho y 64 de alto.
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Se crea el objeto display para controlar la pantalla mediante I2C.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Definicion de los pines utilizados en Arduino.
const int PIN_TRIG = 9;    // Pin que envia la senal del sensor.
const int PIN_ECHO = 10;   // Pin que recibe el rebote del ultrasonido.
const int PIN_BUZZER = 8;  // Pin que controla el buzzer.

// Variable que almacena la distancia medida en centimetros.
int distanciaActual = 0;

// Configuracion inicial del sistema.
void setup() {

  // Configura los pines del sensor y del buzzer.
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Inicia la comunicacion serial a 9600 baudios.
  Serial.begin(9600);

  // Inicializa el display OLED con direccion I2C 0x3C.
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    // Informa un error si no se puede iniciar la pantalla.
    Serial.println("Fallo al iniciar OLED");

    // Detiene el programa si el display no funciona.
    for (;;);

  }

  // Limpia la pantalla y configura el color del texto.
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  // Muestra un mensaje inicial en el display.
  display.setCursor(15, 25);
  display.println("SENSOR DE DIST.");
  display.display();

  // Espera 1,5 segundos antes de comenzar las mediciones.
  delay(1500);

}

// Bucle principal: se ejecuta continuamente mientras el Arduino esta encendido.
void loop() {

  // Obtiene la distancia medida por el sensor ultrasonico.
  distanciaActual = medirDistancia(PIN_TRIG, PIN_ECHO);

  // Envia la distancia al monitor serial.
  Serial.print("Distancia al obstaculo: ");
  Serial.print(distanciaActual);
  Serial.println(" cm");

  // Limpia la pantalla para actualizar la informacion.
  display.clearDisplay();

  // Muestra el titulo de la medicion.
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("DISTANCIA:");

  // Muestra la distancia medida en un tamano de letra mayor.
  display.setTextSize(3);
  display.setCursor(10, 18);
  display.print(distanciaActual);

  // Agrega la unidad de medida en centimetros.
  display.setTextSize(2);
  display.println(" cm");

  // Ubica el mensaje de alerta en la parte inferior.
  display.setTextSize(1);
  display.setCursor(0, 52);

  // Evalua la distancia y determina el nivel de riesgo.
  if (distanciaActual > 0 && distanciaActual <= 5) {

    // Distancia critica: el obstaculo esta a 5 cm o menos.
    display.println("ALERTA CRITICA!");

  } else if (distanciaActual > 5 && distanciaActual <= 15) {

    // Nivel de cuidado: el obstaculo esta entre 6 y 15 cm.
    display.println("CUIDADO: MUY CERCA");

  } else if (distanciaActual > 15 && distanciaActual <= 30) {

    // Nivel de precaucion: el obstaculo esta entre 16 y 30 cm.
    display.println("PRECAUCION");

  } else {

    // Si no se cumple ninguna condicion anterior, indica camino libre.
    display.println("CAMINO LIBRE");

  }

  // Actualiza la informacion visible en la pantalla.
  display.display();

  // Activa el buzzer segun la distancia detectada.
  gestionarAlerta(distanciaActual, PIN_BUZZER);

}

// Funcion que mide la distancia utilizando el sensor HC-SR04.
// Recibe los pines TRIG y ECHO y devuelve la distancia en centimetros.
int medirDistancia(int trig, int echo) {

  // Asegura que el pin TRIG comience en estado bajo.
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  // Envia un pulso ultrasonico de 10 microsegundos.
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Mide cuanto tiempo permanece activa la senal recibida en ECHO.
  long duracion = pulseIn(echo, HIGH);

  // Convierte el tiempo del pulso en distancia en centimetros.
  // Se divide por 2 porque el sonido realiza ida y vuelta.
  int cm = duracion * 0.034 / 2;

  // Devuelve el resultado de la medicion.
  return cm;

}

// Funcion que controla las alertas sonoras segun la distancia.
// Recibe la distancia medida y el pin conectado al buzzer.
void gestionarAlerta(int cm, int buzzer) {

  // Si el obstaculo esta a 5 cm o menos, activa el buzzer continuamente.
  if (cm > 0 && cm <= 5) {

    digitalWrite(buzzer, HIGH);

  }

  // Entre 6 y 15 cm, genera una alerta sonora rapida.
  else if (cm > 5 && cm <= 15) {

    digitalWrite(buzzer, HIGH);
    delay(80);
    digitalWrite(buzzer, LOW);
    delay(80);

  }

  // Entre 16 y 30 cm, genera una alerta sonora mas lenta.
  else if (cm > 15 && cm <= 30) {

    digitalWrite(buzzer, HIGH);
    delay(250);
    digitalWrite(buzzer, LOW);
    delay(250);

  }

  // Si el obstaculo esta a mas de 30 cm o no hay una medicion valida,
  // apaga el buzzer.
  else {

    digitalWrite(buzzer, LOW);
    delay(100);

  }

}

