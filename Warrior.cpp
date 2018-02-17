#include "Warrior.h"

void Warrior::attack(Monster &monster)
{
	int dps_done = m_strength * 0.9 + m_weapon.calculate_attack();
	monster.cur_hp -= dps_done;
}
double Warrior::weapon_speed()
{
	return m_weapon.m_speed - haste();
}
void Warrior::DrawAttack(bool &sword, bool &bolt)
{
	sword = true;
}
int Warrior::atk_stat()
{
	return m_strength*0.8 + m_weapon.m_dps();
}
int Warrior::crit()
{
	return 0;
}
int Warrior::def()
{
	return 0;
}
float Warrior::haste()
{
	return m_agility*0.005;
}
void Warrior::lvl_up()
{
	experience -= expPerLVL();
	LVL++;
	expPerLVL();

	m_strength += 9;
	m_stamina += 10;
	m_agility += 2;
	m_spirit += 7;
	m_total_hp = increase_hp();
	m_hp = m_total_hp;
}
double Warrior::hp_regen()
{
	return 600 * 4 / m_spirit;		// 600 frames - when 12 spirit it regenerates 1hp every 3 sec
}


Warrior::Warrior()
{
	m_total_hp = increase_hp();
	m_hp = m_total_hp;
	expPerLVL();
	max_range = 100;
	base_speed = 1.3;
	cur_speed = base_speed;
}


Warrior::~Warrior()
{
}
