#ifndef MAIN_H
# define MAIN_H

# include <SFML/Graphics.hpp>
# include <SFML/Audio/Music.hpp>
# include <iostream>
# include <cmath>
# include <vector>
# include <cstdint> // for smaller ints
# include <cstring>
# include <climits>

# define DACC 200.0f

struct wallMovement {
	bool left;
	bool right;
	bool up;
	bool down;
};

struct wallResizable {
	bool left;
	bool right; // here
	bool up;
	bool down;
};

struct cursorMovement {
	bool left;
	bool right;
	bool up;
	bool down;
};

#endif