
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int PIN_TRIG = 9;     
const int PIN_ECHO = 10;    
const int PIN_BUZZER = 8;   

int distanciaActual = 0;    

void setup() {
  pinMode(PIN_TRIG, OUTPUT);    
  pinMode(PIN_ECHO, INPUT);     
  pinMode(PIN_BUZZER, OUTPUT);  

  Serial.begin(9600);           

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Fallo al iniciar OLED");
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(15, 25);
  display.println("SENSOR DE DIST.");
  display.display();
  delay(1500);
}

void loop() {
  distanciaActual = medirDistancia(PIN_TRIG, PIN_ECHO);

  Serial.print("Distancia al obstaculo: ");
  Serial.print(distanciaActual);
  Serial.println(" cm");

  display.clearDisplay();
  
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("DISTANCIA:");

  display.setTextSize(3);
  display.setCursor(10, 18);
  display.print(distanciaActual);
  display.setTextSize(2);
  display.println(" cm");

  display.setTextSize(1);
  display.setCursor(0, 52);
  if (distanciaActual > 0 && distanciaActual <= 5) {
    display.println("ALERTA CRITICA!");
  } else if (distanciaActual > 5 && distanciaActual <= 15) {
    display.println("CUIDADO: MUY CERCA");
  } else if (distanciaActual > 15 && distanciaActual <= 30) {
    display.println("PRECAUCION");
  } else {
    display.println("CAMINO LIBRE");
  }

  display.display();

  gestionarAlerta(distanciaActual, PIN_BUZZER);
}

int medirDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);     
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duracion = pulseIn(echo, HIGH); 
  int cm = duracion * 0.034 / 2;       
  return cm;
}

void gestionarAlerta(int cm, int buzzer) {
  if (cm > 0 && cm <= 5) {
    digitalWrite(buzzer, HIGH);
  } 
  else if (cm > 5 && cm <= 15) {
    digitalWrite(buzzer, HIGH);
    delay(80);
    digitalWrite(buzzer, LOW);
    delay(80);
  } 
  else if (cm > 15 && cm <= 30) {
    digitalWrite(buzzer, HIGH);
    delay(250);
    digitalWrite(buzzer, LOW);
    delay(250);
  } 
  else {
    digitalWrite(buzzer, LOW);
    delay(100);
  }
}
