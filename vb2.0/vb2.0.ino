/*********************************************************
 *** protoOS * vb2.0 ******************* by NeoYandrak ***
 *********************************************************
 *** LED matmatrix MAD2791 for phisical eyes and visor ***
 ********************************************************/

// Include libraries
#include <Wire.h>              //For I2C protocol
#include <LiquidCrystal_I2C.h> //For the LCD display
#include <MaxMatrix.h>         //For the LED matrix
#include "nunchuck_funcs.h"    //For the Nunchuck

//Nunchuck variables
byte joyx, joyy, enter, option;
const int joy_min = 80;
const int joy_max = 170;

//GUI
/*Bool*/ bool isMenuOpened;
bool isDebug = false;
/*Control*/ int cursor = 1;
String current = "Idle";
int currentN = 1;
/*LCD*/ LiquidCrystal_I2C lcd(0x27, 20, 4);

//Status Bar
/*lock*/ bool lock;

//lcd custom icons
byte ch1_L[] = {B00001, B00010, B00100, B00010, B00001, B00000, B00000, B00000};
byte ch1_R[] = {B00000, B00000, B10000, B01000, B00100, B01000, B10000, B00000};
byte lockS[] = {B01110, B10001, B10001, B11111, B11001, B10011, B11111, B00000};
byte openS[] = {B00110, B01001, B00001, B11111, B11001, B10011, B11111, B00000};

//Expresiones
unsigned char noseRight[] = {8, 8, B00000000, B00000000, B10000000, B11000000, B11000000, B11111100, B01111110, B00000000};
unsigned char icon01[] = {8, 8, B00100000, B01111000, B11011110, B11000111, B11111111, B00000000, B00000000, B00000000};
unsigned char icon02[] = {8, 8, B00000000, B00000000, B00000000, B10000000, B11100000, B01111000, B00011110, B00000111};
unsigned char icon03[] = {8, 8, B00000000, B00000000, B00000000, B00000000, B00000111, B00011110, B01111000, B11100000};
unsigned char icon04[] = {8, 8, B00000000, B00000000, B00000000, B11100000, B11111000, B00011110, B00000111, B00000001};
unsigned char Glitch01[] = {8, 8, B00001100, B00000000, B11010100, B10001001, B00010100, B01000111, B11010001, B10100101};
unsigned char Glitch011[] = {8, 8, B00000000, B00000000, B00000000, B11010101, B10000101, B11101010, B00010111, B00000100};
unsigned char Glitch02[] = {8, 8, B01110100, B00000000, B01001011, B10010110, B00010010, B00000000, B00000000, B00000000};
unsigned char Glitch022[] = {8, 8, B10100010, B01010100, B10000110, B00010010, B00000101, B00000000, B00000000, B00000000};
unsigned char Glitch03[] = {8, 8, B00101001, B10101001, B01001011, B00101100, B01110100, B00000000, B00000000, B00000000};
unsigned char Glitch033[] = {8, 8, B01001101, B10011010, B01101001, B10100000, B00001000, B00000000, B00000000, B00000000};
unsigned char Glitch04[] = {8, 8, B00000000, B00000000, B10010101, B10011010, B10010111, B11101010, B10111010, B11010000};
unsigned char Glitch044[] = {8, 8, B00000000, B00000000, B10001000, B10101010, B00011011, B10010101, B00000001, B00000000};
unsigned char Eye02[] = {8, 8, B00000000, B00000000, B00000110, B00001111, B11111111, B11111110, B11111100, B11110000};
unsigned char Eye01[] = {8, 8, B00000000, B00000000, B10000000, B11100000, B01111111, B00011111, B00000111, B00000000};
unsigned char Angry1[] = {8, 8, B00000000, B11000000, B11110000, B11111000, B11111100, B11111100, B11111000, B00000000};
unsigned char Angry2[] = {8, 8, B00000000, B00000001, B00000111, B00011111, B00111111, B01111111, B00111111, B00000000};
unsigned char Spooked1[] = {8, 8, B00000001, B00000111, B00000111, B00001111, B00001111, B00000111, B00000111, B00000001};
unsigned char Spooked2[] = {8, 8, B10000000, B11100000, B11100000, B11110000, B11110000, B11100000, B11100000, B10000000};
unsigned char noseLeft[] = {8, 8, B00000000, B00000000, B00000001, B00000011, B00000011, B00111111, B01111110, B00000000};
unsigned char icon01L[] = {8, 8, B00001000, B00011110, B01111011, B11100011, B11111111, B00000000, B00000000, B00000000};
unsigned char icon02L[] = {8, 8, B00000000, B00000000, B00000000, B00000001, B00000111, B00011110, B01111000, B11100000};
unsigned char icon03L[] = {8, 8, B00000000, B00000000, B00000000, B00000000, B11100000, B01111000, B00011110, B00000111};
unsigned char icon04L[] = {8, 8, B00000000, B00000000, B00000000, B00000111, B00011111, B01111000, B11100000, B10000000};
unsigned char Glitch01L[] = {8, 8, B10100101, B11010001, B01000111, B00001010, B10001001, B11010100, B00001100, B00000000};
unsigned char Glitch011L[] = {8, 8, B00000000, B00000000, B00000000, B10101011, B10100001, B01010111, B11101000, B00100000};
unsigned char Glitch02L[] = {8, 8, B00000000, B00000000, B00000000, B00000000, B00010010, B10010110, B01001011, B01110100};
unsigned char Glitch022L[] = {8, 8, B00000000, B00000000, B00000000, B00000101, B00010010, B10000110, B01010100, B10100010};
unsigned char Glitch03L[] = {8, 8, B00000000, B00000000, B00000000, B01110100, B00101100, B01001011, B10101001, B00101001};
unsigned char Glitch033L[] = {8, 8, B00000000, B00000000, B00000000, B00001000, B10100000, B01101001, B10011010, B01001101};
unsigned char Glitch04L[] = {8, 8, B00000000, B00000000, B10101001, B01011001, B11101001, B01010111, B01011101, B00001011};
unsigned char Glitch044L[] = {8, 8, B00000000, B00000000, B00010001, B00000110, B01010101, B11011000, B10101001, B10000000};
unsigned char Eye02L[] = {8, 8, B00000000, B00000000, B01100000, B11110000, B11111111, B01111111, B00111111, B00001111};
unsigned char Eye01L[] = {8, 8, B00000000, B00000000, B00000001, B00000111, B11111110, B11111000, B11100000, B00000000};
unsigned char Angry2L[] = {8, 8, B00000000, B00000011, B00001111, B00011111, B00111111, B00111111, B00011111, B00000000};
unsigned char Angry1L[] = {8, 8, B00000000, B10000000, B11100000, B11111000, B11111100, B11111110, B11111100, B00000000};
unsigned char Spooked1L[] = {8, 8, B00000001, B00000111, B00000111, B00001111, B00001111, B00000111, B00000111, B00000001};
unsigned char Spooked2L[] = {8, 8, B10000000, B11100000, B11100000, B11110000, B11110000, B11100000, B11100000, B10000000};

