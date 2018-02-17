#include "Rogue.h"

void Rogue::attack(Monster &monster)
{
	int dps_done = m_agility * 0.1 + m_weapon.calculate_attack();
	monster.cur_hp -= dps_done;
}
double Rogue::weapon_speed()
{
	if (m_weapon.m_speed - haste() > 0)
		return m_weapon.m_speed - haste();
	else
		return 0.1;
}
void Rogue::DrawAttack(bool &sword, bool &bolt)
{
	sword = true;
}
int Rogue::atk_stat()
{
	return m_agility*0.1 + m_weapon.m_dps();
}
int Rogue::crit()
{
	return 0;
}
int Rogue::def()
{
	return 0;
}
float Rogue::haste()
{
	return m_agility*0.02;
}
void Rogue::lvl_up()
{
	experience -= expPerLVL();
	LVL++;
	expPerLVL();

	m_agility += 6;
	m_stamina += 4;
	m_spirit += 5;
	m_total_hp = increase_hp();
	m_hp = m_total_hp;
}
double Rogue::hp_regen()
{
	return 600 * 3 / m_spirit;		// 600 frames
}

Rogue::Rogue()
{
	m_total_hp = increase_hp();
	m_hp = m_total_hp;
	max_range = 70;
	base_speed = 1.7;
	cur_speed = base_speed;
	m_agility = 18;
	m_stamina = 15;
}


Rogue::~Rogue()
{
}
