// ********************************************************************
//
// Copyright (c) 2016, Kerem Bilgicer
//
// ********************************************************************


#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
byte water[8] = {
  B00000, B00100, B00100, B01010, B10001, B10001, B01110, B00000
};

byte valve[8] = {
  B11111, B00100, B11111, B00001, B11101, B00101, B00101, B00000
};

byte right_arrow[8] = {
  B10000, B11000, B11100, B11110, B11100, B11000, B10000, B00000
};

byte left_arrow[8] = {
  B00001, B00011, B00111, B01111, B00111, B00011, B00001, B00000
};

byte up_down_arrow[8] = {
  B00100, B01110, B11111, B00000, B00000, B11111, B01110, B00100
};
byte up_arrow[8] = {
  B00100, B01110, B11111, B00000, B00000, B00000, B00000, B00000
};
byte down_arrow[8] = {
  B00000, B00000, B00000, B00000, B00000, B11111, B01110, B00100
};
byte key[8] = {
  B01110, B10001, B10001, B01110, B00100, B00110, B00100, B00110
};
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
const int _MAIN_MENU_RETURN_SECONDS_ = 10;
int adc_key_in = 0;
int humidity = 45;
int hourValue = 14;
int minuteValue = 34;
int secondValue = 42;
int old_key = btnNONE;
bool isNavigationMode = true;
int screenNo = 0;
int lastActiveScreenNo = 0;
int counter = 0;
int mainMenuReturnCounter = _MAIN_MENU_RETURN_SECONDS_;

