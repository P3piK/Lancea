#include "Weapon.h"


Weapon::Weapon()
{
}

Weapon::Weapon(char *file, int min_damage, int max_damage, double speed, int classID)
{
	m_weapon_bitmap = al_load_bitmap(file);
	m_min_damage = min_damage;
	m_max_damage = max_damage;
	m_speed = speed;
	m_classID = classID;
}

int Weapon::calculate_attack()
{
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(m_min_damage, m_max_damage); // guaranteed unbiased

	auto random_integer = uni(rng);

	return random_integer;
}
int Weapon::m_dps()
{
	return (m_min_damage + m_max_damage) / 2 / m_speed;
}

Weapon::~Weapon()
{
}
