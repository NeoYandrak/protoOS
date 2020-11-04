/*********************************************************
 *** protoOS * vb3.0 ******************* by NeoYandrak ***
 *********************************************************
 *** Reescribir para Arduino Nano 33 Ble, ya que usa   ***
 *** una arquitectura diferente y las librerías no me  ***
 *** sirven.                                           ***
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

// Others
int loop_cnt = 0;
String version = "vb3.0";

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
  Wire.begin();                 // join i2c bus as master
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