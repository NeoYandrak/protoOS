/*********************************************************
 *** protoOS * vb0.2 ******************* by NeoYandrak ***
 *********************************************************
 *** Esta versión empiea a mirar el cambio de menús    ***
 *** de expresiones del protogen                       ***
 ********************************************************/

// Include libraries
#include <Wire.h>
#include "nunchuck_funcs.h"

int loop_cnt=0; //Loop variable
byte accx,accy,zbut,cbut; //Joycon variables
int menuIndicator = 13; //ledPin variable
int homeIndicator = 12;
bool isMenuOpened; //Asks if the emotions menu is opened

int cursor = 1; //Cursor para expresiones

//Joystick panning (no values still)
const int joyx_min = 100;
const int joyx_max = 150;
const int joyy_min = 110;
const int joyy_max = 150;

bool debug = false;

void setup()
{
  // Iniciar chimeraOS
    Serial.begin(19200);
    Serial.print("Iniciando ch1mera OS... ");

    //Nunchuck
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake

    //Outputs and Inputs
    pinMode(menuIndicator, OUTPUT);
    pinMode(homeIndicator, OUTPUT);

    //GUI
    isMenuOpened = false;
    debug = false;
    digitalWrite(menuIndicator, LOW);
    digitalWrite(homeIndicator, HIGH);
    Serial.println("- completado!");
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
    }

    if (cbut == 1) {
    
      if (isMenuOpened == false) {
        Serial.println ("Menu opened");
        digitalWrite(menuIndicator, HIGH);
        digitalWrite(homeIndicator, LOW);
        isMenuOpened = true;

        menu(cursor);

     } else if (isMenuOpened == true) {
       Serial.println ("Menu closed");
       digitalWrite(menuIndicator, LOW);
       digitalWrite(homeIndicator, HIGH);
       isMenuOpened = false;
      }
    }

    if (isMenuOpened == true){
      //**Cursor initilization**
        //joy x
        if (nunchuck_joyx <= joyx_min) {
          Serial.println("Caso 1");
          cursor = cursor - 2;
          if (cursor <= 0) {
            cursor = 1;
          }

          menu(cursor);

        } else if (nunchuck_joyx >= joyx_max) {
          Serial.println("Caso 2");
          cursor = cursor + 2;
          if (cursor >= 5) {
            cursor = 5;
          }

          menu(cursor);

        }
        //joy y
        if (nunchuck_joyy <= joyy_min) {
          Serial.println("Caso 3");
          cursor = cursor - 1;
          if (cursor <= 0) {
            cursor = 1;
          }

          menu(cursor);

        } else if (nunchuck_joyy >= joyy_max) {
          Serial.println("Caso 4");
          cursor = cursor + 1;
          if (cursor >= 5) {
            cursor = 5;
          }

          menu(cursor);

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
      Serial.println("---");
      Serial.println("> Idle           - Sad");
      Serial.println("- Happy          - Angry");
      break;
    case 2:
      Serial.println("---");
      Serial.println("- Idle           > Sad");
      Serial.println("- Happy          - Angry");
      break;
    case 3:
      Serial.println("---"); 
      Serial.println("- Idle           - Sad");
      Serial.println("> Happy          - Angry");
      break;
    case 4:
      Serial.println("---");
      Serial.println("- Idle           - Sad");
      Serial.println("- Happy          > Angry");
      break;
  }
}