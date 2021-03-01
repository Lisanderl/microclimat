#include <Wire.h>
#include "SparkFunHTU21D.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ActionController.h>
#include "SPIFFS.h"
#include "esp_task_wdt.h"
// #include "esp_camera.h"

#define I2C_SDA 13
#define I2C_SCL 15
#define LED 4

String ssid = "/ssid.txt";
String passPath = "/pass.txt";
String botTokenPath = "/bot_token.txt"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

WiFiClientSecure client;
ActionController *controller;
Timezone tz;

void lightOff()
{
  controller->ledOn = false;
  Serial.print("Off light: ");
  Serial.println(controller->ledOn);
  Serial.println(tz.dateTime());
}

void lightOn()
{

  controller->ledOn = true;
  Serial.print("On light: ");
  Serial.println(controller->ledOn);
  Serial.println(tz.dateTime());
}
void fanOff()
{
  controller->fanOn = false;
  Serial.print("fanOff: ");
  Serial.println(controller->fanOn);
  Serial.println(tz.dateTime());
}

void fanOn()
{

  controller->fanOn = true;
  Serial.print("fanOn: ");
  Serial.println(controller->fanOn);
  Serial.println(tz.dateTime());
}

TaskHandle_t handle1 = NULL;
void vTask1Code(void *pvParameters)
{
  Serial.println("Set wdt");
  //Due to bug with WDT, i set wdt timer to 2 days
  Serial.println(esp_err_to_name(esp_task_wdt_init(3600 * 48, true)));
  for (;;)
  {
    events();
  }
}

void setup()
{

  Serial.begin(115200);
  Serial.println();
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  if (!Wire.begin(I2C_SDA, I2C_SCL))
  {
    Serial.println("An Error has occurred while mounting two wire");
    return;
  }
  WiFi.setTxPower(WIFI_POWER_18_5dBm);
  WiFi.begin("Tenda_1FDCD8", "");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //sinc time
  delay(500);
  waitForSync();
  delay(500);
  tz.setLocation("Europe/Kiev");
  tz.setEvent(lightOff, makeTime(18, 55, 15, 11, OCTOBER, 2020), LOCAL_TIME, 10);
  tz.setEvent(lightOn, makeTime(18, 55, 25, 11, OCTOBER, 2020), LOCAL_TIME, 10);
  tz.setEvent(fanOn, makeTime(18, 55, 35, 11, OCTOBER, 2020), LOCAL_TIME, 10);
  tz.setEvent(fanOff, makeTime(18, 55, 45, 11, OCTOBER, 2020), LOCAL_TIME, 10);
  std::shared_ptr<UniversalTelegramBot> _bot(new UniversalTelegramBot(SPIFFS.open(botTokenPath, FILE_READ).readString(), client));
  std::shared_ptr<HTU21D> _temHymSensor(new HTU21D());
  controller = new ActionController(_bot, _temHymSensor);
  //enother core task
  xTaskCreatePinnedToCore(vTask1Code, "vTaskCode", 4096, NULL, 7, &handle1, 0);
}

void loop()
{
  controller->checkForMessages();
  yield();
}
