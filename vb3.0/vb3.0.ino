/*********************************************************
 *** protoOS * vb3.1 ******************* by NeoYandrak ***
 *********************************************************
 *** Comenzando a programar la matriz de leds para ch1 ***
 *** en nano33 con MAX2791.                            ***
 ********************************************************/

// Include libraries
#include <Wire.h> //For I2C
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <LiquidCrystal_I2C.h> //For the LCD display
#include <MaxMatrix.h>         //For the LED matrix

// Nunchuck Variables
byte joyx, joyy, enter, option;
const int joy_min = 80;
const int joy_max = 170;

// GUI
/*Bool*/ bool isMenuOpened;
bool isDebug = false;
/*Control*/ int cursor = 1;
String current = "Idle";
int currentN = 1;
/*LCD*/ LiquidCrystal_I2C lcd(0x27, 20, 4);

// Status Bar
/*lock*/ bool lock;

//lcd custom icons
byte ch1_L[] = {B00001, B00010, B00100, B00010, B00001, B00000, B00000, B00000};
byte ch1_R[] = {B00000, B00000, B10000, B01000, B00100, B01000, B10000, B00000};
byte lockS[] = {B01110, B10001, B10001, B11111, B11001, B10011, B11111, B00000};
byte openS[] = {B00110, B01001, B00001, B11111, B11001, B10011, B11111, B00000};

// Expresiones
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

// MaxMatrix
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

// Others
int loop_cnt = 0;
int counter = 0;
int counter2 = 0;
String version = "vb3.1";

