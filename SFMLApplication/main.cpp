#include <iostream>
#include <list>
#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

const int windowSize = 1000;
const int tileSize = 100;
const int MAX_TILES = windowSize / tileSize;
//std::list<Tile> tiles;
//Transpose of actual graphic
const short mapMap [MAX_TILES][MAX_TILES] = {   {2,1,1,1,1,1,1,1,1,1},
												{0,0,0,0,0,0,0,0,1,0},
												{1,1,1,1,1,1,1,1,1,1},
												{1,0,0,0,0,0,1,0,0,0},
												{1,1,1,1,1,1,1,1,1,1},
												{0,0,1,0,0,0,0,0,1,0},
												{1,1,1,1,1,1,1,1,1,1},
												{1,0,0,0,0,1,0,0,0,1},
												{1,0,0,0,0,1,0,0,0,1},
												{1,1,1,1,1,1,1,1,1,3} };
bool isIn;
double step = tileSize * 0.002, playerSize = (double) (tileSize - 10)/2.0, x = 40, y = 40;
int allowance = (tileSize - playerSize * 2) / 2 * 1 ;
sf::CircleShape player(playerSize);

class Tile {

public:
	sf::RectangleShape shape;
	Tile(int posx, int posy, int sizex, int sizey) {
		shape.setSize(sf::Vector2f(sizex, sizey));
		shape.setFillColor(sf::Color::Yellow);
		shape.setOrigin(0, 0);
		shape.setPosition(posx, posy);
	}
	Tile() = default;

};

class StartTile : public Tile{

public:
	sf::RectangleShape shape;
	StartTile(int posx, int posy) {
		shape.setSize(sf::Vector2f(tileSize, tileSize));
		shape.setFillColor(sf::Color::Green);
		shape.setOrigin(0, 0);
		shape.setPosition(posx, posy);
	}
	StartTile() = default;
	

};

class EndTile : public Tile{

public:
	sf::RectangleShape shape;
	EndTile(int posx, int posy) {
		shape.setSize(sf::Vector2f(tileSize, tileSize));
		shape.setFillColor(sf::Color::White);
		shape.setOrigin(0, 0);
		shape.setPosition(posx, posy);
	}
	EndTile() = default;
};

StartTile stl;
EndTile enl;

bool checkLeft(int x, int y) {
	if (y > 0) {
		return (mapMap[x][y - 1] > 0);
	}
	return false;
}
bool checkRight(int x, int y) {
	if (y < MAX_TILES) {
		return (mapMap[x][y + 1] > 0);
	}
	return false;
}
bool checkUp(int x, int y) {
	if (x > 0) {
		return (mapMap[x - 1][y] > 0);
	}
	return false;
}
bool checkDown(int x, int y) {
	if (x < MAX_TILES) {
		return (mapMap[x + 1][y] > 0);
	}
	return false;
}
bool checkAround(int x, int y) {
	return checkLeft(x, y) || checkRight(x, y) || checkUp(x, y) || checkDown(x, y);
}

std::list<Tile> listOfTile() {
	std::list<Tile> outputTile;
	short first;
	bool isCont = false;
	// traverse Horizontally
	for (short i = 0; i < MAX_TILES; i++) {
		isCont = false;
		for (short j = 0; j < MAX_TILES; j++) {
			switch (mapMap[i][j]) {
			case 1:	if (isCont) {
						if (j == MAX_TILES - 1) {
							outputTile.push_back(Tile(first * tileSize, i * tileSize, tileSize * (j + 1 - first), tileSize));
							isCont = false;
						}
					} else {
						first = j;
						isCont = true;
					} break;

			case 2: if (isCont) {
						outputTile.push_back(Tile(first * tileSize, i * tileSize, tileSize * (j - first), tileSize));
						isCont = false;
					}
					stl = StartTile(j * tileSize, i * tileSize);
					break;
			case 3:	if (isCont) {
						outputTile.push_back(Tile(first * tileSize, i * tileSize, tileSize * (j - first), tileSize));
						isCont = false;
					}
					enl = EndTile(j * tileSize, i * tileSize);
					break;
			default: if (isCont) {
						if (j - first > 1) {
							outputTile.push_back(Tile(first * tileSize, i * tileSize, tileSize * (j - first), tileSize));
						}
						else {
							if (!checkAround(i, j)) {
								outputTile.push_back(Tile(first * tileSize, i * tileSize, tileSize, tileSize));
							}
						}
					}
					isCont = false;
					break;
			}
		}
	}
	
	for (short i = 0; i < MAX_TILES; i++) {
		isCont = false;
		for (short j = 0; j < MAX_TILES; j++) {
			//if it is a tiles
			if (mapMap[j][i] == 1) {
				//if there was a 0 before this tile
				if (isCont) {
					if (j == MAX_TILES - 1) {
						outputTile.push_back(Tile(i * tileSize, first * tileSize, tileSize , tileSize * (j + 1 - first)));
						isCont = false;
					}
				}
				else {
					if (j == MAX_TILES - 1) {

					}
					else {
						first = j;
						isCont = true;
					}
				}
			}
			else {
				if (isCont) {
					if (j - first > 1) {
						outputTile.push_back(Tile(i * tileSize, first * tileSize, tileSize , tileSize * (j - first)));
					}
					else {
						if (!checkAround(i, j)) {
							outputTile.push_back(Tile(i * tileSize, j * tileSize, tileSize, tileSize));
						}
					}
					isCont = false;
				}
			}
		}
	}
	return outputTile;
};

