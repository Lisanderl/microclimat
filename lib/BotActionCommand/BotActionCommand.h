#include <UniversalTelegramBot.h>
#include "SPIFFS.h"

class BotAction
{
protected:
    String _fileName;
    String _message;
    std::shared_ptr<UniversalTelegramBot> _bot;
    String readFile()
    {

        File f = SPIFFS.open(_fileName, FILE_READ);
        if (!f)
        {
            Serial.println("file open failed");
        }
        String str = f.readString();
        f.close();
        return str;
    }

public:
    BotAction(std::shared_ptr<UniversalTelegramBot> bot)
    {
        this->_bot = bot;
    }

    virtual void execute(const String &chat_id) = 0;
};

class MainMenuCommand : public BotAction
{
public:
    MainMenuCommand(std::shared_ptr<UniversalTelegramBot> bot) : BotAction(bot)
    {
        this->_fileName = "/mainMenu.json";
        this->_message = "Main menu: ";
    }

    void execute(const String &chat_id)
    {
        this->_bot->sendMessageWithReplyKeyboard(chat_id, _message, "", readFile());
    }
};

class AdjMenuCommand : public BotAction
{
public:
    AdjMenuCommand(std::shared_ptr<UniversalTelegramBot> bot) : BotAction(bot)
    {
        this->_fileName = "/adjMenu.json";
        this->_message = "Adj menu: ";
    }

    void execute(const String &chat_id)
    {
        this->_bot->sendMessageWithInlineKeyboard(chat_id, _message, "", readFile());
    }
};

class InfoMenuCommand : public BotAction
{

public:
    InfoMenuCommand(std::shared_ptr<UniversalTelegramBot> bot,
                    std::shared_ptr<HTU21D> temHymSensor) : BotAction(bot)
    {
        this->_fileName = "/info.html";
        this->_temHymSensor = temHymSensor;
    }

    void execute(const String &chat_id)
    {
        auto str = readFile();
        String target = "$";
        str.replace(target + "1", String(_temHymSensor->readTemperature())); //temp
        str.replace(target + "2", String(_temHymSensor->readHumidity()));    //hym
        str.replace(target + "3", "400");                            //co2
        str.replace(target + "4", "Off");                            // light
        this->_bot->sendMessage(chat_id, str, "html");
    }

private:
    std::shared_ptr<HTU21D> _temHymSensor;
};

class PreferencesCommand : public BotAction
{
public:
    PreferencesCommand(std::shared_ptr<UniversalTelegramBot> bot) : BotAction(bot)
    {
        this->_fileName = "/info.html";
    }

    void execute(const String &chat_id)
    {
    }
};
