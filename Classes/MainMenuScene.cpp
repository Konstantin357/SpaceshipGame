#include "MainMenuScene.h"
#include "GameUtils.h"

cocos2d::Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
}

bool MainMenu::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    //Background creation
    {
        cocos2d::Sprite* sprite = cocos2d::Sprite::create("Background.jpg");
        if (sprite == nullptr)
        {
            problemLoading("'Background.jpg'");
        }
        else
        {
            sprite->setPosition(cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
            cocos2d::Size backgroundSize = sprite->getContentSize();
            sprite->setScale(visibleSize.width / backgroundSize.width, visibleSize.height / backgroundSize.height);
            addChild(sprite, 0);
        }
    }

    //Menu header creation
    {
        cocos2d::Label* label = cocos2d::Label::createWithTTF("Spaceship game", "fonts/Marker Felt.ttf", 60);
        if (label == nullptr)
        {
            problemLoading("'fonts/Marker Felt.ttf'");
        }
        else
        {
            label->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));
            addChild(label, 1);
        }
    }

    //Menu start game creation
    {
        cocos2d::Label* labelStartGameText = cocos2d::Label::createWithTTF("Start game", "fonts/Marker Felt.ttf", 48);
        cocos2d::MenuItemLabel* labelStartGame = labelStartGameText != nullptr ? cocos2d::MenuItemLabel::create(labelStartGameText, CC_CALLBACK_1(MainMenu::menuStartGameCallback, this)) : nullptr;
        cocos2d::Size labelStartContentSize = labelStartGame != nullptr ? labelStartGame->getContentSize() : cocos2d::Size();
        if (labelStartGame == nullptr || labelStartContentSize.width <= 0 || labelStartContentSize.height <= 0)
        {
            problemLoading("'fonts/Marker Felt.ttf'");
        }
        else
        {
            const cocos2d::Vec2 labelStartGamePosition = cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - labelStartContentSize.height - 250.f);
            labelStartGame->setPosition(labelStartGamePosition);
            if (cocos2d::Menu* labelStartGameMenu = cocos2d::Menu::create(labelStartGame, nullptr))
            {
                labelStartGameMenu->setPosition(cocos2d::Vec2::ZERO);
                addChild(labelStartGameMenu, 1);
            }
        }
    }

    //Menu exit game creation
    {
        cocos2d::Label* labelExitGameText = cocos2d::Label::createWithTTF("Exit game", "fonts/Marker Felt.ttf", 48);
        cocos2d::MenuItemLabel* labelExitGame = labelExitGameText != nullptr ? cocos2d::MenuItemLabel::create(labelExitGameText, CC_CALLBACK_1(MainMenu::menuCloseGameCallback, this)) : nullptr;
        cocos2d::Size labelExitContentSize = labelExitGame != nullptr ? labelExitGame->getContentSize() : cocos2d::Size();
        if (labelExitGame == nullptr || labelExitContentSize.width <= 0 || labelExitContentSize.height <= 0)
        {
            problemLoading("'fonts/Marker Felt.ttf'");
        }
        else
        {
            const cocos2d::Vec2 labelExitGamePosition = cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - labelExitContentSize.height - 320.f);
            labelExitGame->setPosition(labelExitGamePosition);
            if (cocos2d::Menu* labelExitGameMenu = cocos2d::Menu::create(labelExitGame, nullptr))
            {
                labelExitGameMenu->setPosition(cocos2d::Vec2::ZERO);
                addChild(labelExitGameMenu, 1);
            }
        }
    }

    return true;
}

class GameplayScene;
void MainMenu::menuStartGameCallback(cocos2d::Ref* pSender)
{
    GameUtils::GoToGameScene<GameplayScene>(pSender);
}

void MainMenu::menuCloseGameCallback(Ref* pSender)
{
    GameUtils::ExitGame(pSender);
}