//MaxMatrix
int DIN = 6;
int CLK = 7;
int CS = 8;
int maxInUse = 14;
int column1L = 104;
int column2L = 112;
int column3L = 95;
int column4L = 103;
int column1 = 8;
int column2 = 16;
int column3 = -1;
int column4 = 7;
MaxMatrix m(DIN, CS, CLK, maxInUse);

//Others
int loop_cnt = 0;
int counter = 0;
int counter2 = 0;

String version = "vb1.4";

void setup()
{
  //LCD
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, ch1_L);
  lcd.createChar(1, ch1_R);
  lcd.createChar(2, lockS);
  lcd.createChar(3, openS);
  loading(1);

  //Nunchuk
  nunchuck_setpowerpins();
  nunchuck_init();
  loading(2);

  //Expresión
  current = "Idle";
  currentN = 1;
  randomSeed(analogRead(0));
  m.init();
  m.setIntensity(9); //0-15
  m.clear();
  loading(3);

  //GUI
  isMenuOpened = false;
  isDebug = false;
  lock = false;
  loading(4);
  home();
}

void loop()
{
  if (loop_cnt > 150)
  { //150ms
    loop_cnt = 0;

    //Obtener datos Nunchuck
    nunchuck_get_data();
    joyx = nunchuck_joyx();
    joyy = nunchuck_joyy();
    enter = nunchuck_zbutton();
    option = nunchuck_cbutton();

    //Enter Button
    /*if (enter == 1)
    {
      if (option == 1)
      {
        //check eyeControl
        if (lock == false)
        {
          lock = true;
          enter = 0;
          option = 0;
          lcd.clear();
          statusBar();
          lcd.setCursor(0, 1);
          lcd.print("Bloqueado");
          delay(1000);
          home();
        }
        else if (lock == true)
        {
          lock = false;
          enter = 0;
          option = 0;
          lcd.clear();
          statusBar();
          lcd.setCursor(0, 1);
          lcd.print("Desbloqueado");
          delay(1000);
          home();
        }
      }
    }*/

    //isDebug
    if (isDebug == true)
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Credits");
      lcd.setCursor(1, 2);
      lcd.print("** ch1mera OS ");
      lcd.print(version);
      lcd.setCursor(3, 3);
      lcd.print("by NeoYandrak");
      lcd.setCursor(0, 1);
      lcd.print("====================");
    }

    //isMenuOpened
    if (isMenuOpened == true)
    {
      //**Cursor initilization**
      //joy x
      if (joyx <= joy_min)
      {
        //Serial.println("Caso 1");
        cursor = cursor - 2;
        if (cursor <= 0)
        {
          cursor = 1;
        }
        menu(cursor);
      }
      else if (joyx >= joy_max)
      {
        //Serial.println("Caso 2");
        cursor = cursor + 2;
        if (cursor >= 5)
        {
          cursor = 4;
        }
        menu(cursor);
      }

      //joy y
      if (joyy <= joy_min)
      {
        //Serial.println("Caso 3");
        cursor = cursor + 1;
        if (cursor <= 0)
        {
          cursor = 1;
        }
        menu(cursor);
      }
      else if (joyy >= joy_max)
      {
        //Serial.println("Caso 4");
        cursor = cursor - 1;
        if (cursor >= 5)
        {
          cursor = 4;
        }
        menu(cursor);
      }

      //Select
      if (enter == 1)
      {
        select(cursor);
      }
    }

    if (lock == false)
    {
      //Option Button
      if (option == 1)
      {
        //check isMenuOpened
        if (isMenuOpened == false)
        {
          isMenuOpened = true;
          menu(cursor);
        }
        else if (isMenuOpened == true)
        {
          isMenuOpened = false;
          home();
        }
      }
    }
  }

  loop_cnt++;
  delay(1);
}

