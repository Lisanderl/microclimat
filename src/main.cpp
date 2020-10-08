#include <Wire.h>
#include "SparkFunHTU21D.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ActionController.h>
#include "SPIFFS.h"

#define I2C_SDA 13
#define I2C_SCL 15
#define LED 4

char ssid[] = "ASUS";
String passPath = "/pass.txt";
String botTokenPath = "/bot_token.txt"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

WiFiClientSecure client;
ActionController *controller;

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

  WiFi.begin(ssid, SPIFFS.open(passPath, FILE_READ).readString().c_str());

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  std::shared_ptr<UniversalTelegramBot> _bot(new UniversalTelegramBot(SPIFFS.open(botTokenPath, FILE_READ).readString(), client));
  std::shared_ptr<HTU21D> _temHymSensor(new HTU21D());
  controller = new ActionController(_bot, _temHymSensor, 500);
}

void loop()
{
  controller->checkForMessages();
}