// Main Program
void setup()
{
  // LCD
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, ch1_L);
  lcd.createChar(1, ch1_R);
  lcd.createChar(2, lockS);
  lcd.createChar(3, openS);
  loading(1);

  // Nunchuck
  Serial.begin(9600);
  nunchuck_setpowerpins();
  nunchuck_init(); // send the initilization handshake
  Serial.println("Wii Nunchuck Ready");
  loading(2);

  // Expresión
  current = "Idle";
  currentN = 1;
  randomSeed(analogRead(0));
  m.init();
  m.setIntensity(9); //0-15
  m.clear();
  loading(3);

  // GUI
  isMenuOpened = false;
  isDebug = false;
  lock = false;
  loading(4);
  home();
}
void loop()
{
  if (loop_cnt > 150)
  { // 150ms
    Serial.println("Loop started");
    loop_cnt = 0;

    // Obtener datos nunchuck
    nunchuck_get_data();
    enter = nunchuck_zbutton();
    option = nunchuck_cbutton();
    joyx = nunchuck_joyx();
    joyy = nunchuck_joyy();
    //printNunchuck();              (Those 2 were used as debug)
    //Serial.println("All good");

    // Buttons
    // Enter Button
    if (enter == 1)
    {
      if (option == 1)
      {
        // check eyeControl
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
    }
    // isDebug
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
    // isMenuOpened
    if (isMenuOpened == true)
    {
      // **Cursor initilization**
      // joy x
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

      // joy y
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

      // Select
      if (enter == 1)
      {
        select(cursor);
      }
    }
    // lockMode
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

// Nunchuck functions
static uint8_t nunchuck_buf[6];
static void nunchuck_setpowerpins()
{
#define pwrpin PORTC3
#define gndpin PORTC2
  delay(100); // wait for things to stabilize
}
static void nunchuck_init()
{
  Wire.begin();                 // join bus as master
  Wire.beginTransmission(0x52); // transmit to device 0x52
#if (ARDUINO >= 100)
  Wire.write((uint8_t)0x40); // sends memory address
  Wire.write((uint8_t)0x00); // sends sent a zero.
#else
  Wire.send((uint8_t)0x40); // sends memory address
  Wire.send((uint8_t)0x00); // sends sent a zero.
#endif
  Wire.endTransmission(); // stop transmitting
}
static void nunchuck_send_request()
{
  Wire.beginTransmission(0x52); // transmit to device 0x52
#if (ARDUINO >= 100)
  Wire.write((uint8_t)0x00); // sends one byte
#else
  Wire.send((uint8_t)0x00); // sends one byte
#endif
  Wire.endTransmission(); // stop transmitting
}
static char nunchuk_decode_byte(char x)
{
  x = (x ^ 0x17) + 0x17;
  return x;
}
static int nunchuck_get_data()
{
  int cnt = 0;
  Wire.requestFrom(0x52, 6); // request data from nunchuck
  while (Wire.available())
  {
    // receive byte as an integer
#if (ARDUINO >= 100)
    nunchuck_buf[cnt] = nunchuk_decode_byte(Wire.read());
#else
    nunchuck_buf[cnt] = nunchuk_decode_byte(Wire.receive());
#endif
    cnt++;
  }
  nunchuck_send_request(); // send request for next data payload
  // If we recieved the 6 bytes, then go print them
  if (cnt >= 5)
  {
    return 1; // success
  }
  return 0; //failure
}
static void nunchuck_print_data()
{
  static int i = 0;
  int joy_x_axis = nunchuck_buf[0];
  int joy_y_axis = nunchuck_buf[1];
  int accel_x_axis = nunchuck_buf[2]; // * 2 * 2;
  int accel_y_axis = nunchuck_buf[3]; // * 2 * 2;
  int accel_z_axis = nunchuck_buf[4]; // * 2 * 2;
  int z_button = 0;
  int c_button = 0;
  // byte nunchuck_buf[5] contains bits for z and c buttons
  // it also contains the least significant bits for the accelerometer data
  // so we have to check each bit of byte outbuf[5]
  if ((nunchuck_buf[5] >> 0) & 1)
    z_button = 1;
  if ((nunchuck_buf[5] >> 1) & 1)
    c_button = 1;
  if ((nunchuck_buf[5] >> 2) & 1)
    accel_x_axis += 1;
  if ((nunchuck_buf[5] >> 3) & 1)
    accel_x_axis += 2;
  if ((nunchuck_buf[5] >> 4) & 1)
    accel_y_axis += 1;
  if ((nunchuck_buf[5] >> 5) & 1)
    accel_y_axis += 2;
  if ((nunchuck_buf[5] >> 6) & 1)
    accel_z_axis += 1;
  if ((nunchuck_buf[5] >> 7) & 1)
    accel_z_axis += 2;
  Serial.print(i, DEC);
  Serial.print("\t");
  Serial.print("joy:");
  Serial.print(joy_x_axis, DEC);
  Serial.print(",");
  Serial.print(joy_y_axis, DEC);
  Serial.print("  \t");
  Serial.print("acc:");
  Serial.print(accel_x_axis, DEC);
  Serial.print(",");
  Serial.print(accel_y_axis, DEC);
  Serial.print(",");
  Serial.print(accel_z_axis, DEC);
  Serial.print("\t");
  Serial.print("but:");
  Serial.print(z_button, DEC);
  Serial.print(",");
  Serial.print(c_button, DEC);
  Serial.print("\r\n"); // newline
  i++;
}
static int nunchuck_zbutton()
{
  return ((nunchuck_buf[5] >> 0) & 1) ? 0 : 1; // voodoo
}
static int nunchuck_cbutton()
{
  return ((nunchuck_buf[5] >> 1) & 1) ? 0 : 1; // voodoo
}
static int nunchuck_joyx()
{
  return nunchuck_buf[0];
}
static int nunchuck_joyy()
{
  return nunchuck_buf[1];
}
void printNunchuck()
{
  Serial.print("Z Button:  ");
  Serial.print(enter);
  Serial.print("\tC Button:  ");
  Serial.print(option);
  Serial.print("\tX Joy:  ");
  Serial.print(map(joyx, 15, 221, 0, 255));
  Serial.print("\tY Joy:  ");
  Serial.println(map(joyy, 29, 229, 0, 255));
}

// GUI functions
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
      break;
    case 2:
      current = "Happy";
      break;
    case 3:
      current = "Sad";
      break;
    case 4:
      current = "Angry";
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

// Expresion
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