#pragma once
#include "Monster.h"

class Wolf :
	public Monster
{
public:

	Wolf();
	Wolf(float m_PosX, float m_PosY, ALLEGRO_BITMAP *Front, ALLEGRO_BITMAP *Back, ALLEGRO_BITMAP *Left, ALLEGRO_BITMAP *Right,
		ALLEGRO_BITMAP *WalkFront[2], ALLEGRO_BITMAP *WalkBack[2], ALLEGRO_BITMAP *WalkLeft[2], ALLEGRO_BITMAP *WalkRight[2]);
	~Wolf();
};