void circleControl() {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && (y > (player.getRadius()) + step) ) {
		y = y - step;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && (x > (player.getRadius()) + step) ) {
		x = x - step;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (y < (windowSize - player.getRadius()))) {
		y = y + step;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && (x < (windowSize - player.getRadius()))) {
		x = x + step;
	}

	return;
}

bool checkIn() {
		short xint, yint, xArrCoord, yArrCoord, upperBound, lowerBound, lengthFromCorner;
	bool hasLeft, hasRight, hasUp, hasDown;

	isIn = false;
	xint = (short)x;
	yint = (short)y;
	xArrCoord = xint / tileSize;
	yArrCoord = yint / tileSize;
	upperBound = (tileSize / 2) + allowance;
	lowerBound = (tileSize / 2) - allowance;
	hasLeft = checkLeft(yArrCoord, xArrCoord);
	hasRight = checkRight(yArrCoord, xArrCoord);
	hasDown = checkDown(yArrCoord, xArrCoord);
	hasUp = checkUp(yArrCoord, xArrCoord);


	if (mapMap[yArrCoord][xArrCoord] > 0) {
		if (hasLeft) {
			if (xint % tileSize <= upperBound && yint % tileSize >= lowerBound && yint % tileSize <= upperBound) {
				isIn = true;
			}
			if (hasUp) {
				lengthFromCorner = (x - xArrCoord * tileSize) * (x - xArrCoord * tileSize) + (y - yArrCoord * tileSize) * (y - yArrCoord * tileSize);
				if (lengthFromCorner >= lowerBound * lowerBound && lengthFromCorner <= upperBound * upperBound) {
					isIn = true;
				}
			}
			if (hasDown) {
				lengthFromCorner = (x - xArrCoord * tileSize) * (x - xArrCoord * tileSize) + (y - (yArrCoord + 1) * tileSize) * (y - (yArrCoord + 1) * tileSize);
				if (lengthFromCorner >= lowerBound * lowerBound && lengthFromCorner <= upperBound * upperBound) {
					isIn = true;
				}
			}
		}if (hasRight) {
			if (xint % tileSize >= lowerBound && yint % tileSize >= lowerBound && yint % tileSize <= upperBound) {
				isIn = true;
			}
			//Corner rounder
			if (hasUp) {
				lengthFromCorner = (x - (xArrCoord + 1) * tileSize) * (x - (xArrCoord + 1) * tileSize) + (y - yArrCoord * tileSize) * (y - yArrCoord * tileSize);
				if (lengthFromCorner >= lowerBound * lowerBound && lengthFromCorner <= upperBound * upperBound) {
					isIn = true;
				}
			}
			if (hasDown) {
				lengthFromCorner = (x - (xArrCoord + 1) * tileSize) * (x - (xArrCoord + 1) * tileSize) + (y - (yArrCoord + 1) * tileSize) * (y - (yArrCoord + 1) * tileSize);
				if (lengthFromCorner >= lowerBound * lowerBound && lengthFromCorner <= upperBound * upperBound) {
					isIn = true;
				}
			}
		}if (hasDown) {
			if (yint % tileSize >= lowerBound && xint % tileSize >= lowerBound && xint % tileSize <= upperBound) {
				isIn = true;
			}
		}if (hasUp) {
			if (yint % tileSize <= upperBound && xint % tileSize >= lowerBound && xint % tileSize <= upperBound) {
				isIn = true;
			}
		}
	}
	return isIn;
}

bool checkWin() {
	if (mapMap[(int)y][(int)x] == 3) {
		short midx = (int) x + tileSize / 2, midy = (int) y + tileSize / 2;
		if (x < midx + allowance && x > midx - allowance && y < midy + allowance && y > midy - allowance) {
			return true;
		}
	}
	return false;
}





int main()
{
	sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Mazy");
	
	Tile protoTile[4];
	Tile smallTile[3];


	player.setFillColor(sf::Color::Blue);
	player.setOrigin(playerSize, playerSize);
	player.setPosition(x, y);

	std::list<Tile> tiles = listOfTile();
	StartTile stl = StartTile(0, 0);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		for (std::list<Tile>::iterator it = tiles.begin(); it != tiles.end(); ++it) {
			window.draw(it->shape);
		}
		window.draw(stl.shape);
		window.draw(enl.shape);
		
		circleControl();

		if (checkIn()) {
			player.setFillColor(sf::Color::Cyan);
		}
		else {
			player.setFillColor(sf::Color::Red);
			//x = stl.shape.getPosition().x + tileSize / 2;
			//y = stl.shape.getPosition().y + tileSize / 2;
		}

		if (checkWin()) {
			enl.shape.setFillColor(sf::Color::Magenta);
		}
		else {
			enl.shape.setFillColor(sf::Color::White);
		}

		player.setPosition(x, y);
		window.draw(player);
		window.display();
	}

	return 0;
}