void loading(int c)
{
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("   ");
  lcd.write(0);
  lcd.write(1);
  lcd.print(" ch1mera OS");
  switch (c)
  {
  case 1:
    lcd.setCursor(0, 2);
    lcd.print("  ");
    lcd.print(version);
    lcd.print(" Loading.");
    break;
  case 2:
    lcd.setCursor(0, 2);
    lcd.print("  ");
    lcd.print(version);
    lcd.print(" Loading..");
    break;
  case 3:
    lcd.setCursor(0, 2);
    lcd.print("  ");
    lcd.print(version);
    lcd.print(" Loading...");
    delay(500);
    break;
  case 4:
    lcd.setCursor(0, 2);
    lcd.print("       ");
    lcd.print(version);
    break;
  }
  delay(500);
}
void home()
{
  lcd.clear();
  statusBar();
  lcd.setCursor(0, 1);
  lcd.print("Expresion: ");
  lcd.print(current);
  lcd.setCursor(15, 3);
  lcd.print(version);
}
void menu(int c)
{
  lcd.clear();
  statusBar();
  switch (c)
  {
  case 1:
    lcd.setCursor(1, 1);
    lcd.print("> Idle    - Sad    ");
    lcd.setCursor(0, 2);
    lcd.print(" - Happy   - Angry  ");
    lcd.setCursor(15, 3);
    lcd.print(version);
    break;
  case 2:
    lcd.setCursor(0, 1);
    lcd.print(" - Idle    - Sad    ");
    lcd.setCursor(0, 2);
    lcd.print(" > Happy   - Angry  ");
    lcd.setCursor(15, 3);
    lcd.print(version);
    break;
  case 3:
    lcd.setCursor(0, 1);
    lcd.print(" - Idle    > Sad    ");
    lcd.setCursor(0, 2);
    lcd.print(" - Happy   - Angry  ");
    lcd.setCursor(15, 3);
    lcd.print(version);
    break;
  case 4:
    lcd.setCursor(0, 1);
    lcd.print(" - Idle    - Sad    ");
    lcd.setCursor(0, 2);
    lcd.print(" - Happy   > Angry  ");
    lcd.setCursor(15, 3);
    lcd.print(version);
    break;
  }
}
void select(int c)
{
  switch (c)
  {
  case 1:
    current = "Idle";
    currentN = 1;
    break;
  case 2:
    current = "Happy";
    currentN = 2;
    break;
  case 3:
    current = "Sad";
    currentN = 3;
    break;
  case 4:
    current = "Angry";
    currentN = 4;
    break;
  }

  isMenuOpened = false;
  lcd.clear();
  statusBar();
  lcd.setCursor(0, 1);
  lcd.print("      > ");
  lcd.print(current);
  lcd.setCursor(0, 2);
  lcd.print("      selected ");
  lcd.setCursor(15, 3);
  lcd.print(version);
  delay(1000);

  home();
}
void statusBar()
{
  lcd.setCursor(1, 0);
  lcd.write(0);
  lcd.write(1);
  if (isMenuOpened == false)
  {
    if (lock == false)
    {
      lcd.write(3);
    }
    else if (lock == true)
    {
      lcd.write(2);
    }
  }
  else if (isMenuOpened == true)
  {
    lcd.print("Menu ");
    if (lock == false)
    {
      lcd.write(3);
    }
    else if (lock == true)
    {
      lcd.write(2);
    }
  }
}

