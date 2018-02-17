#pragma once
#include <allegro5\allegro.h>

class Armor
{
public:
	ALLEGRO_BITMAP *m_armor_bitmap;
	int m_armor_value;
	int m_class_ID;

	Armor();
	Armor(ALLEGRO_BITMAP  *armor_bitmap, int armor_value, int class_ID);

	~Armor();
};

