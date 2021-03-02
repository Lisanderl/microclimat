#include <WiFi.h>
#include <ezTime.h>
#include <analogWrite.h>
// #include "esp_camera.h"

#define LED1 4
#define LED2 2
#define FAN 14

Timezone tz;

void lightOff()
{

  Serial.print("Off light: ");
  Serial.println(tz.dateTime());
  pinMode(LED1, INPUT_PULLUP);
  pinMode(LED2, INPUT_PULLUP);
}

void lightOn()
{

  Serial.print("On light: ");
  Serial.println(tz.dateTime());
  pinMode(LED1, INPUT_PULLDOWN);
  pinMode(LED2, INPUT_PULLDOWN);
}
void fanOff()
{
  analogWrite(FAN, 0);
  Serial.println(tz.dateTime());

}

void fanOn()
{
  analogWrite(FAN, 100);
  Serial.println(tz.dateTime());
}

void setup()
{
  pinMode(FAN, OUTPUT);
  analogWriteResolution(FAN, 12);
  pinMode(LED1, INPUT_PULLDOWN);
  pinMode(LED2, INPUT_PULLDOWN);

  Serial.begin(115200);
  Serial.println();



  while ( WiFi.begin("test", "espif_32") != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
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
  tz.setEvent(lightOff, makeTime(9, 20, 20, 2, MARCH, 2021), LOCAL_TIME, MINUTELY);
  tz.setEvent(lightOn, makeTime(8, 55, 25, 1, MARCH, 2021), LOCAL_TIME, MINUTELY);
  // tz.setEvent(fanOn, makeTime(8, 55, 35, 1, MARCH, 2021), LOCAL_TIME, MINUTELY);
  // tz.setEvent(fanOff, makeTime(8, 55, 45, 1, MARCH, 2021), LOCAL_TIME, MINUTELY);
  Serial.print("Events are setted: ");
  Serial.println(tz.dateTime());
}

void loop()
{
events();
}
