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
int ledPin = 13; //ledPin variable

bool isMenuOpened; //Asks if the emotions menu is opened

void setup()
{
    Serial.begin(19200);
    nunchuck_setpowerpins();
    nunchuck_init(); // send the initilization handshake

    isMenuOpened = false;
    Serial.Println("ch1mera OS loaded correctly");
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
            
        /*** Print Joystick variables ***
        Serial.print("accx: ");
        Serial.print((byte)accx,DEC);
        Serial.print("\taccy: ");
        Serial.print((byte)accy,DEC);
        Serial.print("\tzbut: ");
        Serial.print((byte)zbut,DEC);
        Serial.print("\tcbut: ");
        Serial.println((byte)cbut,DEC);*/
      if (cbut == 1) {
        if (isMenuOpened == false) {
          Serial.println ("Menu opened");
          isMenuOpened = true;
        } else if (isMenuOpened == true) {
          Serial.println ("Menu closed");
          isMenuOpened = false;
        }
      }
    }
    
    loop_cnt++;
    delay(1);
}
