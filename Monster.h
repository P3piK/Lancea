#pragma once
#include <string>
#include <allegro5\allegro.h>
#include <random>
#include "mappy_A5.h"

#define WIDTH 1024
#define HEIGHT 768

enum monster_walk_enum { D, U, L, R};

using namespace std;

class Monster
{
protected:
	int min_dmg;
	int max_dmg;
public:
	bool alive = true;
	float m_PosX;
	float m_PosY;
	float prevPosX = m_PosX;
	float prevPosY = m_PosY;

	char * m_name;
	int max_hp;
	int cur_hp = max_hp;
	float speed;

	double attack_speed;
	bool monster_walk[4];
	int exp;
	bool wounded = false;
	int death_frame;
	int range;

	bool OnScreen();
	void WalkMonster();
	void Follow_Player();
	int Calculate_Attack();

	int count;

	int imageWidth;
	int imageHeight;
	ALLEGRO_BITMAP *m_Front, *m_Back, *m_Left, *m_Right, *m_dead;
	ALLEGRO_BITMAP *m_WalkFront[2], *m_WalkBack[2], *m_WalkLeft[2], *m_WalkRight[2];


	Monster();
	~Monster();
};

