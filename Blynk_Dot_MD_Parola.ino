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
#define VIRTUAL_EFFECT V4
#define VIRTUAL_EFFECT_SPEED V5

#define MAX_DEVICES 16                    // Number of modules connected
#define CLK_PIN 14                        // or SCK
#define DATA_PIN 13                       // or MOSI
#define CS_PIN 12                         // or SS
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW // type display
#define SPEED_TIME 25
#define PAUSE_TIME 0

class IconAnimation
{
private:
  unsigned int iconID;
  unsigned int vBarState;
  String icon;

public:
  IconAnimation()
  {
    iconID = 0;
    vBarState = 0;
    icon = "";
  }
  void setIconID(int id) // 0-> hourglass animation 1-> icon fixed
  {
    iconID = id;
  }
  String getNewIcon()
  {
    if (iconID == 0) // Hourglass animation
    {
      switch (vBarState)
      {
      case 0:
        icon = String("j"); // rotate
        vBarState++;
        break;
      case 1: // first decrement
        icon = String("k");
        vBarState++;
        break;
      case 2: // second decrement
        icon = String("l");
        vBarState++;
        break;
      case 3: // third decrement
        icon = String("m");
        vBarState++;
        break;
      case 4:
        icon = String("n");
        vBarState++;
        break;
      case 5:
        icon = String("o");
        vBarState++;
        break;
      case 6:
        icon = String("p");
        vBarState++;
        break;
      case 7:
        icon = String("q");
        vBarState++;
        break;
      case 8:
        icon = String("r");
        vBarState++;
        break;
      case 9:
        icon = String("s");
        vBarState++;
        break;
      case 10:
        icon = String("t");
        vBarState++;
        break;
      case 11:
        icon = String("u");
        vBarState++;
        break;
      case 12:
        icon = String("v");
        vBarState++;
        break;
      case 13:
        icon = String("w");
        vBarState++;
        break;
      case 14:
        icon = String("x");
        vBarState++;
        break;
      case 15:
        icon = String("y");
        vBarState++;
        break;
      case 16:
        icon = String("z");
        vBarState++;
        break;
      default:
        icon = String("{");
        vBarState = 0;
        break;
      }
    }
    else // Fixed icon
    {
      icon = String("W"); //
      vBarState = 0;      // reset the hourglass animation
    }
    return icon;
  }
};

// // // // / // //

// < Global variables >
// Display config
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
MD_MAX72XX::fontType_t *defaultFont;
inline bool isSleepTime()
{ // working from 6h30 to 22h00
  return ((60 * hour() + minute() < 390) || hour() > 22);
}

// Animation display config
unsigned long lastMoved = 0;
unsigned long lastReadingDHT = 0;
unsigned long changeMSG = 0;
unsigned int holdTimeMSG = 0;
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
void changeAnimation(int iconID);
IconAnimation iconAnimation;

// Apply changes when needed
void actDisplayZone1_run();
void actDisplayZone0_run();
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
  }
  Serial.print("Ilu: ");
  Serial.println(ilumination);
}

textPosition_t customMsgEffectPosition = PA_CENTER;
textEffect_t customMsgEffectIn = PA_PRINT;
textEffect_t customMsgEffectOut = PA_SCROLL_LEFT;
uint16_t customMsgEffectSpeed = 25;

BLYNK_WRITE(VIRTUAL_EFFECT_TYPE)
{
  int effect = param.asInt() - 1;

  Serial.print("Eff (In): ");
  switch (effect)
  {
  case 0:
    Serial.println("Left to rigth");
    customMsgEffectPosition = PA_CENTER;
    customMsgEffectIn = PA_PRINT;
    customMsgEffectOut = PA_SCROLL_RIGHT;
    break;
  case 1:
    Serial.println("Right to left");
    customMsgEffectPosition = PA_CENTER;
    customMsgEffectIn = PA_PRINT;
    customMsgEffectOut = PA_SCROLL_LEFT;
    break;
  default: // 2
    customMsgEffectPosition = PA_CENTER;
    customMsgEffectIn = PA_PRINT;
    customMsgEffectOut = PA_SCROLL_LEFT;
    Serial.println("default");
    break;
  }
}
BLYNK_WRITE(VIRTUAL_EFFECT_SPEED)
{
  int speed = param.asInt();
  if (speed <= 100 && speed >= 0)
  {
    int speedValue = map(speed, 0, 100, 0, 250);
    customMsgEffectSpeed = speedValue;
  }
}

