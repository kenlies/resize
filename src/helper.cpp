#include "../include/helper.hpp"

int loadSetBackground(sf::Sprite *bgSprite, sf::Texture *bgTexture) {
    if (!bgTexture->loadFromFile("assets/win.png")) {
        return 1;
    }
    bgTexture->setRepeated(true);
    bgSprite->setTexture(*bgTexture);
    bgSprite->setTextureRect(sf::IntRect(0, 0, 2560, 1440));
    return 0;
}

int loadSetCursor(sf::Sprite *cursorSprite, sf::Texture *cursorTexture) {
    if (!cursorTexture->loadFromFile("assets/cursor.png")) {
        return 1;
    }
    cursorSprite->setTexture(*cursorTexture);
    cursorSprite->setOrigin(sf::Vector2f(20, 20));
    return 0;
}

int	loadSetOsCursor(sf::Sprite *osCursorSprite, sf::Texture *osCursorTexture) {
    if (!osCursorTexture->loadFromFile("assets/oscursor.png")) {
        return 1;
    }
    osCursorSprite->setTexture(*osCursorTexture);
    osCursorSprite->setOrigin(sf::Vector2f(12, 12));
    osCursorSprite->scale({1.7, 1.7});
    return 0;
}

int loadSetTrash(sf::Sprite *trashSprite, sf::Texture *trashTexture) {
    if (!trashTexture->loadFromFile("assets/trash.png")) {
        return 1;
    }
    trashSprite->setTexture(*trashTexture);
    trashSprite->setOrigin(sf::Vector2f(20, 20));
    trashSprite->scale({0.7, 0.7});
    sf::FloatRect trashBounds = trashSprite->getLocalBounds();
    trashSprite->setOrigin(trashBounds.width / 2.f, trashBounds.height / 2.f);
    return 0;
}

int loadSetLevelClearText(sf::Text *levelClearText, sf::Font *levelClearFont) {
    if (!levelClearFont->loadFromFile("assets/sixty.ttf")) {
        return 1;
    }
    levelClearText->setFont(*levelClearFont);
    levelClearText->setString("Level Clear!");
    levelClearText->setCharacterSize(32);
    levelClearText->setFillColor(sf::Color::White);
    levelClearText->setStyle(sf::Text::Bold);
    levelClearText->setOutlineColor(sf::Color::Black);
    levelClearText->setOutlineThickness(3.f);
    sf::FloatRect textBounds = levelClearText->getLocalBounds();
    levelClearText->setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    return 0;
}

int	loadSetGameComplete(sf::Text *gameCompleteText, sf::Font *gameCompleteFont) {
    if (!gameCompleteFont->loadFromFile("assets/sixty.ttf")) {
        return 1;
    }
    gameCompleteText->setFont(*gameCompleteFont);
    gameCompleteText->setString("Game Completed!");
    gameCompleteText->setCharacterSize(32);
    gameCompleteText->setFillColor(sf::Color::White);
    gameCompleteText->setStyle(sf::Text::Bold);
    gameCompleteText->setOutlineColor(sf::Color::Black);
    gameCompleteText->setOutlineThickness(3.f);
    sf::FloatRect textBounds = gameCompleteText->getLocalBounds();
    gameCompleteText->setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    return 0;
}

int	loadSetMusic(sf::Music *music) {
    if (!music->openFromFile("assets/blue.ogg")) {
        return 1;
    }
    music->setVolume(10);
    music->setLoop(true);
    return 0;
}

sf::Vector2f fixDiagonalSpeed(sf::Vector2f acceleration, float dAcc) {
    if (acceleration.x != 0.f && acceleration.y != 0.f) {
        float length = std::sqrt(acceleration.x * acceleration.x + acceleration.y * acceleration.y);
        if (length != 0) {
            acceleration /= length;
            acceleration *= dAcc;
        }
    }
    return acceleration;
}