int read_LCD_buttons()
{
  adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return btnNONE;

  if (adc_key_in < 67)  return btnRIGHT;
  if (adc_key_in < 223)  return btnUP;
  if (adc_key_in < 399)  return btnDOWN;
  if (adc_key_in < 606)  return btnLEFT;
  if (adc_key_in < 875)  return btnSELECT;
  return btnNONE;
};
void drawScreen()
{
  switch (screenNo)
  {
    //    case 0:
    //      lastActiveScreenNo = screenNo;
    //      lcd.clear();
    //      lcd.setCursor(2, 0);
    //      lcd.print("Akilli Sulama");
    //      lcd.setCursor(3, 1);
    //      lcd.print("K-R System");
    //      break;
    case 0:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      if(isNavigationMode)
      {
        lcd.write(byte(6));
      }
      else
      {
        lcd.write(byte(7));
      }
      lcd.setCursor(2, 0);
      lcd.write(byte(0));
      lcd.write(" %");
      lcd.print(humidity);
      lcd.setCursor(11, 0);
      if (hourValue < 10) lcd.print("0");
      lcd.print(hourValue);
      lcd.print(":");
      if (minuteValue < 10) lcd.print("0");
      lcd.print(minuteValue);
      break;
    case 1:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge");
      lcd.setCursor(1, 1);
      lcd.print("3 gunde 17:30");
      lcd.setCursor(15, 1);
      lcd.write(byte(2));
      break;
    case 2:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("2. Bolge");
      lcd.setCursor(1, 1);
      lcd.print("Hergun  17:30");
      lcd.setCursor(15, 1);
      lcd.write(byte(2));
      break;
    case 3:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge");
      lcd.setCursor(0, 1);
      lcd.print("Sulama Yap");
      break;
    case 4:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("2. Bolge");
      lcd.setCursor(0, 1);
      lcd.print("Sulama Yap");
      break;
    case 5:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(5));
      lcd.setCursor(2, 0);
      lcd.print("Saat Ayarla");
      lcd.setCursor(0, 1);
      lcd.print("xx:xx");
      break;
    case 10:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(6));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);
      lcd.print("Saat xx:xx");
      break;
    case 11:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);
      lcd.print("Nem %x");
      break;
    case 12:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(5));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);
      lcd.print("Sure x dk");
      break;
    default:
      screenNo = lastActiveScreenNo;
      break;
  }
}
void analyseKeys()
{  
  int lcd_key = read_LCD_buttons();
  if (lcd_key != old_key)
  {
    if (lcd_key == btnNONE)
    {
      button_Up_Event(old_key);
    }
    old_key = lcd_key;
  }
}
void button_Up_Event(int button)
{
  mainMenuReturnCounter = _MAIN_MENU_RETURN_SECONDS_;
  if (button == btnSELECT)
  {
    isNavigationMode = !isNavigationMode;
  }
  if (isNavigationMode)
  {
    //lcd.noCursor();
    lcd.noBlink();
    switch (button)
    {
      case btnDOWN:
        screenNo++;
        break;
      case btnUP:
        if (screenNo > 0) screenNo--;
        break;
      case btnLEFT:
        if (screenNo > 0) screenNo = screenNo / 10;
        break;
      case btnRIGHT:
        screenNo = screenNo * 10;
        break;
    }
    drawScreen();
    //lcd.setCursor(2, 0);
    //lcd.print(screenNo);
  }
  else
  {
    //lcd.cursor();
    lcd.blink();
  }
}
void setup()
{
  lcd.begin(16, 2);
  lcd.createChar(0, water);
  lcd.createChar(1, valve);
  lcd.createChar(2, right_arrow);
  lcd.createChar(3, left_arrow);
  lcd.createChar(4, up_down_arrow);
  lcd.createChar(5, up_arrow);
  lcd.createChar(6, down_arrow);
  lcd.createChar(7, key);
  screenNo = 0;
  drawScreen();
}
void mainScreenDisplay()
{
  if (counter % 100 == 0)
  {
    minuteValue++;
    if(isNavigationMode)
    {
      mainMenuReturnCounter--;
    }  
    if(mainMenuReturnCounter < 0)
    {
      mainMenuReturnCounter = _MAIN_MENU_RETURN_SECONDS_;
      isNavigationMode = false;
      screenNo = 0;
    }
    if (!isNavigationMode && screenNo == 0)
    {
      drawScreen();
    }      
  }
}
void loop()
{
  counter++;
  analyseKeys();
  mainScreenDisplay();
  delay(10);

  if (counter > 6000)
  {
    counter = 0;
  }
  
  //  if (j == 0)
  //  {
  //    lcd.setCursor(0, 0);
  //    lcd.print("  ");
  //    lcd.setCursor(2, 0);
  //    lcd.write(byte(4));
  //
  //    lcd.setCursor(3, 0);
  //    lcd.print(" Sensorler:   ");
  //
  //    if (i == 0)
  //    {
  //
  //      lcd.setCursor(15, 1);
  //      lcd.write(byte(2));
  //      lcd.setCursor(0, 1);
  //      lcd.write(byte(6));
  //      lcd.print(" Sicaklik:");
  //      lcd.print("24");
  //      lcd.write(byte(223));
  //      lcd.print("C");
  //
  //    }
  //
  //
  //    if (i == 1)
  //    {
  //
  //      lcd.setCursor(0, 1);
  //      lcd.write(byte(3));
  //      lcd.setCursor(14, 1);
  //      lcd.print(" ");
  //      lcd.setCursor(15, 1);
  //      lcd.write(byte(2));
  //      lcd.setCursor(1, 1);
  //      lcd.write(byte(0));
  //      lcd.print(" ");
  //      lcd.print("Ic Nem: %");
  //      lcd.print(millis() / 1000);
  //      lcd.print(" ");
  //
  //    }
  //    if (i == 2)
  //    {
  //
  //      lcd.setCursor(0, 1);
  //      lcd.write(byte(3));
  //      lcd.setCursor(2, 1);
  //      lcd.print(" Dis Nem: %");
  //      lcd.print("15");
  //      lcd.print("  ");
  //
  //    }
  //
  //    if (i > 2)
  //    {
  //      i = 2;
  //    }
  //
  //  } //menü1 ayarları
  //
  //
  //  if (j == 1)
  //  {
  //    lcd.setCursor(2, 0);
  //    lcd.write(byte(4));
  //    lcd.setCursor(3, 0);
  //    lcd.print("Nemi Ayarla");
  //
  //    lcd.setCursor(0, 1);
  //    lcd.write(byte(3));
  //    lcd.write("-");
  //    lcd.setCursor(2, 1);
  //    lcd.print(" Deger: %");
  //    lcd.print(bfr_nem);
  //
  //    lcd.print(" ");
  //    lcd.setCursor(14, 1);
  //    lcd.print("+");
  //    lcd.write(byte(2));
  //
  //  }  //menü2 ayarları
  //  if (j == 2)
  //  {
  //    lcd.setCursor(2, 0);
  //    lcd.write(byte(4));
  //    lcd.setCursor(3, 0);
  //    lcd.print("Sulama Yap  ");
  //    lcd.setCursor(0, 1);
  //    lcd.print("     ");
  //    lcd.setCursor(5, 1);
  //    lcd.write(byte(1));
  //    lcd.print("   ");
  //    lcd.write(byte(1));
  //    lcd.print("      ");
  //
  //
  //
  //  }


}


