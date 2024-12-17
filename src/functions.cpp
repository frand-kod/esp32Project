#include "monitoringLibrary.h"

// Definisi variabel global
DHT dht11(DHTPIN, DHTTYPE);
OneWire oneWire(DS_PIN);
DallasTemperature sensors(&oneWire);
NewPingESP8266 sonar(TRIG_PIN, ECHO_PIN, 400);
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define Baudrate 9600
#define X_LCD 16
#define Y_LCD 2

/****** SETUP INIT ********/
void setupSerial()
{
    Serial.begin(Baudrate);
}
void setupSensordht()
{
    pinMode(DHTPIN, OUTPUT);
    dht11.begin();
}
void setupLcd_I2C()
{
    lcd.begin(X_LCD, Y_LCD);
    lcd.backlight();
}

void setupSensorDallas()
{
    sensors.begin();
}

void setupLed()
{
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
}
void setupPump()
{
    pinMode(RELAYPUMP_PIN, OUTPUT);
}

/** Monitoring Function **/
int dhtProvider(String ValueOption)
{
    return (ValueOption == "humi")   ? dht11.readHumidity()
           : (ValueOption == "temp") ? dht11.readTemperature()
                                     : -1;
}
int dsTemp()
{
    sensors.requestTemperatures();
    int temp = sensors.getTempCByIndex(0); // ambil indek pertama;
    return temp;
}
int distanceSensor()
{
    return sonar.ping_cm();
}
void displayText(int line, int column, const char *text)
{
    // pakai dtostrf(float temp, 0, 2, char tempStr[]);
    // untk Konversi float ke string dengan 2 desimal

    // pakai itoa(tempInt, tempStr, 10);
    // cKonversi integer ke string dengan tipe angka 10an(desimal)

    lcd.setCursor(line, column);
    lcd.print(text);
    lcd.print("    "); // 4 karakter spasi
}

// kontrol pompa sistem
unsigned long previousMillis = 0;
const long intervalGreen = 1000;
const long intervalYellow = 500;
void kontrolPompa(int tempDHT, int treshold)
{
    int intensity = map(tempDHT, 0, 100, 0, 100);
    static bool ledGreenState = LOW;
    static bool ledYellowState = LOW;

    if (millis() - previousMillis >= min(intervalGreen, intervalYellow))
    {
        previousMillis = millis(); // Reset waktu

        // LED Hijau berkedip (setiap intervalGreen)
        if (millis() % intervalGreen == 0)
        {
            ledGreenState = !ledGreenState;
            digitalWrite(LED_GREEN, ledGreenState);
        }

        /** Treshold Zone: treshold - 3 s/d treshold + 3 **/
        if (intensity < (treshold - 3))
        {
            // Suhu rendah: Matikan semua indikator
            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(BUZZER_PIN, LOW);
            digitalWrite(RELAYPUMP_PIN, LOW);
        }
        else if (intensity > (treshold + 3))
        {
            // LED Kuning & Buzzer berkedip (setiap intervalYellow)
            if (millis() % intervalYellow == 0)
            {
                ledYellowState = !ledYellowState;
                digitalWrite(LED_YELLOW, ledYellowState);
                digitalWrite(BUZZER_PIN, ledYellowState);
            }
            digitalWrite(RELAYPUMP_PIN, HIGH); // Pompa aktif
        }
        else
        {
            // Zona Aman: Matikan indikator & pompa
            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(BUZZER_PIN, LOW);
            digitalWrite(RELAYPUMP_PIN, LOW);
        }
    }
}