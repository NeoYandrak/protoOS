/*********************************************************
 *** protoOS * vb1.0 ******************* by NeoYandrak ***
 *********************************************************
 *** Intento de usar el programa con Hardware y un LCD ***
 *** para ver si todo funciona y optimización del      ***
 *** código del SO.                                    ***
 ********************************************************/

// Include libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "nunchuck_funcs.h"

int loop_cnt=0; //Loop variable
byte accx,accy,zbut,cbut; //Joycon variables
int menuIndicator = 13; //ledPin variable
int homeIndicator = 12;
bool isMenuOpened; //Asks if the emotions menu is opened

int cursor = 1; //Cursor para expresiones
String current = "Idle"; //Expresión actual
String version = "vb1.0";

//Joystick panning (no values still)
const int joyx_min = 80;
const int joyx_max = 170;
const int joyy_min = 80;
const int joyy_max = 170;

bool debug = false;

LiquidCrystal_I2C lcd(0x27, 20, 4); //lcd

void setup()
{
  //lcd
    lcd.init();                  
    lcd.backlight();
    lcd.setCursor(0,1);
    lcd.print("   ** ch1mera OS    ");
    lcd.setCursor(0,2);
    lcd.print("  vb1.0 Loading.    ");

  // Iniciar chimeraOS
    Serial.begin(19200);
    Serial.print("Iniciando ch1mera OS");

    //Nunchuck
    delay(1000);
    Serial.print(".");
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("   ** ch1mera OS    ");
    lcd.setCursor(0,2);
    lcd.print("  vb1.0 Loading..   ");
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake

    //Outputs and Inputs
    delay(1000);
    Serial.print(".");
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("   ** ch1mera OS    ");
    lcd.setCursor(0,2);
    lcd.print("  vb1.0 Loading...  ");
    pinMode(menuIndicator, OUTPUT);
    pinMode(homeIndicator, OUTPUT);

    //Expresión
    current = "Idle"; //Idle

    //GUI
    isMenuOpened = false;
    debug = false;
    digitalWrite(menuIndicator, LOW);
    digitalWrite(homeIndicator, HIGH);
    Serial.println("- completado!");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("   ** ch1mera OS    ");
    lcd.setCursor(0,2);
    lcd.print("       vb1.0        ");
    delay(500);
    Serial.println("-------------------------");
    Serial.print  ("    ch1meraOS   - ");
    Serial.println(current);
    Serial.println("  - Eyes not found (404) ");
    lcd.clear();
    lcd.setCursor(1,0); lcd.print("** ");
    lcd.setCursor(0,1); lcd.print("Expresion: "); lcd.print(current);
    lcd.setCursor(0,2); lcd.print("404; Eyes not found");
    lcd.setCursor(15,3); lcd.print(version);
}

void loop() 
{
  if( loop_cnt > 150 ) { //150ms
    loop_cnt = 0;
    nunchuck_get_data();

    accx  = nunchuck_joyx();
    accy  = nunchuck_joyy();
    zbut = nunchuck_zbutton();
    cbut = nunchuck_cbutton(); 
                                                                                                  
    /*** Print Joystick variables ***/
    if (debug == true){ 
      digitalWrite(menuIndicator, HIGH);
      digitalWrite(homeIndicator, HIGH);
  
      Serial.print("accx: ");
      Serial.print((byte)accx,DEC);
      Serial.print("\taccy: ");
      Serial.print((byte)accy,DEC);
      Serial.print("\tzbut: ");
      Serial.print((byte)zbut,DEC);
      Serial.print("\tcbut: ");
      Serial.println((byte)cbut,DEC);
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Credits");
      lcd.setCursor(1,2);
      lcd.print("** ch1mera OS "); lcd.print (version);
      lcd.setCursor(3,3);
      lcd.print("by NeoYandrak");
      lcd.setCursor(0,1);
      lcd.print("====================");

    }

    if (cbut == 1) {
    
      if (isMenuOpened == false) {
        //Serial.println ("Menu opened");
        digitalWrite(menuIndicator, HIGH);
        digitalWrite(homeIndicator, LOW);
        isMenuOpened = true;

        menu(cursor);

     } else if (isMenuOpened == true) {
       //Serial.println ("Menu closed");
       digitalWrite(menuIndicator, LOW);
       digitalWrite(homeIndicator, HIGH);
       isMenuOpened = false;
       
       Serial.println("-------------------------");
       Serial.print  ("    ch1meraOS   - ");
       Serial.println(current);
       Serial.println("  - Eyes not found (404) ");
       lcd.clear();
    lcd.setCursor(1,0); lcd.print("** ");
    lcd.setCursor(0,1); lcd.print("Expresion: "); lcd.print(current);
    lcd.setCursor(0,2); lcd.print("404; Eyes not found");
    lcd.setCursor(15,3); lcd.print(version);
      }
    }

    if (isMenuOpened == true){
      //**Cursor initilization**
        //joy x
        if (accx <= joyx_min) {
        //Serial.println("Caso 1");
        cursor = cursor - 2;
        if (cursor <= 0) {
        cursor = 1;
        }
        menu(cursor);
        } else if (accx >= joyx_max) {
        //Serial.println("Caso 2");
        cursor = cursor + 2;
        if (cursor >= 5) {
        cursor = 4;
        }
        menu(cursor);
        }
        //joy y
        if (accy <= joyy_min) {
        //Serial.println("Caso 3");
        cursor = cursor + 1;
        if (cursor <= 0) {
        cursor = 1;
        }
        menu(cursor);
        } else if (accy >= joyy_max) {
        //Serial.println("Caso 4");
        cursor = cursor - 1;
        if (cursor >= 5) {
        cursor = 4;
        }
        menu(cursor);
        }
      if (zbut == 1){
        select(cursor);
      }
    }
    
    //Debug mode
    if (zbut == 1 ){
    if (cbut == 1){
    if (debug == false){
    debug = true;
    Serial.println("Debug mode activated");
    } else if (debug == true){
    debug = false;
    Serial.println("Debug mode desactivated");
    }
    }
    }
  }

  loop_cnt++;
  delay(1);
}

