// ********************************************************************
//
// Copyright (c) 2016, Kerem Bilgicer
//
// ********************************************************************


#include <LiquidCrystal.h>
#include <DS1302RTC.h>
#include <Streaming.h>
#include <TimeLib.h>
#include <Wire.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
DS1302RTC RTC(17, 16, 15);


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
int hourValue;
int minuteValue;
int secondValue;
int time_index = 0;
int time_index2 = 8, time_index3 = 2;
int old_key = btnNONE;
int sy = 0;
int zone1_temp_time_hour = -1, zone1_temp_time_minute = 0;
int zone1_temp_day_period = 0;
int zone1_irrigation_timing = 0;
int zone1_humidity = 0;
int zone2_temp_time_hour = -1, zone2_temp_time_minute = 0;
int zone2_temp_day_period = 0;
int zone2_irrigation_timing = 0;
int zone2_humidity = 0;
int sensor1_humidity = 20, sensor2_humidity = 60;
unsigned long zone1_counter, zone2_counter;
bool zone1_irrigation_state = false, zone2_irrigation_state = false;
bool isNavigationMode = true;
int screenNo = 0;
bool ayar = false;
int zone1_change_day, zone2_change_day;
int zone1_relay = 2, zone2_relay = 3;
int lastActiveScreenNo = 0;
int counter = 0;
int temp_hourValue, temp_minuteValue;
int mainMenuReturnCounter = _MAIN_MENU_RETURN_SECONDS_;
tmElements_t tm, tm_zone1, tm_zone2 ;
time_t t, zone1_last_time, zone2_last_time;
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
  setSyncProvider(RTC.get);
  if (timeStatus() == timeSet)
  {
    hourValue = hour();
    minuteValue = minute();
    secondValue = second();
    temp_minuteValue = minuteValue;
    temp_hourValue = hourValue;
  }
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
      if (isNavigationMode)
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
      print2digits(hour());
      lcd.print(":");
      print2digits(minute());
      if (zone1_irrigation_state)
      {
        lcd.setCursor(3, 1);
        lcd.print("B1:");
        lcd.write(byte(1));
      }

      if (zone2_irrigation_state)
      {
        lcd.setCursor(9, 1);
        lcd.print("B2:");
        lcd.write(byte(1));
      }
      break;
    case 1:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge");
      lcd.setCursor(1, 1);
      if (zone1_temp_day_period > 0)
      {
        if (zone1_temp_day_period == 1)
        {
          lcd.print("Hergun ");
        }
        else
        {
          lcd.print(zone1_temp_day_period);
          lcd.print (" gunde ");
        }
      }
      else
      {
        lcd.print("x gunde ");
      }
      if (zone1_temp_time_hour < 0)
        lcd.print("xx:xx");
      else
      {
        print2digits(zone1_temp_time_hour);
        lcd.print(":");
        print2digits(zone1_temp_time_minute);
      }
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
      if (zone2_temp_day_period > 0)
      {
        if (zone2_temp_day_period == 1)
        {
          lcd.print("Hergun ");
        }
        else
        {
          lcd.print(zone2_temp_day_period);
          lcd.print (" gunde ");
        }
      }
      else
      {
        lcd.print("x gunde ");
      }
      if (zone2_temp_time_hour < 0)
        lcd.print("xx:xx");
      else
      {
        print2digits(zone2_temp_time_hour);
        lcd.print(":");
        print2digits(zone2_temp_time_minute);
      }
      lcd.setCursor(15, 1);
      lcd.write(byte(2));
      break;
    case 3:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge ");
      if (zone1_irrigation_state)
      {
        lcd.write(byte(1));
        lcd.setCursor(0, 1);
        lcd.print("Sulama ");
      }
      else
      {

        lcd.setCursor(0, 1);
        lcd.print("Sulama Yap ");
      }
      if (zone1_irrigation_timing == 0 && !zone1_irrigation_state)
      {
        lcd.print("OK");
      }
      if (zone1_irrigation_timing > 0)
      {
        lcd.print(zone1_irrigation_timing);
        lcd.print(" dk");
      }

      break;
    case 4:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("2. Bolge ");
      if (zone2_irrigation_state)
      {
        lcd.write(byte(1));
        lcd.setCursor(0, 1);
        lcd.print("Sulama ");
      }
      else
      {
        lcd.setCursor(0, 1);
        lcd.print("Sulama Yap ");
      }
      if (zone2_irrigation_timing == 0 && !zone2_irrigation_state)
      {
        lcd.print("OK");
      }
      if (zone2_irrigation_timing > 0)
      {
        lcd.print(zone2_irrigation_timing);
        lcd.print(" dk");
      }
      break;
    case 5:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(5));
      lcd.setCursor(2, 0);
      lcd.print("Saat Ayarla");
      lcd.setCursor(0, 1);
      print2digits(hour());
      lcd.print(":");
      print2digits(minute());
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
      if (zone1_temp_time_hour < 0)
        lcd.print("Saat  xx:xx");
      else
      {
        lcd.print("Saat  ");
        print2digits(zone1_temp_time_hour);
        lcd.print(":");
        print2digits(zone1_temp_time_minute);
      }

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
      if (zone1_temp_day_period > 0)
      {
        if (zone1_temp_day_period == 1)
        {
          lcd.print("Hergun       ");
        }
        else
        {
          lcd.print(zone1_temp_day_period);
          lcd.print (" gunde bir    ");
        }
      }
      else
      {
        lcd.print("x gunde bir    ");
      }

      break;
    case 12:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);
      lcd.print("Nem %");
      if (zone1_humidity == 0)
        lcd.print("x");
      else
        lcd.print(zone1_humidity);
      break;

    case 13:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(5));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);
      if (zone1_irrigation_timing <= 0)
        lcd.print("Sure x dk      ");
      else
      {
        lcd.print("Sure ");
        lcd.print(zone1_irrigation_timing);
        lcd.print(" dk      ");
      }
      break;
    case 20:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(6));
      lcd.setCursor(2, 0);
      lcd.print("2. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);
      if (zone2_temp_time_hour < 0)
        lcd.print("Saat  xx:xx");
      else
      {
        lcd.print("Saat  ");
        print2digits(zone2_temp_time_hour);
        lcd.print(":");
        print2digits(zone2_temp_time_minute);
      }

      break;

    case 21:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("2. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);
      if (zone2_temp_day_period > 0)
      {
        if (zone2_temp_day_period == 1)
        {
          lcd.print("Hergun       ");
        }
        else
        {
          lcd.print(zone2_temp_day_period);
          lcd.print (" gunde bir    ");
        }
      }
      else
      {
        lcd.print("x gunde bir    ");
      }

      break;
    case 22:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("2. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);
      lcd.print("Nem %");
      if (zone2_humidity == 0)
        lcd.print("x");
      else
        lcd.print(zone2_humidity);
      break;

    case 23:
      lastActiveScreenNo = screenNo;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(5));
      lcd.setCursor(2, 0);
      lcd.print("2. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);
      if (zone2_irrigation_timing <= 0)
        lcd.print("Sure x dk      ");
      else
      {
        lcd.print("Sure ");
        lcd.print(zone2_irrigation_timing);
        lcd.print(" dk      ");
      }
      break;
    default:
      screenNo = lastActiveScreenNo;
      break;
  }
}


void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);
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
    if (ayar)
      isNavigationMode = false;


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
    if (screenNo == 5)
    {
      ayar = true;
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.write(byte(5));
      lcd.setCursor(2, 0);
      lcd.print("Saat Ayarla");
      lcd.setCursor(0, 1);
      if (button == btnRIGHT)
      {

        time_index = 3; //minute selected
      }
      if (button == btnLEFT)
      {

        time_index = 0; //hour selected
      }

      if (button == btnDOWN)
      {
        if (time_index == 3)
        {
          temp_minuteValue--;
          if (temp_minuteValue < 0)
            temp_minuteValue = 59;
        }
        if (time_index == 0)
        {
          temp_hourValue--;
          if (temp_hourValue < 0)
            temp_hourValue = 23;
        }
      }

      if (button == btnUP)
      {
        if (time_index == 3)
        {
          temp_minuteValue++;
          if (temp_minuteValue > 59)
            temp_minuteValue = 0;
        }
        if (time_index == 0)
        {
          temp_hourValue++;
          if (temp_hourValue > 23)
            temp_hourValue = 0;
        }
      }

      if (button == btnSELECT)
      {
        ayar = false;
        tm.Year = CalendarYrToTm(year());
        tm.Month = month();
        tm.Day = day();
        tm.Hour = temp_hourValue;
        tm.Minute = temp_minuteValue;
        tm.Second = second();
        t = makeTime(tm);
        if (RTC.set(t) == 0) {
          setTime(t);
        }
        sy++;
        if (sy >= 2)
        {
          isNavigationMode = true;
          sy = 0;
          screenNo = 5;
          drawScreen();
          lcd.noBlink();
        }

      }
      lcd.setCursor(0, 1);
      print2digits(temp_hourValue);
      lcd.print(":");
      print2digits(temp_minuteValue);

      lcd.setCursor(time_index, 1);

    }

    if (screenNo == 10)
    {
      ayar = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(6));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);
      lcd.print("Saat  xx:xx");
      if (button == btnRIGHT)
      {

        time_index2 = 11; //minute selected
      }
      if (button == btnLEFT)
      {

        time_index2 = 8; //hour selected
      }

      if (button == btnDOWN)
      {

        if (time_index2 == 11)
        {

          zone1_temp_time_minute -= 5;
          if (zone1_temp_time_minute < 0)
            zone1_temp_time_minute = 55;
        }
        if (time_index2 == 8)
        {

          zone1_temp_time_hour--;
          if (zone1_temp_time_hour < 0)
            zone1_temp_time_hour = 23;
        }
      }

      if (button == btnUP)
      {
        if (time_index2 == 11)
        {
          zone1_temp_time_minute += 5;
          if (zone1_temp_time_minute > 59)
            zone1_temp_time_minute = 0;
        }
        if (time_index2 == 8)
        {

          zone1_temp_time_hour++;
          if (zone1_temp_time_hour > 23)
            zone1_temp_time_hour = 0;
        }
      }

      if (button == btnSELECT)
      {
        ayar = false;

        sy++;
        if (sy >= 2)
        {
          isNavigationMode = true;
          sy = 0;
          screenNo = 10;
          drawScreen();
          lcd.noBlink();
        }

      }
      if (zone1_temp_time_hour >= 0)
      {

        lcd.setCursor(8, 1);
        print2digits(zone1_temp_time_hour);
        lcd.print(":");
        print2digits(zone1_temp_time_minute);
        lcd.setCursor(time_index2, 1);

      }
      else
      {

        lcd.setCursor(8, 1);
        lcd.print("xx:xx");
        lcd.setCursor(time_index2, 1);

      }
    }
    if (screenNo == 11)
    {
      ayar = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);

      if (button == btnDOWN)
      {
        zone1_temp_day_period--;
        if (zone1_temp_day_period < 1)
          zone1_temp_day_period = 1;
      }

      if (button == btnUP)
      {
        zone1_temp_day_period++;
        if (zone1_temp_day_period > 30)
          zone1_temp_day_period = 30;
      }

      if (button == btnSELECT)
      {
        ayar = false;

        sy++;
        if (sy >= 2)
        {
          isNavigationMode = true;
          sy = 0;
          screenNo = 11;
          tm_zone1.Year = CalendarYrToTm(year());
          tm_zone1.Month = month();
          tm_zone1.Day = day();
          tm_zone1.Hour = hour();
          tm_zone1.Minute = minute();
          tm_zone1.Second = second();
          zone1_last_time = makeTime(tm_zone1);
          zone1_change_day = day();
          drawScreen();
          lcd.noBlink();
        }

      }
      if (zone1_temp_day_period > 0)
      {
        if (zone1_temp_day_period == 1)
        {
          lcd.print("Hergun  ");
        }
        else
        {
          lcd.print(zone1_temp_day_period);
          lcd.print (" gunde bir");
        }
      }
      else
      {
        lcd.print("x gunde bir");
      }
      lcd.setCursor(time_index3, 1);
    }

    if (screenNo == 12)
    {
      ayar = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);
      lcd.print("Nem %");

      if (button == btnDOWN)
      {
        zone1_humidity -= 10;
        if (zone1_humidity < 10)
          zone1_humidity = 10;
      }

      if (button == btnUP)
      {
        zone1_humidity += 10;
        if (zone1_humidity > 90)
          zone1_humidity = 90;
      }

      if (button == btnSELECT)
      {
        ayar = false;

        sy++;
        if (sy >= 2)
        {
          isNavigationMode = true;
          sy = 0;
          screenNo = 12;
          drawScreen();
          lcd.noBlink();
        }

      }
      lcd.setCursor(7, 1);
      lcd.print(zone1_humidity);
      lcd.setCursor(7, 1);
    }
    if (screenNo == 13)
    {
      ayar = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(5));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);

      if (button == btnDOWN)
      {
        zone1_irrigation_timing--;
        if (zone1_irrigation_timing < 1)
          zone1_irrigation_timing = 1;
      }

      if (button == btnUP)
      {
        zone1_irrigation_timing ++;
        if (zone1_irrigation_timing > 99)
          zone1_irrigation_timing = 99;
      }

      if (button == btnSELECT)
      {
        ayar = false;

        sy++;
        if (sy >= 2)
        {
          isNavigationMode = true;
          sy = 0;
          screenNo = 13;
          drawScreen();
          lcd.noBlink();
        }

      }
      if (zone1_irrigation_timing <= 0)
        lcd.print("Sure x dk      ");
      else
      {
        lcd.print("Sure ");
        lcd.print(zone1_irrigation_timing);
        lcd.print(" dk      ");
      }
      lcd.setCursor(7, 1);

    }
    //2.bolge-------------------------------------------------------//
    if (screenNo == 20)
    {
      ayar = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(6));
      lcd.setCursor(2, 0);
      lcd.print("2. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);
      lcd.print("Saat  xx:xx");
      if (button == btnRIGHT)
      {

        time_index2 = 11; //minute selected
      }
      if (button == btnLEFT)
      {

        time_index2 = 8; //hour selected
      }

      if (button == btnDOWN)
      {

        if (time_index2 == 11)
        {

          zone2_temp_time_minute -= 5;
          if (zone2_temp_time_minute < 0)
            zone2_temp_time_minute = 55;
        }
        if (time_index2 == 8)
        {

          zone2_temp_time_hour--;
          if (zone2_temp_time_hour < 0)
            zone2_temp_time_hour = 23;
        }
      }

      if (button == btnUP)
      {
        if (time_index2 == 11)
        {
          zone2_temp_time_minute += 5;
          if (zone2_temp_time_minute > 59)
            zone2_temp_time_minute = 0;
        }
        if (time_index2 == 8)
        {

          zone2_temp_time_hour++;
          if (zone2_temp_time_hour > 23)
            zone2_temp_time_hour = 0;
        }
      }

      if (button == btnSELECT)
      {
        ayar = false;

        sy++;
        if (sy >= 2)
        {
          isNavigationMode = true;
          sy = 0;
          screenNo = 20;
          drawScreen();
          lcd.noBlink();
        }

      }
      if (zone2_temp_time_hour >= 0)
      {

        lcd.setCursor(8, 1);
        print2digits(zone2_temp_time_hour);
        lcd.print(":");
        print2digits(zone2_temp_time_minute);
        lcd.setCursor(time_index2, 1);

      }
      else
      {

        lcd.setCursor(8, 1);
        lcd.print("xx:xx");
        lcd.setCursor(time_index2, 1);

      }
    }
    if (screenNo == 21)
    {
      ayar = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("2. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);

      if (button == btnDOWN)
      {
        zone2_temp_day_period--;
        if (zone2_temp_day_period < 1)
          zone2_temp_day_period = 1;
      }

      if (button == btnUP)
      {
        zone2_temp_day_period++;
        if (zone2_temp_day_period > 30)
          zone2_temp_day_period = 30;
      }

      if (button == btnSELECT)
      {
        ayar = false;

        sy++;
        if (sy >= 2)
        {
          isNavigationMode = true;
          sy = 0;
          screenNo = 21;
          drawScreen();
          tm_zone2.Year = CalendarYrToTm(year());
          tm_zone2.Month = month();
          tm_zone2.Day = day();
          tm_zone2.Hour = hour();
          tm_zone2.Minute = minute();
          tm_zone2.Second = second();
          zone2_last_time = makeTime(tm_zone2);
          zone2_change_day = day();
          lcd.noBlink();
        }

      }
      if (zone2_temp_day_period > 0)
      {
        if (zone2_temp_day_period == 1)
        {
          lcd.print("Hergun  ");
        }
        else
        {
          lcd.print(zone2_temp_day_period);
          lcd.print (" gunde bir");
        }
      }
      else
      {
        lcd.print("x gunde bir");
      }
      lcd.setCursor(time_index3, 1);
    }

    if (screenNo == 22)
    {
      ayar = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("2. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);
      lcd.print("Nem %");

      if (button == btnDOWN)
      {
        zone2_humidity -= 10;
        if (zone2_humidity < 10)
          zone2_humidity = 10;
      }

      if (button == btnUP)
      {
        zone2_humidity += 10;
        if (zone2_humidity > 90)
          zone2_humidity = 90;
      }

      if (button == btnSELECT)
      {
        ayar = false;

        sy++;
        if (sy >= 2)
        {
          isNavigationMode = true;
          sy = 0;
          screenNo = 22;
          drawScreen();
          lcd.noBlink();
        }

      }
      lcd.setCursor(7, 1);
      lcd.print(zone2_humidity);
      lcd.setCursor(7, 1);
    }
    if (screenNo == 23)
    {
      ayar = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(5));
      lcd.setCursor(2, 0);
      lcd.print("2. Bolge Ayar ");
      lcd.setCursor(0, 1);
      lcd.write(byte(3));
      lcd.setCursor(2, 1);

      if (button == btnDOWN)
      {
        zone2_irrigation_timing--;
        if (zone2_irrigation_timing < 1)
          zone2_irrigation_timing = 1;
      }

      if (button == btnUP)
      {
        zone2_irrigation_timing ++;
        if (zone2_irrigation_timing > 99)
          zone2_irrigation_timing = 99;
      }

      if (button == btnSELECT)
      {
        ayar = false;

        sy++;
        if (sy >= 2)
        {
          isNavigationMode = true;
          sy = 0;
          screenNo = 23;
          drawScreen();
          lcd.noBlink();
        }

      }
      if (zone2_irrigation_timing <= 0)
        lcd.print("Sure x dk      ");
      else
      {
        lcd.print("Sure ");
        lcd.print(zone2_irrigation_timing);
        lcd.print(" dk      ");
      }
      lcd.setCursor(7, 1);

    }
    if (screenNo == 3)
    {
      lcd.noBlink();
      ayar = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("1. Bolge ");
      if (zone1_irrigation_state)
        lcd.write(byte(1));
      lcd.setCursor(0, 1);
      lcd.print("Sulama Yap ");
      if (zone1_irrigation_timing > 0)
      {
        lcd.print(zone1_irrigation_timing);
        lcd.print(" dk");
      }

      if (button == btnSELECT)
      {
        ayar = false;
        zone1_irrigation_state = !zone1_irrigation_state;
        if (zone1_irrigation_state)
        {
          digitalWrite(zone1_relay, LOW);
        }
        else
        {
          digitalWrite(zone1_relay, HIGH);
        }
        isNavigationMode = true;
        screenNo = 3;
        drawScreen();
      }

    }

    if (screenNo == 4)
    {
      lcd.noBlink();
      ayar = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(4));
      lcd.setCursor(2, 0);
      lcd.print("2. Bolge ");
      if (zone2_irrigation_state)
        lcd.write(byte(1));
      lcd.setCursor(0, 1);
      lcd.print("Sulama Yap ");
      if (zone2_irrigation_timing > 0)
      {
        lcd.print(zone2_irrigation_timing);
        lcd.print(" dk");
      }

      if (button == btnSELECT)
      {
        ayar = false;
        zone2_irrigation_state = !zone2_irrigation_state;
        if (zone2_irrigation_state)
        {
          digitalWrite(zone2_relay, LOW);
        }
        else
        {
          digitalWrite(zone2_relay, HIGH);
        }
        isNavigationMode = true;
        screenNo = 4;
        drawScreen();
      }

    }

  }
}
void setup()
{

  lcd.begin(16, 2);
  pinMode(zone1_relay, OUTPUT);
  digitalWrite(zone1_relay, HIGH);
  pinMode(zone2_relay, OUTPUT);
  digitalWrite(zone2_relay, HIGH);
  setSyncProvider(RTC.get);
  if (timeStatus() == timeSet)
  {
    hourValue = hour();
    minuteValue = minute();
    secondValue = second();
  }
  delay(100);
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
    if (isNavigationMode)
    {
      mainMenuReturnCounter--;
    }
    if (mainMenuReturnCounter < 0)
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
  //sensor1_humidity = analogRead(A4);
  //sensor2_humidity = analogRead(A5);
  /* if (day() - zone1_change_day < 0)
    {
     if (abs(day() - zone1_change_day) % zone1_temp_day_period == 0)
     {
       if (zone1_temp_time_hour == hour() && zone1_temp_time_minute == minute() && zone1_humidity > sensor1_humidity)
       {
         digitalWrite(zone1_relay, LOW);
         zone1_irrigation_state = true;
         zone1_change_day = day();
       }

     }
    }
    if ((day() - zone1_change_day) == zone1_temp_day_period || day() == zone1_change_day)
    {
     if (zone1_temp_time_hour == hour() && zone1_temp_time_minute == minute() && zone1_humidity > sensor1_humidity)
     {
       digitalWrite(zone1_relay, LOW);
       zone1_irrigation_state = true;
       zone1_change_day = day();
     }
    }*/


  if (((long(now()) - zone1_last_time > long(zone1_temp_day_period) * 3600 * 24) && (long(now()) - zone1_last_time < long(zone1_temp_day_period + 1) * 3600 * 24)) || ((long(now()) - zone1_last_time > 0 && long(now()) - zone1_last_time < 86400)))
  {
    if (zone1_temp_time_hour == hour() && zone1_temp_time_minute == minute() && zone1_humidity > sensor1_humidity &&  !zone1_irrigation_state)
    {

      digitalWrite(zone1_relay, LOW);
      zone1_irrigation_state = true;
      tm_zone1.Year = CalendarYrToTm(year());
      tm_zone1.Month = month();
      tm_zone1.Day = day();
      tm_zone1.Hour = hour();
      tm_zone1.Minute = minute();
      tm_zone1.Second = second();
      zone1_last_time = makeTime(tm_zone1);
    }
  }

  if (((long(now()) - zone2_last_time > long(zone2_temp_day_period) * 3600 * 24) && (long(now()) - zone2_last_time < long(zone2_temp_day_period + 1) * 3600 * 24)) || ((long(now()) - zone2_last_time > 0 && long(now()) - zone2_last_time < 86400)))
  {
    if (zone2_temp_time_hour == hour() && zone2_temp_time_minute == minute() && zone2_humidity > sensor2_humidity &&  !zone2_irrigation_state)
    {
      digitalWrite(zone2_relay, LOW);
      zone2_irrigation_state = true;
      tm_zone2.Year = CalendarYrToTm(year());
      tm_zone2.Month = month();
      tm_zone2.Day = day();
      tm_zone2.Hour = hour();
      tm_zone2.Minute = minute();
      tm_zone2.Second = second();
      zone2_last_time = makeTime(tm_zone2);
    }
  }



  if (counter > 6000)
  {
    counter = 0;
  }


  if (zone1_irrigation_state && zone1_irrigation_timing > 0)
  {
    zone1_counter++;
    if (zone1_counter >= long(zone1_irrigation_timing) * 6000)
    {

      digitalWrite(zone1_relay, HIGH);
      zone1_counter = 0;
      zone1_irrigation_state = false;
    }
  }

  if (zone2_irrigation_state && zone2_irrigation_timing > 0)
  {
    zone2_counter++;
    if (zone2_counter >= long(zone2_irrigation_timing) * 6000)
    {

      digitalWrite(zone2_relay, HIGH);
      zone2_counter = 0;
      zone2_irrigation_state = false;
    }
  }

