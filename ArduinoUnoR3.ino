/*

1 linea: scrolling
2 linea: statica
 
 riferimento: http://arduino.cc/en/Tutorial/LiquidCrystal
 
 circuito:
 * LCD RS pin al digital pin 2
 * LCD Enable pin al digital pin 3
 * LCD D4 pin al digital pin 4
 * LCD D5 pin al digital pin 5
 * LCD D6 pin al digital pin 6
 * LCD D7 pin al digital pin 7
 * LCD R/W pin a ground
 * 2x resistenza da 390ohm in serie:
 * finisce a +5V e ground
 * wiper  LCD VO pin (pin 3)
*/

// librerie
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Usa l'indirizzo default I2C
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// A seconda del modello del servo, è necessario sintonizzare questi valori al meglio possibile senza forzare il perno

#define start1  130 // Avvio movimento del servo 1
#define stop1  300 // Stop movimento del servo 1
#define start2  110 // Avvio movimento del servo 2
#define stop2  220 // Stop movimento del servo 2

uint8_t servonum1 = 0; // Servo 1 assegnato alla port 0 di Adafruit servo driver
uint8_t servonum2 = 1; // Servo 1 assegnato alla port 1 di Adafruit servo driver

// Inizializzazione pin del display a seconda del circuito
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Impostazione LCD 16x2
int screenWidth = 16;
int screenHeight = 2;

// Prima linea a scorrimento, seconda statica
// line1 = Scorrimento
String line1 = "ISTITUTO TECNICO INDUSTRIALE STATALE - G. OMAR – NOVARA 2015/16       Non esistono surrogati del duro lavoro - Thomas Edison";
// line2 = Statica
String line2 = " FERRI JOSEPH ";

// Flag di riferimento
int stringStart, stringStop = 0;
int scrollCursor = screenWidth;

// Assegnazione pin ai vari Led
void setup() {
  lcd.begin(screenWidth,screenHeight);
  Serial.begin(9600);
  pinMode (8, OUTPUT); //LED 1 al pin 8
  pinMode (13, OUTPUT); //LED 2 al pin 13
  pinMode (12, OUTPUT); //LED 3 al pin 12
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  yield();
}

void loop() {
  lcd.setCursor(scrollCursor, 0);
  lcd.print(line1.substring(stringStart,stringStop));
  lcd.setCursor(0, 1);
  lcd.print(line2);
  delay(300);
  lcd.clear();
  if(stringStart == 0 && scrollCursor > 0){
    scrollCursor--;
    stringStop++;
  } else if (stringStart == stringStop){
    stringStart = stringStop = 0;
    scrollCursor = screenWidth;
  } else if (stringStop == line1.length() && scrollCursor == 0) {
    stringStart++;
  } else {
    stringStart++;
    stringStop++;
  }
  char c = Serial.read();
  
  if (c == 'Y' || c == 'y') //LED 1 on
  {
    digitalWrite (8, HIGH);
  }
  if (c == 'U' || c == 'u') //LED 1 off
  {
    digitalWrite (8, LOW);
  }
  
  if (c == 'G' || c == 'g') //LED 2 on
  {
    digitalWrite (13, HIGH);
  }
  if (c == 'H' || c == 'h') //LED 2 off
  {
    digitalWrite (13, LOW);
  }
if (c == 'B' || c == 'b') //LED 3 on
  {
    digitalWrite (12, HIGH);
  }
  if (c == 'N' || c == 'n') //LED 3 off
  {
    digitalWrite (12, LOW);
  }
  
  if (c == 'O' || c == 'o') //Servo 1, 0 a 90
  {
    for (uint16_t pulselen = start1; pulselen < stop1; pulselen++) 
    {
      pwm.setPWM(servonum1, 0, pulselen);
      delay(10);
    }
  }
  if (c == 'P' || c == 'p') //Servo 1, 90 a 0?
  {
    for (uint16_t pulselen = stop1; pulselen > start1; pulselen--) 
    {
      pwm.setPWM(servonum1, 0, pulselen);
      delay(10);
    }
  }

  if (c == 'J' || c == 'j') //Servo 2, 0 a 90?
  {
    for (uint16_t pulselen = start2; pulselen < stop2; pulselen++) 
    {
      pwm.setPWM(servonum2, 0, pulselen);
      delay(10);
    }
  }
  if (c == 'K' || c == 'k') //Servo 2, 90 a 0?
  {
    for (uint16_t pulselen = stop2; pulselen > start2; pulselen--) 
    {
      pwm.setPWM(servonum2, 0, pulselen);
      delay(10);
    }
  }
}
