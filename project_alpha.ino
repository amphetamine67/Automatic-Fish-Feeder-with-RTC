#include <Wire.h> //adding Wire Library so the I2C comms is possible
#include <Servo.h>; //adding Servo Library
#include <RTClib.h> //adding RTC Library
#include <LiquidCrystal_I2C.h> //adding LCD Library

RTC_DS3231 rtc; //Giving class 'rtc' to DS3231 RTC
LiquidCrystal_I2C lcd(0x27,  16, 2); //addressing LCD on IIC (IIC address - specific numeric identifier to assign a peripheral)
Servo mtr; //giving a class 'mtr' for Servo (you can give it any word beside mtr)

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

void setup() {
  Wire.begin();
  Serial.begin(9600); //Initializing comms between Arduino board and other components with 9600b/s speed

      // Command to detects RTC problem
  if (!rtc.begin()) {
    Serial.println("Couldn't find the RTC. Check the wiring.");
    Serial.flush();
    abort();
    while (1);
  }

  // RTC setup, let the RTC taking date and time info from PC
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //LCD initialization
  lcd.init(); //Initiating LCD
  lcd.backlight(); //Initiating backlight to light up LCD

  // Servo setup
  mtr.attach(9); //Attaching Servo to digital pin no.9
  mtr.write(0); //Writing Servo angle as 0 degree
}




void loop() {

  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.print(", ");
  lcd.print(now.year() % 100);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
  
  int tempC = rtc.getTemperature();
  
  lcd.setCursor(0, 1);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  lcd.print("    ");
  lcd.print(tempC);
  lcd.print("C");

  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(", ");
  Serial.print(now.year() % 100);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.println(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print("    ");
  Serial.print(tempC);
  Serial.print("C");
  Serial.println(' ');

  if (now.hour() == 7 & now.minute() == 30 & now.second() == 0){
    Feeding(2);
  }
  if (now.hour() == 13 & now.minute() == 30 & now.second() == 0){
    Feeding(1);
  }
  if (now.hour() == 19 & now.minute() == 30 & now.second() == 0){
    Feeding(1);
  }
  
  delay(1000);
}

void Feeding(int sum) {
  for(int i=1; i <= sum; i++) {
    mtr.write(90);
    delay(7000);
    mtr.write(0);
  }
}
