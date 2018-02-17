#include "Snake.h"

Snake::Snake()
{
	max_hp = 32;
	cur_hp = max_hp;
	speed = 1.4;
	min_dmg = 3;
	max_dmg = 7;
	attack_speed = 1.0;
	m_name = "snake";
	exp = 16;
	range = 60;

	imageWidth = 33;
	imageHeight = 33;
}

Snake::Snake(float PosX, float PosY, ALLEGRO_BITMAP *Front, ALLEGRO_BITMAP *Back, ALLEGRO_BITMAP *Left, ALLEGRO_BITMAP *Right, ALLEGRO_BITMAP *WalkFront[2], ALLEGRO_BITMAP *WalkBack[2], ALLEGRO_BITMAP *WalkLeft[2], ALLEGRO_BITMAP *WalkRight[2])
{
	m_PosX = PosX;
	m_PosY = PosY;
	m_Front = Front;
	m_Back = Back;
	m_Left = Left;
	m_Right = Right;
	m_WalkFront[2] = WalkFront[2];
	m_WalkBack[2] = WalkBack[2];
	m_WalkLeft[2] = WalkLeft[2];
	m_WalkRight[2] = WalkRight[2];
}

Snake::~Snake()
{
}