void menu (int c){
  switch (c){
    case 1:
    Serial.println("-------------------------");
    Serial.println("  > Idle        - Sad    ");
    Serial.println("  - Happy       - Angry  ");
    lcd.clear();
    lcd.setCursor(1,0); lcd.print("Menu");
    lcd.setCursor(0,1); lcd.print(" > Idle    - Sad    ");
    lcd.setCursor(0,2); lcd.print(" - Happy   - Angry  ");
    lcd.setCursor(15,3); lcd.print(version);
    break;
    case 2:
    Serial.println("-------------------------");
    Serial.println("  - Idle        - Sad    ");
    Serial.println("  > Happy       - Angry  ");
    lcd.clear();
    lcd.setCursor(1,0); lcd.print("Menu");
    lcd.setCursor(0,1); lcd.print(" - Idle    - Sad    ");
    lcd.setCursor(0,2); lcd.print(" > Happy   - Angry  ");
    lcd.setCursor(15,3); lcd.print(version);
    break;
    case 3:
    Serial.println("-------------------------");
    Serial.println("  - Idle        > Sad    ");
    Serial.println("  - Happy       - Angry  ");
    lcd.clear();
    lcd.setCursor(1,0); lcd.print("Menu");
    lcd.setCursor(0,1); lcd.print(" - Idle    > Sad    ");
    lcd.setCursor(0,2); lcd.print(" - Happy   - Angry  ");
    lcd.setCursor(15,3); lcd.print(version);
    break;
    case 4:
    Serial.println("-------------------------");
    Serial.println("  - Idle        - Sad    ");
    Serial.println("  - Happy       > Angry  ");
    lcd.clear();
    lcd.setCursor(1,0); lcd.print("Menu");
    lcd.setCursor(0,1); lcd.print(" - Idle    - Sad    ");
    lcd.setCursor(0,2); lcd.print(" - Happy   > Angry  ");
    lcd.setCursor(15,3); lcd.print(version);
    break;
  }
}

void select (int c){
switch (c){
case 1:
current = "Idle";
      Serial.println("-------------------------");
      Serial.print("         - ");
      Serial.println(current);
      Serial.println("      Seleccionado.      ");
break;
case 2:
current = "Happy";
      Serial.println("-------------------------");
      Serial.print("         - ");
      Serial.println(current);
      Serial.println("      Seleccionado.      ");
break;
case 3:
current = "Sad";
      Serial.println("-------------------------");
      Serial.print("          - ");
      Serial.println(current);
      Serial.println("      Seleccionado.      ");
break;
case 4:
      current = "Angry";
      Serial.println("-------------------------");
      Serial.print("         - ");
      Serial.println(current);
      Serial.println("      Seleccionado.      ");
break;
}
  
lcd.clear();
lcd.setCursor(1,0); lcd.print("** ");
lcd.setCursor(0,1); lcd.print("      > "); lcd.print(current);
lcd.setCursor(0,2); lcd.print("      selected ");
lcd.setCursor(15,3); lcd.print(version);

delay(1000);
isMenuOpened = false;   
  digitalWrite(menuIndicator, LOW);
  digitalWrite(homeIndicator, HIGH);
  Serial.println("-------------------------");
  Serial.print  ("    ch1meraOS   - ");
  Serial.println(current);
  Serial.println("  - Eyes not found (404) ");
  lcd.clear();
  lcd.setCursor(1,0); lcd.print("** ");
  lcd.setCursor(0,1); lcd.print("Expresion: "); lcd.print(current);
  lcd.setCursor(0,2); lcd.print("404; Eyes not found");
  lcd.setCursor(15,3); lcd.print(version);
}