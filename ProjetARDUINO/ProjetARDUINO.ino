/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/

#include <SPI.h>
#include <WiFi.h>
// 
#include "config.h"
#include <Keypad.h> 
#include <LiquidCrystal.h>

LiquidCrystal lcd(11, 39, 38, 37, 35,36);
// set up the 'analog' feed
AdafruitIO_Feed *analog = io.feed("pin");



const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {16, 15, 14, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 9, 6, 5}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

int PIN; // code pin correcte sur ad  fruit 
int entered_pin = NULL ; // nombre entrée 


void setup(){
  lcd.setCursor(5, 1);
  Serial.begin(9600);
  io.connect();//connection adafruit 

  analog->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  analog->get();
  //  lcd 
  lcd.begin(16,2);

}

void loop(){
  io.run();
  char customKey = customKeypad.getKey(); 
  
  if (customKey){
    int customKeyInt = customKey - '0' + ( customKey >='A' ? 9 : 0 ); // convertie le char a int
    // verifie si le code est un int entre 0 9 ou un CHAR 
    if( customKeyInt < 0 || customKeyInt > 15 ){
      Serial.println("erreur, entrer un nombre");
      delay(500);
      lcd.println("erreur , entrer un nombre");
      return;
      lcd.clear(); // clean le LCD 
      }
    // ajoute le customKey aux code pin entrée 
    Serial.println("Entrez le pin");
    lcd.println("Entrez le code pin");
    lcd.clear();
     entered_pin = entered_pin*10 + customKeyInt;
    Serial.println(entered_pin);
    lcd.println(entered_pin);// montre le pin sur le lcd
        
    // verifie si le code pin est a 4 caracter
     if (entered_pin > 999) {
      // Regarde si le code est bon
      if (entered_pin == PIN) {
        Serial.println("Code Bon");
       
        lcd.println("Code Bon");
        delay(3000);
        lcd.clear();
        // si le code est bon ou else erreur
      } else {
        Serial.println("Code faux"); 
        delay(500);
        lcd.println("Code faux");
        delay(3000);
        lcd.clear();
        // erreur
       
      }
      entered_pin = 0; // reset 
     lcd.clear();
    }
    }
  
}
  
void handleMessage(AdafruitIO_Data *data) {

  // convert the data to integer
  PIN = data->toInt();

  Serial.print("received <- ");
  Serial.println(PIN);
}
