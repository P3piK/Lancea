#pragma once
#include <allegro5\allegro.h>
#include <random>

class Weapon
{
public:
	int m_min_damage;
	int m_max_damage;
	double m_speed;
	int m_dps();
	
	int m_classID;
	int calculate_attack();

	ALLEGRO_BITMAP *m_weapon_bitmap;

	Weapon();
	Weapon(char *file, int min_damage, int max_damage, double speed, int classID);
	~Weapon();
};

