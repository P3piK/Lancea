#include "Monster.h"


Monster::Monster()
{
	for (int i = 0; i < 4; i++)
		monster_walk[i] = false;

	count = 60 + (rand() % (int)(130 - 60 + 1));
}

bool Monster::OnScreen()
{
	if (m_PosX > -100 &&
		m_PosX < WIDTH + 10 &&
		m_PosY > -100 &&
		m_PosY < HEIGHT + 10)
		return true;
	else 
		return false;
}

void Monster::WalkMonster()
{
	m_PosX -= monster_walk[R] * speed;
	m_PosX += monster_walk[L] * speed;
	m_PosY += monster_walk[U] * speed;
	m_PosY -= monster_walk[D] * speed;
}

void Monster::Follow_Player()
{
	if (m_PosX > WIDTH / 2.0)
	{
		monster_walk[R] = true;
		monster_walk[L] = monster_walk[U] = monster_walk[D] = false;
	}
	else if (m_PosX + imageWidth < WIDTH / 2.0)
	{
		monster_walk[L] = true;
		monster_walk[R] = monster_walk[U] = monster_walk[D] = false;
	}
	else if (m_PosY + imageHeight < HEIGHT / 2.0)
	{
		monster_walk[U] = true;
		monster_walk[R] = monster_walk[L] = monster_walk[D] = false;
	}
	else if (m_PosY > HEIGHT / 2.0)
	{
		monster_walk[D] = true;
		monster_walk[R] = monster_walk[L] = monster_walk[U] = false;
	}
}
int Monster::Calculate_Attack()
{
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(min_dmg, max_dmg); // guaranteed unbiased

	auto random_integer = uni(rng);

	return random_integer;
}

Monster::~Monster()
{
}
