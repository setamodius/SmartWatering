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

byte down_arrow[8] = {
  B00100, B01110, B11111, B00000, B00000, B11111, B01110, B00100
};
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
int adc_key_in = 0;
int humidity = 45;
int hourValue = 0;
int minuteValue = 0;
int secondValue = 0;
int old_key = btnNONE;
bool isNavigationMode = true;
int screenNo = 0;

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
void drawScreen(int screenno)
{
  lcd.clear();
  switch (screenno)
  {
    case 0:
      lcd.setCursor(2, 0);
      lcd.print("Akilli Sulama");
      lcd.setCursor(3, 1);
      lcd.print("K-R System");
      break;
    case 1:
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
      lcd.write(" %");
      lcd.print(humidity);
      lcd.setCursor(8, 0);
      if (hourValue < 10) lcd.print("0");
      lcd.print(hourValue);
      lcd.print(":");
      if (minuteValue < 10) lcd.print("0");
      lcd.print(minuteValue);
      lcd.print(":");
      if (secondValue < 10) lcd.print("0");
      lcd.print(secondValue);
      break;
    case 2:
      lcd.setCursor(4, 0);
      lcd.print("1. Bolge");
      lcd.setCursor(0, 1);
      lcd.print("2 gunde    16:30");
      break;
    case 3:
      lcd.setCursor(4, 0);
      lcd.print("2. Bolge");
      lcd.setCursor(0, 1);
      lcd.print("Hergun     17:30");
      break;
    case 4:
      lcd.setCursor(0, 0);
      lcd.print("1. Bolge");
      lcd.setCursor(0, 1);
      lcd.print("Sulama Yap");
      break;
    case 5:
      lcd.setCursor(0, 0);
      lcd.print("2. Bolge");
      lcd.setCursor(0, 1);
      lcd.print("Sulama Yap");
      break;
    case 6:
      lcd.setCursor(0, 0);
      lcd.print("Saat Ayarla");
      lcd.setCursor(0, 1);
      lcd.print("xx:xx");
      break;
    case 20:
      lcd.setCursor(0, 0);
      lcd.print("+ 1. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.print("Saat xx:xx");
      break;
    case 21:
      lcd.setCursor(0, 0);
      lcd.print("+ 1. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.print("Saat xx:xx");
      break;
    case 22:
      lcd.setCursor(0, 0);
      lcd.print("+ 1. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.print("Nem %x");
      break;
    case 23:
      lcd.setCursor(0, 0);
      lcd.print("+ 1. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.print("Sure x dk");
      break;
    case 200:
      lcd.setCursor(0, 0);
      lcd.print("Sulama saati");
      lcd.setCursor(0, 1);
      lcd.print("xx:xx ayarlandi");
      break;
  }
}
void analyseKeys()
{
  int lcd_key = read_LCD_buttons();
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print(analogRead(0));
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
  if (isNavigationMode)
  {
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
    //drawScreen(screenNo);
  }
}
void setup()
{
  lcd.begin(16, 2);
  lcd.createChar(0, water);
  lcd.createChar(1, valve);
  lcd.createChar(2, right_arrow);
  lcd.createChar(3, left_arrow);
  lcd.createChar(4, down_arrow);
  drawScreen(0);
  delay(1000);
  drawScreen(1);
}
void loop()
{
  analyseKeys();
  delay(1);
  //  lcd_key = read_LCD_buttons();
  //  switch (lcd_key)
  //  {
  //    case btnDOWN:
  //      {
  //        if (bt_alt == false)
  //        {
  //          bt_alt = true;
  //          j--;
  //          if (j == 1) {
  //            nem_ayar = true;
  //            el_su = false;
  //            bfr_nem = nem;
  //          }
  //          else if (j == 2) {
  //            el_su = true;
  //            nem_ayar = false;
  //          }
  //          else {
  //            nem_ayar = false;
  //            el_su = false;
  //          }
  //          i = 0;
  //          if (j < 0) {
  //            j = 0;
  //          }
  //        }
  //        break;
  //      }
  //    case btnUP:
  //      {
  //        if (bt_ust == false)
  //        {
  //          bt_ust = true;
  //          j++;
  //          if (j > 2) {
  //            j = 2; //menü sonu
  //          }
  //          if (j == 1) {
  //            nem_ayar = true;
  //            bfr_nem = nem;
  //            el_su = false;
  //          }
  //          else if (j == 2) {
  //            el_su = true;
  //            nem_ayar = false;
  //          }
  //          else {
  //            nem_ayar = false;
  //            el_su = false;
  //          }
  //
  //          i = 0;
  //        }
  //        break;
  //      }
  //
  //    case btnRIGHT:
  //      {
  //        if (bt_sag == false)
  //        {
  //          bt_sag = true;
  //          i++;
  //          if (nem_ayar)
  //          {
  //            bfr_nem = bfr_nem + 5;
  //            if (bfr_nem > 95)
  //            {
  //              bfr_nem = 95;
  //            }
  //          }
  //        }
  //        break;
  //
  //      }
  //    case btnLEFT:
  //      {
  //        if (bt_sol == false)
  //        {
  //          i--;
  //          if (i < 0) {
  //            i = 0;
  //          }
  //          bt_sol = true;
  //          if (nem_ayar)
  //          {
  //            bfr_nem = bfr_nem - 5;
  //            if (bfr_nem < 10)
  //            {
  //              bfr_nem = 10;
  //            }
  //          }
  //        }
  //
  //        break;
  //
  //      }
  //    case btnNONE:
  //      {
  //        bt_sag = false;
  //        bt_sol = false;
  //        bt_ust = false;
  //        bt_alt = false;
  //        break;
  //
  //      }
  //    case btnSELECT:
  //      {
  //        if (nem_ayar)
  //        {
  //          lcd.clear();
  //          nem = bfr_nem;
  //          lcd.setCursor(0, 0);
  //          lcd.print("Nem degeri");
  //          lcd.print("% ");
  //          lcd.print(nem);
  //          lcd.setCursor(0, 1);
  //          lcd.print("olarak ayarlandi");
  //          delay(1500);
  //          lcd.clear();
  //          nem_ayar = false;
  //          i = 0; j = 0;
  //        }
  //        if (el_su)
  //        {
  //          lcd.clear();
  //          lcd.setCursor(0, 0);
  //          lcd.write(byte(1));
  //          lcd.setCursor(1, 0);
  //          lcd.print("    SULAMA");
  //          lcd.setCursor(15, 0);
  //          lcd.write(byte(1));
  //          lcd.setCursor(0, 1);
  //          for (int t = 0; t < 16; t++)
  //            lcd.write(byte(0));
  //          delay(3000);
  //          el_su = false;
  //          i = 0; j = 0;
  //        }
  //        break;
  //      }
  //
  //  }
  //
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


