/*********************************************************
 *** protoOS * vb1.4 ******************* by NeoYandrak ***
 *********************************************************
 *** Optimicing code for arduino and deleting some     ***
 *** things to better space management.                ***
 ********************************************************/

// Include libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "nunchuck_funcs.h"

//Nunchuck variables
byte joyx, joyy, enter, option;
const int joy_min = 80;
const int joy_max = 170;

//GUI
/*Bool*/ bool isMenuOpened;
bool isDebug = false;
/*Control*/ int cursor = 1;
String current = "Idle";
/*LCD*/ LiquidCrystal_I2C lcd(0x27, 20, 4);

//Status Bar
/*lock*/ bool lock;

//lcd custom icons
byte ch1_L[] = {B00001, B00010, B00100, B00010, B00001, B00000, B00000, B00000};
byte ch1_R[] = {B00000, B00000, B10000, B01000, B00100, B01000, B10000, B00000};
byte lock[] = {B01110, B10001, B10001, B11111, B11001, B10011, B11111, B00000};
byte open[] = {B00110, B01001, B00001, B11111, B11001, B10011, B11111, B00000};

//Others
int loop_cnt = 0;

String version = "vb1.4";

void setup()
{
	//LCD
	lcd.init();
	lcd.backlight();
	lcd.createChar(0, ch1_L);
	lcd.createChar(1, ch1_R);
	lcd.createChar(2, lock);
	lcd.createChar(3, open);
	loading(1);

	//Nunchuk
	nunchuck_setpowerpins();
	nunchuck_init();
	loading(2);

	//Expresión
	current = "Idle";
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
		if (enter == 1)
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
		}

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