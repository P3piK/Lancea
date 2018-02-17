#pragma once
#include "Player.h"
class Mage :
	public Player
{
public:

	virtual void attack(Monster &monster);
	virtual double weapon_speed();
	virtual void DrawAttack(bool &sword, bool &bolt);
	virtual int atk_stat();
	virtual int crit();
	virtual int def();
	virtual float haste();
	virtual void lvl_up();
	virtual double hp_regen();

	Mage();
	~Mage();
};

