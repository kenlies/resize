#ifndef HELPER_H
# define HELPER_H

# include <SFML/Graphics.hpp>
# include <SFML/Audio/Music.hpp>
# include <complex> // for std::sqrt()

int				loadSetBackground(sf::Sprite *bgSprite, sf::Texture *bgTexture);
int				loadSetCursor(sf::Sprite *cursorSprite, sf::Texture *cursorTexture);
int				loadSetOsCursor(sf::Sprite *osCursorSprite, sf::Texture *osCursorTexture);
int				loadSetTrash(sf::Sprite *trashSprite, sf::Texture *trashTexture);
int				loadSetLevelClearText(sf::Text *levelClearText, sf::Font *levelClearFont);
int				loadSetGameComplete(sf::Text *gameCompleteText, sf::Font *gameCompleteFont);
int				loadSetMusic(sf::Music *music);
sf::Vector2f	fixDiagonalSpeed(sf::Vector2f acceleration, float dAcc);

#endif
