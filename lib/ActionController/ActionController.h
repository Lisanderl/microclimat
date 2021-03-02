#include <UniversalTelegramBot.h>
#include <BotActionCommand.h>
#include <EEPROM.h>
#include <ezTime.h>

class ActionController
{

public:
    boolean fanOn = false;
    boolean ledOn = false;
    ActionController(u8_t fanPin = 12, u8_t ledPin1 = 14, u8_t ledPin2 = 14)
    {
        this->_fanPin = fanPin;
        this->_ledPin1 = ledPin1;
        this->_ledPin2 = ledPin2;
        //settings
    }

    void turnFanOn()
    {
    }

    void turnFanOff()
    {
    }

    void turnLedOn()
    {
    }

    void turnLedOff()
    {
    }

private:
    unsigned long actionLastTime = 0;
    u8_t _fanPin;
    u8_t _ledPin1;
    u8_t _ledPin2;
};