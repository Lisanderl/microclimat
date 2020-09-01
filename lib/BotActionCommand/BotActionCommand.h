#include <UniversalTelegramBot.h>
#include <SimpleMap.h>
#include "SPIFFS.h"

class BotAction
{
protected:
    String _fileName;
    String _message;
    UniversalTelegramBot *_bot;
    String readFile()
    {

        File f = SPIFFS.open(_fileName, FILE_READ);
        if (!f)
        {
            Serial.println("file open failed");
        }
        return f.readString();
    }

public:
    BotAction(UniversalTelegramBot *bot)
    {
        this->_bot = bot;
    }
    void execute(const String &chat_id)
    {
        this->_bot->sendMessageWithInlineKeyboard(chat_id, _message, "", readFile());
    }
};

class MainMenuCommand : public BotAction
{
public:
    MainMenuCommand(UniversalTelegramBot *bot) : BotAction(bot)
    {
        this->_fileName = "/mainMenu.json";
        this->_message = "Manin menu: ";
    }
};

class BotActionFlyweight
{
private:
    SimpleMap<String, BotAction *> *_myMap;
    const String actions[6] = {"/mainMenu", "/adj", "/fan", "/led", "/temp", "/hym"};

public:
    BotActionFlyweight(String *menuActionName)
    {

        _myMap = new SimpleMap<String, BotAction *>([](String &a, String &b) -> int {
            if (a == b)
                return 0;

            if (a > b)
                return 1;

            /*if (a < b) */ return -1;
        });
        // for (int x = 0; x < (sizeof(menuActionName) / sizeof(menuActionName[0])); x++)
        // {
        //     _myMap->put(menuActionName[x], nullptr);
        // }
    }
    ~BotActionFlyweight()
    {
        _myMap->clear();
        _myMap = nullptr;
    }

    BotAction *getBotAction(const String &str, UniversalTelegramBot *bot)
    {
        if (_myMap->has(str))
        {
            return _myMap->get(str);
        }
    }

private:
    BotAction *createBotAction(const String &str, UniversalTelegramBot *bot)
    {

        switch (_myMap->getIndex(str))
        {
        case 0:
            return new MainMenuCommand(bot);
            break;

        default:
            break;
        }
    }
};
