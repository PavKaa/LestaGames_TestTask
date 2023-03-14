#include <SFML/Graphics.hpp>
#include "Block.h"
#include <iostream>

using namespace sf;

Block blocks[28];

//Доска для блоков. Пример, позже сделаю рандом
int Board[5][5]
{
	3, 4, 2, 4, 1,
	3, 0, 2, 0, 1,
	3, 4, 2, 4, 1,
	3, 0, 1, 0, 2,
	3, 4, 2, 4, 1
};

//Стартовое заполнение доски
void fillBoard()
{
	int n = 0;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			if (!(Board[i][j]))
			{
				blocks[n].setType(Board[i][j]);
			}
			else if(Board[i][j] == 4)
			{
				blocks[n].setSprite(150, 0, 65, 60);
				blocks[n].setPos(65 * j + 188, 60 * i + 105);
				blocks[n].setType(Board[i][j]);
			}
			else
			{
				blocks[n].setSprite(50 * (Board[i][j] - 1), 0, 50, 45);
				blocks[n].setPos(65 * j + 196, 60 * i + 113);
				blocks[n].setType(Board[i][j]);
			}
			n++;
		}

	for (int i = 25; i < 28; i++)
	{
		blocks[i].setSprite(50 * (i - 25), 0, 50, 45);
		blocks[i].setPos(130 * (27 - i) + 196, 45);
		blocks[i].setType(28 - i);
	}
}

//Перемещение рамки
void frameMove(int offsetX, int offsetY, Sprite & frame) 
{
	Vector2f posFrame = frame.getPosition();
	if (posFrame.x + offsetX < 188 || posFrame.x + offsetX > 512) return;
	if (posFrame.y + offsetY < 105 || posFrame.y + offsetY > 404) return;

	for (int i = 0; i < 25; i++)
		if (blocks[i].getSprite().getGlobalBounds().contains(posFrame.x + offsetX, posFrame.y + offsetY) && blocks[i].getType() == 4)
		{
			frameMove(offsetX * 2, offsetY * 2, frame);
			return;
		}
	
	frame.move(offsetX, offsetY);
}

//Перемещение блоков, даже если придется премещать вместе с этим соседний блок
bool possibleBlockMove(int offsetX, int offsetY, int index)
{
	Vector2f posBlock = blocks[index].getSprite().getPosition();
	if (posBlock.x + offsetX < 188 || posBlock.x + offsetX > 512) return false;
	if (posBlock.y + offsetY < 105 || posBlock.y + offsetY > 404) return false;

	for (int i = 0; i < 25; i++)
		if (blocks[i].getSprite().getGlobalBounds().contains(posBlock.x + offsetX, posBlock.y + offsetY) && blocks[i].getType() == 4)
			return false;

	return true;
}

//Поиск блока по заданным координатам
int searchBlock(int posX, int posY)
{
	for (int i = 0; i < 25; i++)
		if (blocks[i].getSprite().getPosition() == Vector2f(posX, posY))
			return i;

	return -1;
}

//Проверка победы
bool checkWin()
{
	for (int i = 0; i < 25; i++)
	{
		if (blocks[i].getType() == 0 || blocks[i].getType() == 4) continue;

		int index = searchBlock(65 * (i % 5) + 196, 60 * (i / 5) + 113);

		if (index == -1) return false;
		if (blocks[index].getType() != ((i + i / 5 + 2) % 3 + 1)) 
			return false;

	}

	return true;
}

//Напоминание!
//Сделать выбор блоков путем перемещения рамки на стрелки или WASD
//Сделать рамку постоянно моргающей, если выбран блок