// < Blynk functions />

void setup()
{
  Serial.begin(115200);
  delay(10); // To wait for above function to start

  // Initialize the dot display
  Serial.println("Initializing display...");
  for (byte i = 0; i < 4; i++)
  {
    delay(100);
    P.begin(2);

    // Zones Config
    P.setZone(0, 0, 10);
    P.setZone(1, 11, 15);
    P.setIntensity(0, 0);
    P.setIntensity(1, 0);
    P.displayZoneText(0, msg_custom, PA_CENTER, SPEED_TIME, 0, PA_PRINT, PA_SCROLL_LEFT);
    P.displayZoneText(1, msg_time, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);

    defaultFont = P.getFont(0);

    P.setFont(special);
  }

  Serial.println("Initializing another dependences...");
  pinMode(2, OUTPUT);
  pinMode(16, OUTPUT);
  EEPROM.begin(128);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  // Recover custom message from memory
  String msg = recoverMsg();
  msg.toCharArray(msg_custom, 128);

  delay(1000); // to wait display
  Serial.println("Ready");
}

void loop()
{
  Blynk.run();
  readingDHT_run();
  if (!isSleepTime())
  {
    P.displayAnimate();
    actDisplayZone0_run();
    actDisplayZone1_run();
  }
}

// < MORE > //

// RunWithoutDelay CODE
void actDisplayZone1_run()
{
  if (millis() - lastMoved >= 500)
  {
    lastMoved = millis();
    buildMsg_time();
    P.displayReset(1);
  }
}
void actDisplayZone0_run()
{
  if (P.getZoneStatus(0) && (millis() - changeMSG >= holdTimeMSG))
  {
    switch (crrMsg)
    {
    case 0:                       // Show date, humidity and temperature.
      iconAnimation.setIconID(0); // set hourglass animation
      lastMoved = 0;              // reset zone 1
      P.setFont(0, special);
      P.displayZoneText(0, msg_Date_and_DHT, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_SCROLL_DOWN, PA_NO_EFFECT);
      holdTimeMSG = 8500; // animation time more 8 seconds
      crrMsg++;
      break;

    case 1: // Hide date, humidity and temperature.
      P.setFont(0, special);
      P.displayZoneText(0, msg_Date_and_DHT, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_SCROLL_DOWN);
      holdTimeMSG = 0; // ONLY ANIMATION TIME.
      crrMsg++;
      break;

    case 2:                       // Show custom message.
      iconAnimation.setIconID(1); // Change icon to "IF" while show the custom message
      lastMoved = 0;              // reset zone 1
      P.setFont(0, defaultFont);
      P.displayZoneText(0, msg_custom, customMsgEffectPosition, customMsgEffectSpeed, 0, customMsgEffectIn, customMsgEffectOut);
      P.setTextEffect(0, customMsgEffectOut, customMsgEffectOut);
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

  textIn += iconAnimation.getNewIcon();

  for (int i = 0; i < 30; i++)
    msg_time[i] = textIn[i];
}
void buildMsg_custom(String *msgFromApp)
{
  String textIn = " " + *msgFromApp + "  ";
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

  textIn += " ";

  if (temperatureDHT > 0)
    textIn += "+";
  else if (temperatureDHT < 0)
    textIn += "-";

  String temperatureTXT = String(temperatureDHT),
         humidityTXT = String(humidityDHT);

  textIn +=
      temperatureTXT.substring(0, temperatureTXT.length() - 1) + "& " + humidityTXT.substring(0, humidityTXT.length() - 1) + "%";

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