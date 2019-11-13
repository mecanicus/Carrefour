#include <Time.h>
#include <Wire.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t

void setup()
   {   Serial.begin(115200);
       while (!Serial) ;              // Solo para el Leonardo
       setSyncProvider(RTC.get);      // Vamos a usar el RTC

       //setTime(21,46,00,8,11,2014); // Las 21:45:00 del dia 8 de Nov de 2014

       if (timeStatus() != timeSet)
           Serial.println("Unable to sync with the RTC");
       else
           Serial.println("RTC has set the system time");
   }
void loop()
   {     digitalClockDisplay();
         delay(1000);
   }
void digitalClockDisplay()
   {     Serial.print(hour());
         printDigits(minute());
         printDigits(second());
         Serial.print(" ");
         Serial.print(day());
         Serial.print(" ");
         Serial.print(month());
         Serial.print(" ");
         Serial.print(year());
         Serial.println();
   }
void printDigits(int digits)
   {     Serial.print(":");
         if(digits < 10)
            Serial.print('0');
         Serial.print(digits);
   }
