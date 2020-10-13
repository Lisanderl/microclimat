#include <UniversalTelegramBot.h>
#include <BotActionCommand.h>
#include <EEPROM.h>
#include <ezTime.h>

class ActionController
{

public:
    ActionController(std::shared_ptr<UniversalTelegramBot> bot,
                     std::shared_ptr<HTU21D> temHymSensor,
                     std::shared_ptr<Timezone> zonedTime,
                     int bot_mtbs = 700)
    {
        this->_bot = bot;
        this->_temHymSensor = temHymSensor;
        this->_temHymSensor->begin();
        this->_temHymSensor->setResolution(0b11);
        this->_bot_mtbs = bot_mtbs;
        this->_zonedTime = zonedTime;
        _bot_lasttime = 0;
        //settings
        waitForSync();
        this->_zonedTime->setLocation("Europe/Kiev");
        this->_zonedTime->setEvent(oturnOnLights, makeTime(23, 32, 55, 12, OCTOBER, 2020));
        this->_zonedTime->setEvent(oturnOffLights, makeTime(23, 32, 30, 12, OCTOBER, 2020));
    }

    void checkForMessages()
    {
        if (millis() > _bot_lasttime + _bot_mtbs)
        {
            int numNewMessages = _bot->getUpdates(_bot->last_message_received + 1);

            while (numNewMessages)
            {
                Serial.println("got response");
                handleNewMessages(numNewMessages);
                numNewMessages = _bot->getUpdates(_bot->last_message_received + 1);
            }

            _bot_lasttime = millis();
        }
    }

    void checkForActions()
    {
        // if((millis() - actionLastTime) > TEN_MIN){
        //     actionLastTime = millis();
        // }

        events();
        delay(5000);

        Serial.println("RSS:         " + dateTime(_zonedTime->now(), RSS));
     // Serial.println("Some:         " + this->_zonedTime->tzTime(makeTime(23, 18, 0, MONDAY, OCTOBER, 2020)));
        Serial.println("Some2:         " + dateTime(makeTime(23, 18, 0, TUESDAY, OCTOBER, 2020)));
        Serial.println();
    }

    static void oturnOnLights()
    {
        Serial.println("Light is on");
    }

    static void oturnOffLights()
    {
        Serial.println("Light is off");
    }

    void oturnOnFan()
    {
    }

    void oturnOffFan()
    {
    }

private:
    std::shared_ptr<UniversalTelegramBot> _bot;
    std::shared_ptr<HTU21D> _temHymSensor;
    std::shared_ptr<Timezone> _zonedTime;
    int _bot_mtbs;
    long _bot_lasttime;
    unsigned long actionLastTime = 0;
    const unsigned long TEN_MIN = 600000;

    void selectAction(BotAction *ba, String chat_id)
    {
        ba->execute(chat_id);
    }
    void handleNewMessages(int numNewMessages)
    {
        for (int i = 0; i < numNewMessages; i++)
        {
            Serial.println(_bot->messages[i].type);
            if (_bot->messages[i].type == "callback_query")
            {
                Serial.print("Call back button pressed by: ");
                Serial.println(_bot->messages[i].from_id);
                Serial.print("Data on the button: ");
                Serial.println(_bot->messages[i].text);
                return;
            }

            if (_bot->messages[i].type == "message" && _bot->messages[i].text.charAt(0) == '/')
            {
                handleMenuAction(i);
                return;
            }

            Serial.println("main menu:");
            selectAction(new MainMenuCommand(_bot), _bot->messages[i].chat_id);
        }
    }
    void handleMenuAction(int i)
    {
        if (_bot->messages[i].text.equals("/Preferences"))
        {
            Serial.print("adj action");
            selectAction(new AdjMenuCommand(_bot), _bot->messages[i].chat_id);
            return;
        }

        if (_bot->messages[i].text.equals("/Info"))
        {
            Serial.print("info action");
            selectAction(new InfoMenuCommand(_bot, _temHymSensor), _bot->messages[i].chat_id);
            return;
        }
    }

    void handlePrefAction()
    {
    }
};
