
/*=================================================================*/
/*Dichiaro le variabili*/
/*=================================================================*/
#include <LiquidCrystal_I2C.h>         //Libreria per pilotare il display LCD con solo 2 fili 
#define pin_temp A2                   //Pin di collegamento del piedino Vout del sensore di temperatura
float temp = 0;                      //Variabile in cui verrà memorizzata la temperatura rilevata
int tip120PIN = 10; 
LiquidCrystal_I2C lcd(0x3F,16,2);   //Inizializzazione della libreria con i pin I2C per il displayLCD
int pinSensorPIR = 5;               //Variabile sensore PIR
int pinLed = 9;                     //Variabile led garage

int valorSensorPIR = 0;              //Imposto un valore basso al sensore PIR
/*=================================================================*/
/*Setup*/
/*=================================================================*/
void setup() {
  lcd.init();    //Impostazione del display LCD
  lcd.backlight();
  lcd.setCursor(0, 0);     //Sposto il cursore sulla prima riga (riga 0) e sulla prima colonna
  lcd.print("Temperatura:");    //Stampo il messaggio 'Temperatura:' sulla prima riga
//Imposto Vref dell'ADC a 1,1V

analogReference(INTERNAL);
  Serial.begin(9600); //Inizializzo il seriale
 

/*=================================================================*/
  /*Definisco i pin come input e output*/
/*=================================================================*/
  pinMode(tip120PIN, OUTPUT);  // analogWrite(tip120PIN,200);
  pinMode(pinSensorPIR,INPUT);
  pinMode(pinLed,OUTPUT);
  pinMode(pin_temp,INPUT);
  delay(6000);
}
/*=================================================================*/
/*Il loop: dove il codice viene eseguito*/
/*=================================================================*/ 
void loop() {  
//Calcolo temperatura
temp = 0;
for (int i = 0; i < 5; i++) {                  //Esegue l'istruzione successiva 5 volte
temp += (analogRead(pin_temp) / 9.31);         //Calcola la temperatura e la somma alla variabile 'temp'
}
temp /= 5;                                    //Calcola la media matematica dei valori di temperatura

/*=================================================================
Imposto una condizione sul transistor TP120: quando la temperatura è inferiore a 30°, la tensione e 0
quando la temperatura è => 30°, viene regolata al 50% dell'alimentazione impiegata (in questo caso 4,5V), 
quando la temperatura supera i 35°, viene regolata una tensione massima
=================================================================*/

if (temp <30) {
 analogWrite(tip120PIN,0);
 lcd.setCursor(1, 1);    
 lcd.print("Duty cycle  0% ");
}  else if ((temp >=29) && (temp <35))
{  analogWrite(tip120PIN,123);
  lcd.setCursor(1, 1);    
 lcd.print("Duty cycle 50% ");
} else {
  analogWrite(tip120PIN,250);
  lcd.setCursor(1, 1);    
 lcd.print("Duty cycle 100%");
}
  
/*Visualizzo la temperatura sul display LCD */
lcd.setCursor(12, 0); //Sposto il cursore sulla prima colonna e sulla seconda riga
lcd.print(temp); //Stampo sul display LCD la temperatura
lcd.print(" C"); //Stampo uno spazio e il carattere 'C' sul display

delay(1000); //Ritardo di un secondo (può essere modificato)
  //Leggo il valore del sensore PIR, che può assumere 1 o 2 valori
  //1 quando rileva movimento e 0 quando non c'è movimento.
  valorSensorPIR = digitalRead(pinSensorPIR);
   
  Serial.print("Valore del sensore PIR: ");  
  Serial.println(valorSensorPIR);
   
  //Verificando l'occorrenza di movimento
  if (valorSensorPIR == 1) {
    accendi();
  } else {
    spegni();
  }    
}
 /*Imposto la funzione accendi e spegni allarme (che poi si occuperebbe soltanto del led)*/
 
void accendi() {
  digitalWrite(pinLed, HIGH);  //accendo il led
   
   
  delay(6000); //Tempo in cui il led rimane acceso
   
  spegni();
  delay(2000);
}
 
void spegni() {
  digitalWrite(pinLed, LOW); //Spengo il led
   
}
