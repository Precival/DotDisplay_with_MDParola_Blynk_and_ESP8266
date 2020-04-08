#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <EEPROM.h>

#include <DHT.h>

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "Font_SPECIAL.h"

#define VIRTUAL_MSG V0
#define VIRTUAL_ILUMINATION V1
#define VIRTUAL_TEMPERATURE V2
#define VIRTUAL_HUMIDITY V3

#define MAX_DEVICES 16                    // Number of modules connected
#define CLK_PIN 14                        // or SCK
#define DATA_PIN 13                       // or MOSI
#define CS_PIN 12                         // or SS
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW // type display
#define SPEED_TIME 25
#define PAUSE_TIME 0

// < Global variables >
// Display config
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
MD_MAX72XX::fontType_t *defaultFont;

// Animation display config
unsigned long lastMoved = 0;
unsigned long lastReadingDHT = 0;
unsigned long changeMSG = 0;
unsigned int holdTimeMSG = 0;
unsigned int vBarState = 0;
bool firstTimeToShow = false;
bool blinkTime = false;

// Blynk config
WidgetRTC rtc;
char msg_time[30] = "";         // Time
char msg_custom[128] = "";      // Last message
char msg_Date_and_DHT[50] = ""; // Date, temperature and humidity
byte crrMsg = 0;

// WIFI and BlynkAuth config
char auth[] = "uCwWLBGxMAt7daJGOhiptFsa7cZadbwb";
char ssid[] = "brisa-214346";
char pass[] = "wf9ab383";

// DHT config
DHT dht(5, DHT22); //DHTPIN, DHTTYPE
float temperatureDHT = 0,
      humidityDHT = 0;
bool readedDHT = false;

// < Global variables />

// < Global functions >
// EEPROM
void saveMsg(String *msg);
String recoverMsg();

// Build texts to show
void buildMsg_custom();
void buildMsg_time();
void buildMsg_DateAndDHT(String *msgFromApp);

// Apply changes when needed
void actDisplay0_run();
void actDisplay1_run();
void readingDHT_run();
// < Global functions />

// < Blynk functions >
BLYNK_CONNECTED()
{
  rtc.begin();
  Blynk.syncAll();
  digitalWrite(16, HIGH);
}
BLYNK_APP_DISCONNECTED()
{
  digitalWrite(16, LOW);
}
BLYNK_WRITE(VIRTUAL_MSG)
{
  String msgApp = param.asStr();
  saveMsg(&msgApp);
  buildMsg_custom(&msgApp);
}
BLYNK_WRITE(VIRTUAL_ILUMINATION)
{
  int ilumination = param.asInt();
  if (ilumination <= 100 && ilumination >= 0)
  {
    int ilumValue = map(ilumination, 0, 100, 0, 15);
    P.setIntensity(0, ilumValue);
    P.setIntensity(1, ilumValue);
    P.displayReset(0);
    P.displayReset(1);
  }
  Serial.print("Ilu: ");
  Serial.println(ilumination);
}
// < Blynk functions />

