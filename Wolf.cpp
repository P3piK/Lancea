#include "Wolf.h"


Wolf::Wolf()
{
	max_hp = 40;
	cur_hp = max_hp;
	speed = 1.1;
	min_dmg = 5;
	max_dmg = 11;
	attack_speed = 1.5;
	m_name = "wolf";
	exp = 25;
	range = 75;

	imageWidth = 58;
	imageHeight = 58;
}

Wolf::Wolf(float PosX, float PosY, ALLEGRO_BITMAP *Front, ALLEGRO_BITMAP *Back, ALLEGRO_BITMAP *Left, ALLEGRO_BITMAP *Right, ALLEGRO_BITMAP *WalkFront[2], ALLEGRO_BITMAP *WalkBack[2], ALLEGRO_BITMAP *WalkLeft[2], ALLEGRO_BITMAP *WalkRight[2])
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

Wolf::~Wolf()
{
}