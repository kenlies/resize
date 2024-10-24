#include "../include/main.hpp"
#include "../include/helper.hpp"

// This is a small game idea I had while reading the sfml tutorials about window management :D
// Most of the code is just stuffed into main just because why not :D
// (hey its only 500 lines anyway)

int main()
{   
    std::string goalString = "800x600";
    sf::Vector2u goalVec({800, 600});

    sf::ContextSettings settings;
    settings.antialiasingLevel = 2;

    sf::RenderWindow window(sf::VideoMode(200, 600), "", sf::Style::Close, settings);
    window.setFramerateLimit(60);
    window.setPosition({50, 150});

    sf::Mouse mouse;
    mouse.setPosition(sf::Vector2i(50 + 200, 150 + 600), window); // initially set the mouse position to be at the center

    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    if (loadSetBackground(&bgSprite, &bgTexture) == 1) {
        std::cout << "Error loading background texture" << '\n';
        return 1;
    }

    sf::Texture cursorTexture;
    sf::Sprite cursorSprite;
    if (loadSetCursor(&cursorSprite, &cursorTexture) == 1) {
        std::cout << "Error loading cursor texture" << '\n';
        return 2;
    }

    sf::Texture osCursorTexture;
    sf::Sprite osCursorSprite;
    if (loadSetOsCursor(&osCursorSprite, &osCursorTexture) == 1) {
        std::cout << "Error loading oscursor texture" << '\n';
        return 3;
    }
    window.setMouseCursorVisible(false);

    sf::Texture trashTexture;
    sf::Sprite trashSprite;
    std::vector<sf::Sprite> trashSpriteVec;
    if (loadSetTrash(&trashSprite, &trashTexture) == 1) {
        std::cout << "Error loading cursor texture" << '\n';
        return 4;
    }

    sf::Font levelClearFont;
    sf::Text levelClearText;
    if (loadSetLevelClearText(&levelClearText, &levelClearFont) == 1) {
        std::cout << "Error loading level clear font" << '\n';
        return 5;
    }

    sf::Text gameCompleteText;
    if (loadSetGameComplete(&gameCompleteText, &levelClearFont) == 1) {
        std::cout << "Error loading level lost font" << '\n';
        return 6;
    }

    sf::Music music;
    if (loadSetMusic(&music) == 1) {
        std::cout << "Error loading music" << '\n';
        return 6;
    }
    music.play();

    sf::Clock frameRateClock;                // track deltatime
    sf::Clock levelTransitionClock;          // track level transition delay
    sf::Clock trashClock;                    // limit the amount of trashbins drawn
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    sf::Vector2u nextLevelSize;
    uint8_t level = 1;
    float levelTransition = 3.0f;
    float x = window.getSize().x / 2;                           // wasd cursor pos
    float y = window.getSize().y / 2;                           // wasd cursor pos
    bool isLevelClearTextVisible = false;
    bool isGameCompleteTextVisible = false;
    bool isCursorVisible = true;
    bool isOsCursorLevel = false;
    bool isOsCursorVisible = false;
    bool levelCheck = true;
    bool levelCleared = false;
    bool levelLost = false;
    bool levelStartAnim = false;
    bool levelEndAnim = false;
    bool running = true;

    wallMovement wallMove;
    wallResizable wallResize;
    cursorMovement curMove;

    memset(&wallMove, 0, sizeof(wallMove));
    memset(&wallResize, 0, sizeof(wallResize));
    memset(&curMove, 0, sizeof(curMove));
    wallResize.right = true;

    while (running)
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            switch (event.type)
            {
                // window closed
                case sf::Event::Closed:
                    running = false;
                    break;

                // key pressed
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Q) {
                        running = false;
                    }
                    else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
                        //moveUp = true;
                        curMove.up = true;
                    }
                    else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
                        //moveDown = true;
                        curMove.down = true;
                    }
                    else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
                        //moveLeft = true;
                        curMove.left = true;
                    }
                    else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
                        //moveRight = true;
                        curMove.right = true;
                    }
                    break;

                // key released
                case sf::Event::KeyReleased:
                    if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
                        //moveUp = false;
                        curMove.up = false;
                    }
                    else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
                        //moveDown = false;
                        curMove.down = false;
                    }
                    else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
                        //moveLeft = false;
                        curMove.left = false;
                    }
                    else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
                        //moveRight = false;
                        curMove.right = false;
                    }
                    break;

                // mouse click
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        std::cout << "the right button was pressed" << std::endl;
                    }
                    else if (event.mouseButton.button == sf::Mouse::Left){
                        std::cout << "the left button was pressed" << std::endl;
                    }
                    break ;
                
                // scroll
                case sf::Event::MouseWheelScrolled:
                    break ;
                
                // mouse move
                case sf::Event::MouseMoved:
                    break ;

                case sf::Event::LostFocus:
                    // game pause
                    break;
                
                case sf::Event::GainedFocus:
                    // game resume
                    break;
                case sf::Event::Resized:
                    window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height))); // keep things the same size inside window
                    break;
                // we don't process other types of events
                default:
                    break;
            }
        }

        // handle window things
        static bool flip = false;
        const float tolerance = 2.f;
        sf::Vector2f playerPos = cursorSprite.getPosition();
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2i windowPos = window.getPosition();
        sf::Vector2i mousePos = mouse.getPosition();

        // check if mouse is outside of window: lose level
        if (isOsCursorVisible) {
            if (mousePos.x <= windowPos.x * 2 || mousePos.x >= windowPos.x * 2 + windowSize.x * 2
                || mousePos.y <= windowPos.y * 2 || mousePos.y >= windowPos.y * 2 + windowSize.y * 2) {
                levelLost = true;
            }
        }

        // lose level
        if (windowSize.x <= 3 || windowSize.y <= 3) {
            levelLost = true;
        }

        // level clear
        if (windowSize.x >= goalVec.x && windowSize.y >= goalVec.y && levelCheck) {
            levelCheck = false;
            levelCleared = true;
            levelTransitionClock.restart();
        }

        if (isCursorVisible) {
            // check if player pushes borders *
            // * touching right
            if (std::abs(playerPos.x - windowSize.x) <= tolerance && curMove.right && wallResize.right) { 
                windowSize.x = windowSize.x + 8;
            }
            // * touching up
            // * somehow the most possible y position for window is 67 (mac), thats what I am checking below
            else if (playerPos.y <= tolerance && curMove.up && wallResize.up && windowPos.y > 67) {
                windowSize.y = windowSize.y + 6;
                windowPos.y -= 6;
            }
            // * touching left
            else if (playerPos.x <= tolerance && curMove.left && wallResize.left) {
                for (uint8_t i = 0; i < 8; i++) {
                    windowSize.x = windowSize.x + 1;
                    window.setSize(sf::Vector2u(windowSize.x, windowSize.y));
                    windowPos.x -= 1;
                    window.setPosition(sf::Vector2i(windowPos.x, windowPos.y));
                }
            }
            // * touching down
            else if (std::abs(playerPos.y - windowSize.y) <= tolerance && curMove.down && wallResize.down) {
                for (uint8_t i = 0; i < 6; i++) {
                    windowSize.y += 1;
                    window.setSize(sf::Vector2u(windowSize.x, windowSize.y));
                }
                window.setPosition(sf::Vector2i(windowPos.x, windowPos.y));
            }
            // set the title only when cursor is visible/we are playing :)
            if (isGameCompleteTextVisible) {
                window.setTitle("Wohoo!");
            } else {
                window.setTitle("Level " + std::to_string(level) + " | goal >= " + goalString + " | curr = " + std::to_string(windowSize.x) + "x" + std::to_string(windowSize.y));
            }
        }

        // level start animation, palyercursor and actual cursor location set in new level
        // and show the game complete text if so
        if (levelStartAnim) {
            uint8_t incX;
            uint8_t incY;
            windowSize.x >= nextLevelSize.x ? incX = 0 : incX = 6;
            windowSize.y >= nextLevelSize.y ? incY = 0 : incY = 4;
            if (flip) {
                window.setSize(sf::Vector2u(windowSize.x + incX, windowSize.y + incY));
                window.setPosition(sf::Vector2i(windowPos.x - incX, windowPos.y + incY / 2));
                flip = false;
            } else {
                window.setSize(sf::Vector2u(windowSize.x + incX, windowSize.y + incY));
                flip = true;
            }
            if (windowSize.x >= nextLevelSize.x && windowSize.y >= nextLevelSize.y) {
                window.setSize(sf::Vector2u(nextLevelSize.x, nextLevelSize.y));
                levelStartAnim = false;
                isCursorVisible = true;
                x = windowSize.x / 2;
                y = windowSize.y / 2;
                if (isOsCursorLevel) {
                    isOsCursorVisible = true;
                }
                mouse.setPosition(sf::Vector2i(windowPos.x + windowSize.x, windowPos.y + windowSize.y), window);
            }
        }
        else
            window.setSize(sf::Vector2u(windowSize.x, windowSize.y));

        // level end animation
        if (levelEndAnim) {
            if (flip) {
                window.setSize(sf::Vector2u(windowSize.x - 8, windowSize.y - 6));
                window.setPosition(sf::Vector2i(windowPos.x + 8, windowPos.y));
                flip = false;
            } else {
                window.setSize(sf::Vector2u(windowSize.x - 8, windowSize.y - 6));
                flip = true;
            }
        }
        
        // move borders
        if ((wallMove.down || wallMove.up || wallMove.right || wallMove.left) 
            && (!levelStartAnim && !levelEndAnim && !levelCleared)) {
            int8_t incX = 0;
            int8_t incY = 0;
            int8_t moveX = 0;
            int8_t moveY = 0;
            if (wallMove.right && wallMove.left && wallMove.up && wallMove.down) {
                incY = -2;
                incX = -2;  
                moveX = 2;
            }
            else if (wallMove.down && wallMove.left && wallMove.up) {
                incY = -1;
                incX = -1;
                moveX = 2;
            }
            else if (wallMove.right && wallMove.left) {
                incX = -2;
                moveX = 2;
            }
            else if (wallMove.up) {
                incY = -1;
                moveY = 1;
            }
            else if (wallMove.down) {
                incY = -1;
                moveY = -1;
            }
            else if (wallMove.right) {
                incX = -1;
            }
            else if (wallMove.left) {
                incX = -1;
                moveX = 2;
            }
            if (flip) {
                window.setSize(sf::Vector2u(windowSize.x + incX, windowSize.y + incY));
                window.setPosition(sf::Vector2i(windowPos.x + moveX, windowPos.y + moveY));
                flip = false;
            } else {
                window.setSize(sf::Vector2u(windowSize.x + incX, windowSize.y + incY));
                flip = true;
            }
        }

        // move player
        acceleration = sf::Vector2f(0.f, 0.f);
        sf::Time ElapsedTime = frameRateClock.restart();; // prevent movement speed dependency on framerate

        if (curMove.up) {
            acceleration.y -= DACC;
        }
        if (curMove.down) {
            acceleration.y += DACC;
        }
        if (curMove.left) {
            acceleration.x -= DACC;
        }
        if (curMove.right) {
            acceleration.x += DACC;
        }
        acceleration = fixDiagonalSpeed(acceleration, DACC);

        velocity += acceleration;
        velocity *= 0.80f; // smoothen the stop/add friction
        x += velocity.x * ElapsedTime.asSeconds();
        y += velocity.y * ElapsedTime.asSeconds();
        if (x > windowSize.x)
            x = windowSize.x;
        else if (x < 0)
            x = 0;
        else if (y > windowSize.y)
            y = windowSize.y;
        else if (y < 0)
            y = 0;
        cursorSprite.setPosition(static_cast<int>(x), static_cast<int>(y));

        // change level
        if (levelCleared || levelLost) {
            isCursorVisible = false;
            isOsCursorVisible = false;
            if (levelCleared) {
                isLevelClearTextVisible = true;
            } else {
                level = 0;
            }

            if (isLevelClearTextVisible) {
                levelClearText.setPosition({windowSize.x / 2.f, windowSize.y / 2.f});
            }

            if (levelTransitionClock.getElapsedTime().asSeconds() >= levelTransition / 2.4) {
                levelEndAnim = true; // play the level ending animation
                window.setTitle("");
                trashSpriteVec.clear();
            }
            if (levelTransitionClock.getElapsedTime().asSeconds() >= levelTransition) {
                level++;
                memset(&wallMove, 0, sizeof(wallMove));
                memset(&wallResize, 0, sizeof(wallResize));
                window.setSize({5, 5});
                isOsCursorLevel = false;
                isLevelClearTextVisible = false;
                // levels
                switch (level) {
                    case 1:
                        isGameCompleteTextVisible = false;
                        levelTransition = 3.0;
                        goalString = "800x600";
                        goalVec = {800, 600};
                        window.setPosition({200, 300});
                        nextLevelSize = {200, 600};
                        wallResize.right = true;
                        break;

                    case 2:
                        window.setPosition({600, 200});
                        nextLevelSize = {800, 200};
                        wallResize.down = true;
                        break;

                    case 3:
                        window.setPosition({800, 700});
                        nextLevelSize = {800, 300};
                        wallResize.up = true;
                        wallMove.down = true;
                        break;

                    case 4:
                        levelTransition = 3.1;
                        goalString = "832x624";
                        goalVec = {832, 624};
                        window.setPosition({1100, 200});
                        nextLevelSize = {400, 300};
                        wallResize.down = true;
                        wallResize.left = true;
                        wallMove.right = true;
                        break;

                    case 5:
                        window.setPosition({250, 600});
                        nextLevelSize = {400, 400};
                        wallResize.up = true;
                        wallResize.right = true;
                        wallMove.left = true;
                        break;

                    case 6:
                        window.setPosition({735, 150});
                        nextLevelSize = {1200, 200};
                        wallResize.down = true;
                        wallMove.left = true;
                        wallMove.right = true;
                        break;
                    
                    case 7:
                        levelTransition = 3.6;
                        goalString = "960x720";
                        goalVec = {960, 720};
                        window.setPosition({735, 440});
                        nextLevelSize = {150, 150};
                        wallResize.up = true;
                        wallResize.down = true;
                        wallResize.right = true;
                        wallResize.left = true;
                        isOsCursorLevel = true;
                        break;

                    case 8:
                        window.setPosition({735, 700});
                        nextLevelSize = {960, 300};
                        wallResize.up = true;
                        wallMove.up = true;
                        isOsCursorLevel = true;
                        break;

                    case 9:
                        window.setPosition({1150, 300});
                        nextLevelSize = {600, 460};
                        wallResize.up = true;
                        wallResize.down = true;
                        wallResize.left = true;
                        wallMove.up = true;
                        wallMove.down = true;
                        wallMove.left = true;
                        isOsCursorLevel = true;
                        break;
                    
                    case 10:
                        levelTransition = 3.8;
                        goalString = "1024x768";
                        goalVec = {1024, 768};
                        window.setPosition({735, 450});
                        nextLevelSize = {512, 384};
                        wallResize.up = true;
                        wallResize.down = true;
                        wallResize.right = true;
                        wallResize.left = true;
                        wallMove.up = true;
                        wallMove.down = true;
                        wallMove.right = true;
                        wallMove.left = true;
                        isOsCursorLevel = true;
                        break;
                    
                    case 11: // game complete
                        isGameCompleteTextVisible = true;
                        window.setPosition({735, 440});
                        goalVec = {INT_MAX, INT_MAX};
                        nextLevelSize = {512, 384};
                        wallResize.up = true;
                        wallResize.down = true;
                        wallResize.right = true;
                        wallResize.left = true;
                        isOsCursorLevel = true;
                        break;
                }
                levelEndAnim = false;
                levelCleared = false;
                levelLost = false;
                levelCheck = true;
                levelStartAnim = true;
            }
        }

        // check if trash is to be placed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && trashClock.getElapsedTime().asMilliseconds() > 70 && isCursorVisible) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            trashSprite.setPosition(mousePos.x, mousePos.y);
            trashSpriteVec.push_back(trashSprite);
            trashClock.restart();
        }

        // keep the game complete in the middle
        if (isGameCompleteTextVisible) {
            gameCompleteText.setPosition({windowSize.x / 2.f, windowSize.y / 2.f});
        }

        osCursorSprite.setPosition(static_cast<sf::Vector2f>(mouse.getPosition(window)));

        window.clear();

        //draw everyhing here
        window.draw(bgSprite);

        if (isLevelClearTextVisible) {
            window.draw(levelClearText);
        }

        if (isGameCompleteTextVisible) {
            window.draw(gameCompleteText);
        }

        if (isOsCursorVisible) {
            for (sf::Sprite trashSprite : trashSpriteVec) {
                window.draw(trashSprite);
            }
            window.draw(osCursorSprite);
        }

        if (isCursorVisible) {
            window.draw(cursorSprite);
        }

        window.display();
    }

    // somewhat more graceful music stop when immidiately quitting game
    music.setVolume(0);
    sf::sleep(sf::seconds(0.4f));
    music.stop();
    // release resources

    return 0;
}
