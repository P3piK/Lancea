#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_ttf.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <random>
#include <math.h>

#include "mappy_A5.h"
#include "Player.h"
#include "Warrior.h"
#include "Mage.h"
#include "Rogue.h"
#include "Monster.h"
#include "Wolf.h"
#include "Snake.h"
#include "Weapon.h"
#include "Armor.h"
#include "Button.h"

#include "rapidxml\rapidxml.hpp"
#include "rapidxml\rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

#define FULL_WIDTH 1228
#define WIDTH 1024
#define HEIGHT 768


enum direction { DOWN, UP, LEFT, RIGHT };
bool direction[4] = { true, false, false, false };
bool walk[4] = { false, false, false, false };

void EventKeyDown(ALLEGRO_EVENT ev, bool &done, bool &render, ALLEGRO_EVENT_QUEUE *event_queue, int &newPosX, int &newPosY, Player *player_ptr);	// when key is pressed
void EventKeyUp(ALLEGRO_EVENT ev, bool &done, int &curFrame, int &frameCount);	// when key is released
void Move(Player *&player_ptr);		// main move function
void MoveOnMouse(Player *&player_ptr, int &newPosX, int &newPosY, float oldPosX, float oldPosY);	// selects coordinates to move 

void initWarrior(Warrior &war_char);
void initMage(Mage &mage_char);
void initRogue(Rogue &rogue_char);
void DrawStandingCharacter(Player *player_ptr);
void DrawWalkingCharacter(Player *player_ptr, int curFrame);
void SetWalkFalse();
void SetDirectionFalse();
void SetPosZero(int &newPosX, int &newPosY);
int Collision(Player *player_ptr);
int PlayerMonsterCollision(Monster monster, Player *&player_ptr, int &newPosX, int &newPosY);
int OnPath(Player *player_ptr);
void destroyPlayer(Player *&player_ptr);

void InitWeapon(Weapon &weapon);
void DrawHPHUD();
void DrawEqMenu();

void destroyMonster(Monster &monst);
void initWolf(Wolf &wolf);
void initSnake(Snake &snake);
void MoveMonster(Monster &monster, Player *player_ptr);
void WalkCountMonster(Monster &monster, Player *player_ptr, int count);
void DrawStandingMonster(Monster &monster);
void DrawWalkingMonster(Monster &monster, int curFrame);
void DrawDeadMonster(Monster monster);

void EnterMenu(bool &done, int &option, int &load_option, ALLEGRO_USTR *&name);
void ChooseCharacterMenu(bool &done, bool &exit, int &option, ALLEGRO_USTR *&name);
void LoadGameMenu(bool &exit, int &load_option, ALLEGRO_USTR *&name);
void EscapeMenu(bool &done, Player *player_ptr);
void StatsMenu(Player *player_ptr, bool &done);
void SaveMenu(Player *player_ptr, bool &exit);
bool MouseCoursor(ALLEGRO_EVENT menu_ev, Button button);

bool MonsterCollision(Monster monster, Player *player_ptr);
void DrawMonsterHP(Monster &monster, Player *&player_ptr);
int TargetMonster(Monster, float, float);
void DrawTargetSquare(Monster);
void WalkToDirection();
void MoveBolt(float &boltX, float &boltY, Monster monster, bool &bolt);
int PrintDeathMessage();
void RespawnMonster(Monster &monster, Player *player_ptr);
void ReadSlotNames(char *&, char*&, char*&);
char * FileToChar(char *);
int ReadClassID(char * file_content, int load_option);
void SaveSlotNames(char*, char*, char*);