int main()
{
	RenderWindow window(VideoMode(700, 473), "Test task");

	//Переменные
	bool canMove = false,  animated = false, choosed = false, isWon = false;
	float dx, dy, blockOffsetX, blockOffsetY;
	int n = -1, stepAnim = -1, index;
	Vector2f oldPos;
	Vector2i newPos;
	Clock clock, clockAnim;
	float frameTimer = 0;
	Color colorFrame(0, 0, 0, 255);

	//Текстуры
	Texture textureBackGround, textureBlocks, textureFrame, textureVictoryScreen;
	textureBackGround.loadFromFile("Images/BackGroungWithDesk.jpg");
	textureBlocks.loadFromFile("Images/Colors.png");
	textureFrame.loadFromFile("Images/frame.png");
	textureVictoryScreen.loadFromFile("Images/BackGroungWin.jpg");
	
	//Спрайты
	Sprite backGround, frame, victoryScreen;
	backGround.setTexture(textureBackGround);
	victoryScreen.setTexture(textureVictoryScreen);
	frame.setTexture(textureFrame);
	frame.setPosition(196, 113);

	//Присвоение текстур блокам
	for (int i = 0; i < 28; i++)
		blocks[i].setTexture(textureBlocks);
	fillBoard();

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		Vector2f posFrame = frame.getPosition();
		Vector2i mousePos = Mouse::getPosition(window);
		
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();

			//Выбор блока для перемещения
			if (event.type == Event::KeyPressed && !isWon)
				if (event.key.code == Keyboard::Enter)
				{
					int i = searchBlock(posFrame.x, posFrame.y);
					if(i != -1)
						if (blocks[i].getType() != 0)
						{
							Color colorFrame(0, 0, 0, 255);
							frame.setColor(colorFrame);
							animated = true;
							choosed = true;
						}
				}

			//Отмена выбора блока для премещения
			if (event.type == Event::KeyPressed && !isWon)
				if (event.key.code == Keyboard::Escape)
				{
					colorFrame.a = 255;
					frame.setColor(colorFrame);
					animated = false;
					choosed = false;
				}
		}

		frameTimer += time;

		//Мигание рамки во время выбора блока
		if (clockAnim.getElapsedTime().asMicroseconds() > 1500 && animated && !isWon)
		{
			clockAnim.restart();
			colorFrame.a += stepAnim;
			frame.setColor(colorFrame);
			if (colorFrame.a >= 255) stepAnim = -1;
			if (colorFrame.a <= 50) stepAnim = 1;
		}
		
		//Перемещение рамки выбора c задержкой в полсекунды
		if (frameTimer > 500 && !choosed && !isWon)
		{
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				frameMove(0, -60, frame);
				frameTimer = 0;
			}

			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				frameMove(0, 60, frame);
				frameTimer = 0;
			}

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{ 
				frameMove(-65, 0, frame);
				frameTimer = 0;
			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				frameMove(65, 0, frame);
				frameTimer = 0;
			}
		}

		//Перемещение выбранного блока
		if (choosed && !isWon)
		{
			for (index = 0; index < 25; index++)
				if (blocks[index].getSprite().getPosition() == posFrame)
					break;

			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				if (possibleBlockMove(0, -60, index))
				{
					canMove = true;
					blockOffsetX = 0;
					blockOffsetY = -60;
				}
				else
				{
					colorFrame.a = 255;
					frame.setColor(colorFrame);
					animated = false;
					choosed = false;
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				if(possibleBlockMove(0, 60, index))
				{
					canMove = true;
					blockOffsetX = 0;
					blockOffsetY = 60;
				}
				else
				{
					colorFrame.a = 255;
					frame.setColor(colorFrame);
					animated = false;
					choosed = false;
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				if(possibleBlockMove(-65, 0, index))
				{
					canMove = true;
					blockOffsetX = -65;
					blockOffsetY = 0;
				}
				else
				{
					colorFrame.a = 255;
					frame.setColor(colorFrame);
					animated = false;
					choosed = false;
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				if(possibleBlockMove(65, 0, index))
				{
					canMove = true;
					blockOffsetX = 65;
					blockOffsetY = 0;
				}
				else
				{
					colorFrame.a = 255;
					frame.setColor(colorFrame);
					animated = false;
					choosed = false;
				}
			}
		}

		//Отрисовка c анимацией
		if (canMove && !isWon)
		{
			Vector2f oldPos = blocks[index].getSprite().getPosition();
			int anotherBlock = searchBlock(oldPos.x + blockOffsetX, oldPos.y + blockOffsetY);

			if (anotherBlock == -1)
			{
				for (int i = 0; i < 50; i++)
				{
					blocks[index].getSprite().move(blockOffsetX / 50, blockOffsetY / 50);
					window.draw(blocks[index].getSprite());
				}

				blocks[index].getSprite().setPosition(oldPos.x + blockOffsetX, oldPos.y + blockOffsetY);
				frameMove(blockOffsetX, blockOffsetY, frame);
				colorFrame.a = 255;
				frame.setColor(colorFrame);
				canMove = false;
				animated = false;
				choosed = false;

				//Механика со сменой двух соседних блоков при ходе
				/*for (int i = 0; i < 50; i++)
				{
					blocks[index].getSprite().move(blockOffsetX / 50, blockOffsetY / 50);
					blocks[anotherBlock].getSprite().move(-blockOffsetX / 50, -blockOffsetY / 50);
					window.draw(blocks[index].getSprite());
					window.draw(blocks[anotherBlock].getSprite());
				}

				blocks[index].getSprite().setPosition(oldPos.x + blockOffsetX, oldPos.y + blockOffsetY);
				blocks[anotherBlock].getSprite().setPosition(oldPos);
				frameMove(blockOffsetX, blockOffsetY, frame);
				colorFrame.a = 255;
				frame.setColor(colorFrame);
				canMove = false;
				animated = false;
				choosed = false;*/
			}
			else 
			{
				colorFrame.a = 255;
				frame.setColor(colorFrame);
				canMove = false;
				animated = false;
				choosed = false;
			}	
		}
		
		if (!isWon)
		{
			window.draw(backGround);

			for (int i = 0; i < 28; i++)
				if (blocks[i].getType())
					window.draw(blocks[i].getSprite());
			window.draw(frame);
			window.display();
		}
		
		if (checkWin())
		{
			isWon = true;
			window.clear();
			window.draw(victoryScreen);
			window.display();
		}
	}

	return 0;
}