void expresion()
{
  switch (currentN)
  {
  case 1:
    if (current == "Idle")
    {
      m.writeSprite(104, 0, Eye01L);
      m.writeSprite(96, 0, Eye02L);
      m.writeSprite(8, 0, Eye02);
      m.writeSprite(0, 0, Eye01);
    }
    break;
  case 2:
    if (current == "Happy")
    {
      //program Happy
    }
    break;
  case 3:
    if (current == "Sad")
    {
      //program Sad
    }
    break;
  case 4:
    if (current == "Angry")
    {
      m.writeSprite(64, 0, Glitch044L);
      m.writeSprite(72, 0, Glitch033L);
      m.writeSprite(80, 0, Glitch022L);
      m.writeSprite(88, 0, Glitch011L);
      m.writeSprite(16, 0, Glitch011);
      m.writeSprite(24, 0, Glitch022);
      m.writeSprite(32, 0, Glitch033);
      m.writeSprite(40, 0, Glitch044);
      delay(75);
      counter++;
    }
    if ((counter >= 17) && (counter < 18))
    {
      m.writeSprite(88, 0, icon01L);
      m.writeSprite(80, 0, icon02L);
      m.writeSprite(72, 0, icon03L);
      m.writeSprite(64, 0, icon04L);
      m.writeSprite(40, 0, icon04);
      m.writeSprite(32, 0, icon03);
      m.writeSprite(24, 0, icon02);
      m.writeSprite(16, 0, icon01);
      delay(200);
      counter++;
    }
    if (counter >= 18)
    {
      m.writeSprite(64, 0, Glitch04L);
      m.writeSprite(72, 0, Glitch03L);
      m.writeSprite(80, 0, Glitch02L);
      m.writeSprite(88, 0, Glitch01L);
      m.writeSprite(16, 0, Glitch01);
      m.writeSprite(24, 0, Glitch02);
      m.writeSprite(32, 0, Glitch03);
      m.writeSprite(40, 0, Glitch04);
      delay(75);
      counter = 0;
    }
    break;
  }
  m.writeSprite(88, 0, icon01L); //Mouth and nose
  m.writeSprite(80, 0, icon02L);
  m.writeSprite(72, 0, icon03L);
  m.writeSprite(64, 0, icon04L);
  m.writeSprite(56, 0, noseLeft);
  m.writeSprite(48, 0, noseRight);
  m.writeSprite(40, 0, icon04);
  m.writeSprite(32, 0, icon03);
  m.writeSprite(24, 0, icon02);
  m.writeSprite(16, 0, icon01);
  if (counter2 > 17)
  { //Blink
    for (int i = 0; i < 5; i++)
    {
      column1L = column1L - 1;
      column2L = column2L - 1;
      column1 = column1 - 1;
      column2 = column2 - 1;
      column3L = column3L + 1;
      column4L = column4L + 1;
      column3 = column3 + 1;
      column4 = column4 + 1;
      m.setColumn(column1L, 00000000);
      m.setColumn(column2L, 00000000);
      m.setColumn(column3L, 00000000);
      m.setColumn(column4L, 00000000);
      m.setColumn(column1, 00000000);
      m.setColumn(column2, 00000000);
      m.setColumn(column3, 00000000);
      m.setColumn(column4, 00000000);
      delay(15);
      counter2++;
    }
    column1L = 104;
    column2L = 112;
    column3L = 95;
    column4L = 103;
    column1 = 8;
    column2 = 16;
    column3 = -1;
    column4 = 7;
    counter2 = 0;
  }
  counter2++;
}