void setup()
{
  pinMode(2, OUTPUT);
  pinMode(16, OUTPUT);
  EEPROM.begin(128);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  // To wait for above functions to start
  delay(10);

  // Recover custom message from memory
  String msg = recoverMsg();
  msg.toCharArray(msg_custom, 128);

  // Initialize the dot display
  Serial.println("Initializing display...");
  for (byte i = 0; i < 4; i++)
  {
    delay(100);
    P.begin(2);

    // Zones Config
    P.setZone(0, 0, 9);
    P.setZone(1, 10, 15);
    P.setIntensity(0, 0);
    P.setIntensity(1, 0);
    P.displayZoneText(0, msg_custom, PA_CENTER, SPEED_TIME, 0, PA_PRINT, PA_SCROLL_LEFT);
    P.displayZoneText(1, msg_time, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
    //P.addChar(0, '$', degC); // °C

    defaultFont = P.getFont(0);

    P.setFont(1, special);
  }

  delay(100);
  Serial.println("Ready");
}

void loop()
{
  Blynk.run();
  P.displayAnimate();
  actDisplay0_run();
  actDisplay1_run();
  readingDHT_run();
}

// < MORE > //

// RunWithoutDelay CODE
void actDisplay0_run()
{
  if (millis() - lastMoved >= 500)
  {
    lastMoved = millis();
    buildMsg_time();
    P.displayReset(1);
  }
}
void actDisplay1_run()
{
  if (P.getZoneStatus(0) && (millis() - changeMSG >= holdTimeMSG))
  {
    switch (crrMsg)
    {
    case 0: // Show date, humidity and temperature.
      P.setFont(0, special);
      P.displayZoneText(0, msg_Date_and_DHT, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_SCROLL_DOWN, PA_NO_EFFECT);
      holdTimeMSG = 8000; // animation time more 8 seconds
      crrMsg++;
      break;

    case 1: // Hide date, humidity and temperature.
      P.setFont(0, special);
      P.displayZoneText(0, msg_Date_and_DHT, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_SCROLL_DOWN);
      holdTimeMSG = 0; // ONLY ANIMATION TIME.
      crrMsg++;
      break;

    case 2: // Show custom message.
      P.setFont(0, defaultFont);
      P.displayZoneText(0, msg_custom, PA_CENTER, SPEED_TIME, 0, PA_PRINT, PA_SCROLL_LEFT);
      P.setTextEffect(0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      holdTimeMSG = 0; // only animation time
      crrMsg = 0;
      break;

    default:
      break;
    }
    changeMSG = millis();
    P.displayReset(0);
  }
}
void readingDHT_run()
{
  if (millis() - lastReadingDHT >= 5000)
  {
    digitalWrite(2, HIGH);
    lastReadingDHT = millis();
    buildMsg_DateAndDHT();

    Blynk.virtualWrite(VIRTUAL_TEMPERATURE, temperatureDHT);
    Blynk.virtualWrite(VIRTUAL_HUMIDITY, humidityDHT);
    Blynk.syncVirtual(VIRTUAL_TEMPERATURE, VIRTUAL_HUMIDITY);

    digitalWrite(2, LOW);
  }
}

// Build message CODE
void buildMsg_time()
{

  String textIn = "";
  int _hour = hour(), _minute = minute(), _second = second();

  blinkTime = true; //!blinkTime;

  if (_hour > 9)
    textIn += String(_hour);
  else
    textIn += "0" + String(_hour);

  if (blinkTime)
    textIn += ":";
  else
    textIn += " ";

  if (_minute > 9)
    textIn += String(_minute);
  else
    textIn += "0" + String(_minute);

  if (blinkTime)
    textIn += ":";
  else
    textIn += " ";

  if (_second > 9)
    textIn += String(_second);
  else
    textIn += "0" + String(_second);

  textIn += " ";
  switch (vBarState)
  {
  case 0:
    textIn += String("j");
    vBarState++;
    break;
  case 1:
    textIn += String("k");
    vBarState++;
    break;
  case 2:
    textIn += String("l");
    vBarState++;
    break;
  case 3:
    textIn += String("m");
    vBarState++;
    break;
  case 4:
    textIn += String("n");
    vBarState++;
    break;
  case 5:
    textIn += String("o");
    vBarState++;
    break;
  case 6:
    textIn += String("p");
    vBarState++;
    break;
  case 7:
    textIn += String("q");
    vBarState++;
    break;
  case 8:
    textIn += String("r");
    vBarState++;
    break;
  case 9:
    textIn += String("s");
    vBarState++;
    break;
  case 10:
    textIn += String("t");
    vBarState++;
    break;
  case 11:
    textIn += String("u");
    vBarState++;
    break;
  case 12:
    textIn += String("v");
    vBarState++;
    break;
  case 13:
    textIn += String("w");
    vBarState++;
    break;
  case 14:
    textIn += String("x");
    vBarState++;
    break;
  case 15:
    textIn += String("y");
    vBarState++;
    break;
  case 16:
    textIn += String("z");
    vBarState++;
    break;
  default:
    textIn += String("{");
    vBarState = 0;
    break;
  }

  for (int i = 0; i < 30; i++)
    msg_time[i] = textIn[i];
}
void buildMsg_custom(String *msgFromApp)
{
  String textIn = "  " + *msgFromApp + "  ";
  for (int i = 0; i < 128; i++)
    msg_custom[i] = textIn[i];
}
void buildMsg_DateAndDHT()
{
  readDHT();

  String textIn = "";
  int _day = day(), _month = month();

  if (_day > 9)
    textIn += String(_day);
  else
    textIn += "0" + String(_day);

  textIn += "/";

  if (_month > 9)
    textIn += String(_month);
  else
    textIn += "0" + String(_month);

  textIn += "/" + String(year());

  textIn += "   ";

  if (temperatureDHT > 0)
    textIn += "+";
  else if (temperatureDHT < 0)
    textIn += "-";

  textIn +=
      String(temperatureDHT).substring(0, -1) + "$   " + String(humidityDHT).substring(0, -1) + "%";

  for (int i = 0; i < 50; i++)
    msg_Date_and_DHT[i] = textIn[i];
}

// < DHT CODE > //
void readDHT()
{
  temperatureDHT = dht.readTemperature();
  humidityDHT = dht.readHumidity();

  if (isnan(temperatureDHT) || isnan(humidityDHT))
  {
    temperatureDHT = 0;
    humidityDHT = 0;
  }
}

// < EEPROM CODE > //
void saveMsg(String *msg)
{
  char data[128];
  (*msg).toCharArray(data, 128);
  EEPROM.put(0, data);
  EEPROM.commit();
}
String recoverMsg()
{
  char data[128];
  EEPROM.get(0, data);
  return String(data);
}

// < MORE /> //