#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Block
{
	int type;
	Sprite sprite;
	Vector2f position;


public:
	void setType(int);
	int getType();
	void setTexture(Texture&);
	void setSprite(int, int, int, int);
	Sprite & getSprite();
	void setPos(int, int);
	Vector2f getPos();
};