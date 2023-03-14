#include "Block.h"

void Block::setType(int type) { this->type = type; }

int Block::getType() { return type; }

void Block::setTexture(Texture& texture) { sprite.setTexture(texture); }

void Block::setSprite(int posX, int posY, int width, int hight) 
{ 
	sprite.setTextureRect(IntRect(posX, posY, width, hight)); 
}

Sprite & Block::getSprite() { return sprite; }

void Block::setPos(int posX, int posY) 
{ 
	position.x = posX;
	position.y = posY;
	sprite.setPosition(position);
}

Vector2f Block::getPos() { return position; }