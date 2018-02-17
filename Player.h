#pragma once
#include <string>
#include <fstream>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include "Weapon.h"
#include "Armor.h"
#include "Monster.h"

#include "rapidxml\rapidxml.hpp"
#include "rapidxml\rapidxml_print.hpp"

#define FULL_WIDTH 1228
#define WIDTH 1024
#define HEIGHT 768

using namespace std;
using namespace rapidxml;

class Player
{	
protected:
	int m_strength;
	int m_agility;
	int m_stamina;
	int m_intelligence;
	int m_spirit;

	int LVL = 1;
	int expPerLVL();
public:
	Weapon m_weapon;
	Armor m_head_armor;
	Armor m_chest_armor;
	Armor m_legs_armor;
	Armor m_boots_armor;

	string name;
	int class_id = 0;
	int max_range;
	int m_hp;
	int m_total_hp;

	bool died = false;

	virtual int atk_stat() = 0;			//ATK
	virtual int crit() = 0;
	virtual int def() = 0;
	//BLK
	virtual float haste() = 0;
	virtual void lvl_up() = 0;
	virtual double hp_regen() = 0;
	int increase_hp();
	double base_speed;
	double cur_speed;

	int experience = 0;

	// void change_hp(int x);
	int CheckForLvlUp();
	void DrawEq();
	void DrawHPHUD();
	void DrawStats(ALLEGRO_FONT *);
	void LoadCharacter(char *, char*);
	void SaveCharactersData(int slot_number, char * file_content);

	float m_PosX;
	float m_PosY;
	float prevPosX = m_PosX;
	float prevPosY = m_PosY;

	int imageHeight;
	int imageWidth;

	ALLEGRO_BITMAP *m_Front, *m_Back, *m_Left, *m_Right;
	ALLEGRO_BITMAP *m_WalkFront[2], *m_WalkBack[2], *m_WalkLeft[2], *m_WalkRight[2];


	virtual void attack(Monster &) = 0;
	virtual double weapon_speed() = 0;
	virtual void DrawAttack(bool &sword, bool &bolt) = 0;

	Player();
	/*Player(float magePosX, float magePosY, 
		Weapon weapon, Armor head_armor, Armor chest_armor, Armor legs_armor, Armor boots_armor,
		ALLEGRO_BITMAP *Front, ALLEGRO_BITMAP *Back, ALLEGRO_BITMAP *Left, ALLEGRO_BITMAP *Right,
		ALLEGRO_BITMAP *WalkFront[2], ALLEGRO_BITMAP *WalkBack[2], ALLEGRO_BITMAP *WalkLeft[2], ALLEGRO_BITMAP *WalkRight[2]);*/
	~Player();
};

