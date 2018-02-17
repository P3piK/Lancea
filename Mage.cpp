#include "Mage.h"

void Mage::attack(Monster &monster)
{
	int dps_done = m_intelligence * 0.4 + m_weapon.calculate_attack();
	monster.cur_hp -= dps_done;
}
double Mage::weapon_speed()
{
	return m_weapon.m_speed - haste();
}
void Mage::DrawAttack(bool &sword, bool &bolt)
{
	bolt = true;
}
int Mage::atk_stat()
{
	return m_intelligence*0.4 + m_weapon.m_dps();
}
int Mage::crit()
{
	return 0;
}
int Mage::def()
{
	return 0;
}
float Mage::haste()
{
	return m_intelligence*0.005 + m_spirit*0.012;
}
void Mage::lvl_up()
{
	experience -= expPerLVL();
	LVL++;
	expPerLVL();

	m_intelligence += 5;
	m_stamina += 2;
	m_spirit += 4;
	m_total_hp = increase_hp();
	m_hp = m_total_hp;
}
double Mage::hp_regen()
{
	return 600 * 6 / m_spirit;		// 600 frames
}

Mage::Mage()
{
	m_total_hp = increase_hp();
	m_hp = m_total_hp;
	max_range = 300;
	base_speed = 1.5;
	cur_speed = base_speed;
}


Mage::~Mage()
{
}
