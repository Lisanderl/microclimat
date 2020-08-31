#include <Wire.h>
#include "SparkFunHTU21D.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <BotActionCommand.h>
#include "SPIFFS.h"

#define I2C_SDA 13
#define I2C_SCL 15
#define LED 4

char ssid[] = "";
char pass[] = "";
String BOTtoken = ""; 

HTU21D htu;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
BotAction *mainMenu = new MainMenuCommand(bot);

int Bot_mtbs = 1000; 
long Bot_lasttime; 

void selectAction(BotAction *ba, String chat_id)
{
  ba->execute(chat_id);
}

void handleNewMessages(int numNewMessages)
{
  for (int i = 0; i < numNewMessages; i++)
  {

    // Inline buttons with callbacks when pressed will raise a callback_query message
    if (bot.messages[i].type == "callback_query")
    {
      Serial.print("Call back button pressed by: ");
      Serial.println(bot.messages[i].from_id);
      Serial.print("Data on the button: ");
      Serial.println(bot.messages[i].text);
      bot.sendMessage(bot.messages[i].from_id, bot.messages[i].text, "");
    }
    else
    {
      String chat_id = String(bot.messages[i].chat_id);
      String text = bot.messages[i].text;
      selectAction(mainMenu, chat_id);
    }
  }
}

void setup()
{

  Serial.begin(115200);
  Serial.println();

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (millis() > Bot_lasttime + Bot_mtbs)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}
