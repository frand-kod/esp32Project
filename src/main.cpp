// Sistem Monitoring Kolam Final Project

#include "monitoringLibrary.h"
int displayMode = 0; // 0 untuk suhu, 1 untuk kelembaban, 2 untuk data lainnya
void setup()
{
  // put your setup code here, to run once:
  void setupSerial();
  void setupSensordht();
  void setupLcd_I2C();
  void setupSensorDallas();
  void setupLed();
  void setupPump();

  lcd.println("Monitoring Kolam");
  lcd.println("By Kel 6 ");
  delay(200);
}

void loop()
{
  // put your main code here, to run repeatedly:
  char temp[3];
  char tempair[3];
  char tinggi[3];
  int tempAirKolam = dsTemp();
  int tempLingkungan = dhtProvider("temp");
  int ketinggian = distanceSensor();
  int treshold = 27; // andaikan suhu batas == 27 C

  if (displayMode == 0)
  {
    lcd.setCursor(1, 0);
    lcd.print("lingkungan: ");
    displayText(1, 13, itoa(tempLingkungan, temp, 10)); // tampilkan di kolom ke 13
    lcd.print(" C");
  }
  else if (displayMode == 1)
  {
    lcd.setCursor(1, 0);
    lcd.print("Kolam : ");
    displayText(1, 13, itoa(tempAirKolam, tempair, 10)); // tampilkan di kolom ke 13
    lcd.print(" C");
  }
  else if (displayMode == 2)
  {
    lcd.setCursor(1, 0);
    lcd.print("Ketinggian : ");
    displayText(1, 13, itoa(ketinggian, tinggi, 10)); // tampilkan di kolom ke 13
    lcd.print(" C");
  }

  displayMode = (displayMode + 1) % 3; // Pergantian antara suhu, kelembaban, dan data lainnya

  kontrolPompa(tempAirKolam, treshold);

  delay(1000);
}
