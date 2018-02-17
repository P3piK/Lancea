#include "Source.h"

int main()
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	Player * player_ptr;
	ALLEGRO_USTR *name = al_ustr_new("");


	bool done = false;	// game loop condition
	bool render = false; // for rendering screen

	int COUNT = 0; // counting every 1/60 sec

	// files
	char * file_name = "CharactersData.xml";

	// animation variables
	const int maxFrame = 2;		//
	int curFrame = 0;			// 
	int frameCount = 0;			//
	int frameDelay = 20;		//

	// MoveOnMouse variables
	int destinX = WIDTH / 2;
	int destinY = HEIGHT / 2;
	int newPosX = 0;
	int newPosY = 0;
	bool move_valid;

	int option = 0;
	int load_option = 0; // loading a character
	// targetting
	bool target_set = false;
	int monster_targetted = 0;
	bool target_attack = false;
	bool sword_attack = false;
	bool bolt = false;
	float boltX = WIDTH / 2;
	float boltY = HEIGHT / 2;
	int respawn_delay = 300;
	int spawn_point_X = 761;
	int spawn_point_Y = 605;

	// INITIALIZATION
	if (!al_init())
	{
		cerr << "Allegro not loaded properly." << endl;
		return -1;
	}


	display = al_create_display(FULL_WIDTH, HEIGHT);

	if (!display)
	{
		cerr << "Display not loaded properly." << endl;
		return -1;
	}

	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	if (MapLoad("bg1.FMP", 1))
		return -5;
	/////////////////////////////////////////////////////////////////////////

	EnterMenu(done, option, load_option, name);		// enter menu screen

	Warrior war_char;
	Mage mage_char;
	Rogue rog_char;
	
	if (load_option)
		option = ReadClassID(FileToChar(file_name), load_option);

	if (option == 1)
	{
		initWarrior(war_char);
		player_ptr = &war_char;
		player_ptr->imageHeight = 64;
		player_ptr->imageWidth = 64;
		player_ptr->class_id = 1;
	}
	else if (option == 2)
	{
		initMage(mage_char);
		player_ptr = &mage_char;
		player_ptr->imageHeight = 61;
		player_ptr->imageWidth = 39;
		player_ptr->class_id = 2;
	}
	else
	{
		initRogue(rog_char);
		player_ptr = &rog_char;
		player_ptr->imageHeight = 34;
		player_ptr->imageWidth = 33;
		player_ptr->class_id = 3;
	}

	if (load_option == 1)
	{
		player_ptr->LoadCharacter("Slot1", FileToChar(file_name));
	}
	else if (load_option == 2)
	{
		player_ptr->LoadCharacter("Slot2", FileToChar(file_name));
	}
	else if (load_option == 3)
	{
		player_ptr->LoadCharacter("Slot3", FileToChar(file_name));
	}
	else
	{
		player_ptr->m_PosX = spawn_point_X;
		player_ptr->m_PosY = spawn_point_Y;
	}
	player_ptr->name = al_cstr(name);
	player_ptr->m_total_hp = player_ptr->increase_hp();
	player_ptr->m_hp = player_ptr->m_total_hp;


	float oldPosX = player_ptr->m_PosX;
	float oldPosY = player_ptr->m_PosY;

	Monster monster[9];
	Wolf wolf[4];
	Snake snake[5];
	int number_of_monsters = 9;

	initWolf(wolf[0]);
	initWolf(wolf[1]);
	initWolf(wolf[2]);
	initWolf(wolf[3]);
	initSnake(snake[0]);
	initSnake(snake[1]);
	initSnake(snake[2]);
	initSnake(snake[3]);
	initSnake(snake[4]);

	// INITIALIZATIONS FOR TESTING

	Weapon w1("graphics/3213.gif", 3, 7, 1.2, 0);
	player_ptr->m_weapon = w1;
	/////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	monster[0] = wolf[0];
	monster[1] = wolf[1];
	monster[2] = wolf[2];
	monster[3] = wolf[3];
	monster[4] = snake[0];
	monster[5] = snake[1];
	monster[6] = snake[2];
	monster[7] = snake[3];
	monster[8] = snake[4];

	for (int i = 0; i < number_of_monsters; i++)
		RespawnMonster(monster[i], player_ptr);


	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	//al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		EventKeyDown(ev, done, render, event_queue, newPosX, newPosY, player_ptr);	// if key down
		EventKeyUp(ev, done, curFrame, frameCount);									// if key up

		if (ev.type == ALLEGRO_EVENT_TIMER)		
		{
			if (++frameCount >= frameDelay)		// for changing bitmaps - animations
			{
				if (++curFrame >= maxFrame)
					curFrame = 0;

				frameCount = 0;
			}

			if ((COUNT % int(player_ptr->hp_regen()) == 0) && player_ptr->m_hp != player_ptr->m_total_hp)		// hp regen
				player_ptr->m_hp++;

			for (int i = 0; i < number_of_monsters; i++)		// monster attack
			{
				if (monster[i].wounded && monster[i].alive)
				{
					monster[i].Follow_Player();

					if (sqrt(pow(monster[i].m_PosX + monster[i].imageWidth / 2 - WIDTH / 2, 2) +
						pow(monster[i].m_PosY + monster[i].imageHeight / 2 - HEIGHT / 2, 2)) < monster[i].range)		// calculating range
						if (COUNT % int(monster[i].attack_speed * 60) == 0)
							player_ptr->m_hp -= monster[i].Calculate_Attack();	
				}
			}

			if (target_set)
			{
				if (!monster[monster_targetted].alive)	// untarget if monster's dead
					target_set = false;

				if (bolt)
					MoveBolt(boltX, boltY, monster[monster_targetted], bolt);	// animation for a mage's bolt

				if (sqrt(pow(WIDTH / 2 - (monster[monster_targetted].m_PosX + monster[monster_targetted].imageWidth / 2), 2) + 
					pow(HEIGHT / 2 - (monster[monster_targetted].m_PosY + monster[monster_targetted].imageHeight / 2), 2)) < player_ptr->max_range)	// check if in range
				{
					/*if (COUNT % int(0.95*(player_ptr->m_weapon.m_speed * 60)) == 0)
					{
					player_ptr->DrawAttack(sword_attack, bolt);
					}*/
					if (COUNT % int(player_ptr->weapon_speed() * 60) == 0)	// attack 
					{
						player_ptr->DrawAttack(sword_attack, bolt);			// attack animation
						player_ptr->attack(monster[monster_targetted]);		// calculate dmg done
						monster[monster_targetted].wounded = true;
						target_attack = true;
					}
				}
				else
					target_attack = false;									// if out of range or untargetted
			}

			MoveOnMouse(player_ptr, newPosX, newPosY, oldPosX, oldPosY);	// walk on mouse mechanics

			for (int i = 0; i < number_of_monsters; i++)
			{
				if (PlayerMonsterCollision(monster[i], player_ptr, newPosX, newPosY)) // check if player not collides with monster
				{
					move_valid = false;
					break;
				}
				else
					move_valid = true;
			}

			if (move_valid)
				Move(player_ptr);

			for (int i = 0; i < number_of_monsters; i++)
			{
				MoveMonster(monster[i], player_ptr);					// moves if player 'moves'
				if (monster[i].alive)	// if monster's not hunting player
				{
					WalkCountMonster(monster[i], player_ptr, COUNT);	// choosing direction of walking
					if (monster[i].m_PosX >  WIDTH / 2 + 1500 || monster[i].m_PosX < WIDTH / 2 - 1500 ||
						monster[i].m_PosY > HEIGHT / 2 + 1000 || monster[i].m_PosY < HEIGHT / 2 - 1000)
						RespawnMonster(monster[i], player_ptr);
				}
				else
					if (monster[i].death_frame + respawn_delay < COUNT)
						RespawnMonster(monster[i], player_ptr);
			}
			
			if (target_set && monster[monster_targetted].cur_hp <= 0)
			{
				bolt = false;
				sword_attack = false;
				monster[monster_targetted].alive = false;
				monster[monster_targetted].death_frame = COUNT;
				player_ptr->experience += monster[monster_targetted].exp;
			}
			if (player_ptr->CheckForLvlUp())
				player_ptr->lvl_up();

			if (player_ptr->m_hp < 0)
				player_ptr->died = true;

			COUNT++;
			render = true;
		}

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1)		// setting destined point
		{
			destinX = ev.mouse.x;
			destinY = ev.mouse.y;
			oldPosX = player_ptr->m_PosX;
			oldPosY = player_ptr->m_PosY;
			newPosX = destinX - WIDTH / 2;
			newPosY = destinY - HEIGHT / 2;
		}
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 2) // targetting
		{
			for (int i = 0; i < number_of_monsters; i++)
				if (TargetMonster(monster[i], ev.mouse.x, ev.mouse.y) && monster[i].alive)	
				{
					target_set = true;
					monster_targetted = i;
					break;
				}
				else
					target_set = false;
		}

		// DRAWINGS
		if (render && al_is_event_queue_empty(event_queue))		
		{
			render = false;

			MapChangeLayer(0);
			MapDrawBG(player_ptr->m_PosX, player_ptr->m_PosY, 0, 0, WIDTH, HEIGHT); // drawing map

			/*if (magePosX > 80 &&dz
				magePosX < 400 &&
				magePosY > 80 &&
				magePosY < 400)
				{
				MapChangeLayer(1);
				MapDrawBG(magePosX, magePosY, 0, 0, WIDTH, HEIGHT);
				}*/
			/*if (target_attack)
			player_ptr->attack(monster[monster_targetted]);*/

			if (target_set && monster[monster_targetted].alive)
				DrawTargetSquare(monster[monster_targetted]);
		
			for (int i = 0; i < number_of_monsters; i++)
				if (!monster[i].alive)
					DrawDeadMonster(monster[i]);

			DrawStandingCharacter(player_ptr);
			DrawWalkingCharacter(player_ptr, curFrame);

			for (int i = 0; i < number_of_monsters; i++)
			{
				if (monster[i].alive)
				{
					DrawStandingMonster(monster[i]);
					DrawWalkingMonster(monster[i], curFrame);
					DrawMonsterHP(monster[i], player_ptr);
				}
			}


			if (bolt && monster[monster_targetted].alive)
				al_draw_filled_circle(boltX, boltY, 5, al_map_rgb(255, 0, 255));

			DrawEqMenu();
			player_ptr->DrawEq();
			DrawHPHUD();
			player_ptr->DrawHPHUD();


			if (player_ptr->died)
				if (PrintDeathMessage())
				{
					player_ptr->died = false;
					player_ptr->m_hp = player_ptr->m_total_hp;
					player_ptr->m_PosX = spawn_point_X;
					player_ptr->m_PosY = spawn_point_Y;
					player_ptr->prevPosX = player_ptr->m_PosX;
					player_ptr->prevPosY = player_ptr->m_PosY;
					target_attack = false;
					for (int i = 0; i < number_of_monsters; i++)
					{
						monster[i].wounded = false;
						monster[i].cur_hp = monster[i].max_hp;
						RespawnMonster(monster[i], player_ptr);
					}
					al_flush_event_queue(event_queue);
				}

			al_flip_display();
		}
	}

	MapFreeMem();
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	// destroyPlayer(player_ptr);
	/*for (int i = 0; i < 4; i++)
		destroyMonster(wolf[i]);
	for (int i = 0; i < 5; i++)
		destroyMonster(snake[i]);*/
	//for (int i = 0; i < number_of_monsters; i++)
	//	destroyMonster(monster[i]);

	return 0;
}

void EventKeyDown(ALLEGRO_EVENT ev, bool &done, bool &render, ALLEGRO_EVENT_QUEUE *event_queue, int &newPosX, int &newPosY, Player *player_ptr)
{
	if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		SetWalkFalse();
		switch (ev.keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE:
			EscapeMenu(done, player_ptr);
			render = true;
			al_flush_event_queue(event_queue);
			break;
		case ALLEGRO_KEY_DOWN:
			walk[DOWN] = true;
			walk[UP] = false;
			walk[LEFT] = false;
			walk[RIGHT] = false;
			SetDirectionFalse();
			SetPosZero(newPosX, newPosY);
			break;
		case ALLEGRO_KEY_UP:
			walk[DOWN] = false;
			walk[UP] = true;
			walk[LEFT] = false;
			walk[RIGHT] = false;
			SetDirectionFalse();
			SetPosZero(newPosX, newPosY);
			break;
		case ALLEGRO_KEY_LEFT:
			walk[DOWN] = false;
			walk[UP] = false;
			walk[LEFT] = true;
			walk[RIGHT] = false;
			SetDirectionFalse();
			SetPosZero(newPosX, newPosY);
			break;
		case ALLEGRO_KEY_RIGHT:
			walk[DOWN] = false;
			walk[UP] = false;
			walk[LEFT] = false;
			walk[RIGHT] = true;
			SetDirectionFalse();
			SetPosZero(newPosX, newPosY);
			break;
		}
	}

}
void EventKeyUp(ALLEGRO_EVENT ev, bool &done, int &curFrame, int &frameCount)
{
	if (ev.type == ALLEGRO_EVENT_KEY_UP)
	{
		/*switch (ev.keyboard.keycode)
		{
		case ALLEGRO_KEY_ESCAPE:
			break;
		case ALLEGRO_KEY_DOWN:
			direction[DOWN] = true;
			direction[UP] = false;
			direction[LEFT] = false;
			direction[RIGHT] = false;
			break;
		case ALLEGRO_KEY_UP:
			direction[DOWN] = false;
			direction[UP] = true;
			direction[LEFT] = false;
			direction[RIGHT] = false;
			break;
		case ALLEGRO_KEY_LEFT:
			direction[DOWN] = false;
			direction[UP] = false;
			direction[LEFT] = true;
			direction[RIGHT] = false;
			break;
		case ALLEGRO_KEY_RIGHT:
			direction[DOWN] = false;
			direction[UP] = false;
			direction[LEFT] = false;
			direction[RIGHT] = true;
			break;
		}*/
		WalkToDirection();
		SetWalkFalse();

		/*switch (ev.keyboard.keycode)
		{
		case ALLEGRO_KEY_UP:
			walk[UP] = false;
		case ALLEGRO_KEY_DOWN:
			walk[DOWN] = false;
		case ALLEGRO_KEY_LEFT:
			walk[LEFT] = false;
		case ALLEGRO_KEY_RIGHT:
			walk[RIGHT] = false;
		}*/

		curFrame = 0;
		frameCount = 0;
	}
}
void Move(Player *&player_ptr)
{
	player_ptr->prevPosX = player_ptr->m_PosX;
	player_ptr->prevPosY = player_ptr->m_PosY;

	//if on path, move faster
	if (OnPath(player_ptr))
	{
		player_ptr->cur_speed = player_ptr->base_speed * 1.4;
		player_ptr->m_PosX += walk[RIGHT] * player_ptr->cur_speed;
		player_ptr->m_PosX -= walk[LEFT] * player_ptr->cur_speed;
		player_ptr->m_PosY -= walk[UP] * player_ptr->cur_speed;
		player_ptr->m_PosY += walk[DOWN] * player_ptr->cur_speed;
	}
	else
	{
		player_ptr->cur_speed = player_ptr->base_speed;
		player_ptr->m_PosX += walk[RIGHT] * player_ptr->cur_speed;
		player_ptr->m_PosX -= walk[LEFT] * player_ptr->cur_speed;
		player_ptr->m_PosY -= walk[UP] * player_ptr->cur_speed;
		player_ptr->m_PosY += walk[DOWN] * player_ptr->cur_speed;
	}

	if (Collision(player_ptr))
	{
		player_ptr->m_PosX = player_ptr->prevPosX;
		player_ptr->m_PosY = player_ptr->prevPosY;
	}

	//Map size restrictions
	if (player_ptr->m_PosX < 0)
		player_ptr->m_PosX = 0;
	if (player_ptr->m_PosY < 0)
		player_ptr->m_PosY = 0;
	if (player_ptr->m_PosY > 4700)
		player_ptr->m_PosY = player_ptr->prevPosY;
	if (player_ptr->m_PosX > (mapwidth * mapblockwidth) - FULL_WIDTH)
		player_ptr->m_PosX = player_ptr->prevPosX;
	/*if (player_ptr->m_PosY > (mapheight * mapblockheight) - HEIGHT)
		player_ptr->m_PosY = player_ptr->prevPosY;*/
}
void MoveOnMouse(Player *&player_ptr, int &newPosX, int &newPosY, float oldPosX, float oldPosY)
{
/*	if ((newPosX != 0 || newPosY != 0) && Collision(player.m_PosX, player.m_PosY))
	{
		newPosX = 0;
		newPosY = 0;
		return;
	}
*/
	// WALK RIGHT
	if (newPosX != 0 && player_ptr->m_PosX < oldPosX + newPosX)
	{
		walk[DOWN] = false;
		walk[UP] = false;
		walk[LEFT] = false;
		walk[RIGHT] = true;
		SetDirectionFalse();

		if (player_ptr->m_PosX + 3 >= oldPosX + newPosX)
		{
			WalkToDirection();
			SetWalkFalse();
			newPosX = 0;
			return;
		}
		return;
	}
	// WALK LEFT
	if (newPosX != 0 && player_ptr->m_PosX > oldPosX + newPosX)
	{
		walk[DOWN] = false;
		walk[UP] = false;
		walk[LEFT] = true;
		walk[RIGHT] = false;
		SetDirectionFalse();

		if (player_ptr->m_PosX - 3 <= oldPosX + newPosX)
		{
			WalkToDirection();
			SetWalkFalse();
			newPosX = 0;
			return;
		}
		return;
	}
	// WALK UP
	if (newPosY != 0 && player_ptr->m_PosY > oldPosY + newPosY)
	{
		walk[DOWN] = false;
		walk[UP] = true;
		walk[LEFT] = false;
		walk[RIGHT] = false;
		SetDirectionFalse();

		if (player_ptr->m_PosY - 2 <= oldPosY + newPosY)
		{
			WalkToDirection();
			SetWalkFalse();
			newPosY = 0;
			return;
		}
		return;
	}
	// WALK DOWN
	if (newPosY != 0 && player_ptr->m_PosY < oldPosY + newPosY)
	{
		walk[DOWN] = true;
		walk[UP] = false;
		walk[LEFT] = false;
		walk[RIGHT] = false;
		SetDirectionFalse();

		if (player_ptr->m_PosY + 2 >= oldPosY + newPosY)
		{
			WalkToDirection();
			SetWalkFalse();
			newPosY = 0;
			return;
		}
		return;
	}
}

void initWarrior(Warrior &war_char)
{
	war_char.m_Front = al_load_bitmap("graphics/war_front.png");
	war_char.m_Back = al_load_bitmap("graphics/war_back.png");
	war_char.m_Left = al_load_bitmap("graphics/war_left.png");
	war_char.m_Right = al_load_bitmap("graphics/war_right.png");
	war_char.m_WalkFront[0] = al_load_bitmap("graphics/war_front_walk1.png");
	war_char.m_WalkFront[1] = al_load_bitmap("graphics/war_front_walk2.png");
	war_char.m_WalkBack[0] = al_load_bitmap("graphics/war_back_walk1.png");
	war_char.m_WalkBack[1] = al_load_bitmap("graphics/war_back_walk2.png");
	war_char.m_WalkLeft[0] = al_load_bitmap("graphics/war_left_walk1.png");
	war_char.m_WalkLeft[1] = al_load_bitmap("graphics/war_left_walk2.png");
	war_char.m_WalkRight[0] = al_load_bitmap("graphics/war_right_walk1.png");
	war_char.m_WalkRight[1] = al_load_bitmap("graphics/war_right_walk2.png");
}
void initMage(Mage &mage_char)
{
	mage_char.m_Front = al_load_bitmap("graphics/mag_front.png");
	mage_char.m_Back = al_load_bitmap("graphics/mag_back.png");
	mage_char.m_Left = al_load_bitmap("graphics/mag_left.png");
	mage_char.m_Right = al_load_bitmap("graphics/mag_right.png");
	mage_char.m_WalkFront[0] = al_load_bitmap("graphics/mag_front_walk1.png");
	mage_char.m_WalkFront[1] = al_load_bitmap("graphics/mag_front_walk2.png");
	mage_char.m_WalkBack[0] = al_load_bitmap("graphics/mag_back_walk1.png");
	mage_char.m_WalkBack[1] = al_load_bitmap("graphics/mag_back_walk2.png");
	mage_char.m_WalkLeft[0] = al_load_bitmap("graphics/mag_left_walk1.png");
	mage_char.m_WalkLeft[1] = al_load_bitmap("graphics/mag_left_walk2.png");
	mage_char.m_WalkRight[0] = al_load_bitmap("graphics/mag_right_walk1.png");
	mage_char.m_WalkRight[1] = al_load_bitmap("graphics/mag_right_walk2.png");
}
void initRogue(Rogue &rog_char)
{
	rog_char.m_Front = al_load_bitmap("graphics/rogue_front.png");
	rog_char.m_Back = al_load_bitmap("graphics/rogue_back.png");
	rog_char.m_Left = al_load_bitmap("graphics/rogue_left.png");
	rog_char.m_Right = al_load_bitmap("graphics/rogue_right.png");
	rog_char.m_WalkFront[0] = al_load_bitmap("graphics/rogue_front_walk1.png");
	rog_char.m_WalkFront[1] = al_load_bitmap("graphics/rogue_front_walk2.png");
	rog_char.m_WalkBack[0] = al_load_bitmap("graphics/rogue_back_walk1.png");
	rog_char.m_WalkBack[1] = al_load_bitmap("graphics/rogue_back_walk2.png");
	rog_char.m_WalkLeft[0] = al_load_bitmap("graphics/rogue_left_walk1.png");
	rog_char.m_WalkLeft[1] = al_load_bitmap("graphics/rogue_left_walk2.png");
	rog_char.m_WalkRight[0] = al_load_bitmap("graphics/rogue_right_walk1.png");
	rog_char.m_WalkRight[1] = al_load_bitmap("graphics/rogue_right_walk2.png");
}
void destroyPlayer(Player *&player_ptr)
{
	al_destroy_bitmap(player_ptr->m_Back);
	al_destroy_bitmap(player_ptr->m_Front);
	al_destroy_bitmap(player_ptr->m_Left);
	al_destroy_bitmap(player_ptr->m_Right);
	for (int i = 0; i < 2; i++)
	{
		al_destroy_bitmap(player_ptr->m_WalkBack[i]);
		al_destroy_bitmap(player_ptr->m_WalkFront[i]);
		al_destroy_bitmap(player_ptr->m_WalkLeft[i]);
		al_destroy_bitmap(player_ptr->m_WalkRight[i]);
	}
}
void DrawStandingCharacter(Player *player_ptr)
{
	if (!walk[0] || !walk[1] || !walk[2] || !walk[3])
	{
		if (direction[DOWN])
			al_draw_bitmap(player_ptr->m_Front, WIDTH / 2 - player_ptr->imageWidth / 2, HEIGHT / 2 - player_ptr->imageHeight / 2, 0);
		if (direction[UP])
			al_draw_bitmap(player_ptr->m_Back, WIDTH / 2 - player_ptr->imageWidth / 2, HEIGHT / 2 - player_ptr->imageHeight / 2, 0);
		if (direction[LEFT])
			al_draw_bitmap(player_ptr->m_Left, WIDTH / 2 - player_ptr->imageWidth / 2, HEIGHT / 2 - player_ptr->imageHeight / 2, 0);
		if (direction[RIGHT])
			al_draw_bitmap(player_ptr->m_Right, WIDTH / 2 - player_ptr->imageWidth / 2, HEIGHT / 2 - player_ptr->imageHeight / 2, 0);
	}
}
void DrawWalkingCharacter(Player *player_ptr, int curFrame)
{
	if (walk[DOWN])
		al_draw_bitmap(player_ptr->m_WalkFront[curFrame], WIDTH / 2 - player_ptr->imageWidth / 2, HEIGHT / 2 - player_ptr->imageHeight / 2, 0);
	if (walk[UP])
		al_draw_bitmap(player_ptr->m_WalkBack[curFrame], WIDTH / 2 - player_ptr->imageWidth / 2, HEIGHT / 2 - player_ptr->imageHeight / 2, 0);
	if (walk[LEFT])
		al_draw_bitmap(player_ptr->m_WalkLeft[curFrame], WIDTH / 2 - player_ptr->imageWidth / 2, HEIGHT / 2 - player_ptr->imageHeight / 2, 0);
	if (walk[RIGHT])
		al_draw_bitmap(player_ptr->m_WalkRight[curFrame], WIDTH / 2 - player_ptr->imageWidth / 2, HEIGHT / 2 - player_ptr->imageHeight / 2, 0);
}

void SetWalkFalse()
{
	for (int i = 0; i < 4; i++)
		walk[i] = false;
}
void SetDirectionFalse()
{
	for (int i = 0; i < 4; i++)
		direction[i] = false;
}
void SetPosZero(int &newPosX, int &newPosY)
{
	newPosX = 0;
	newPosY = 0;
}
int Collision(Player *player_ptr)
{
	BLKSTR * blkdatapt;
	BLKSTR * blkdatapt1;
	float x = player_ptr->m_PosX;
	float y = player_ptr->m_PosY;

	blkdatapt = MapGetBlock((x + WIDTH / 2 - player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - player_ptr->imageHeight / 2) / mapblockheight);
	blkdatapt1 = MapGetBlock((x + WIDTH / 2 - player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - player_ptr->imageHeight / 2) / mapblockheight);

	if (walk[DOWN])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 + player_ptr->imageHeight / 2) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 + player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 + player_ptr->imageHeight / 2) / mapblockheight);
	}
	if (walk[UP])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - player_ptr->imageHeight / 2) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 + player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - player_ptr->imageHeight / 2) / mapblockheight);
	}
	if (walk[LEFT])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - player_ptr->imageHeight / 2) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 - player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 + player_ptr->imageHeight / 2) / mapblockheight);
	}
	if (walk[RIGHT])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 + player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - player_ptr->imageHeight / 2) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 + player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 + player_ptr->imageHeight / 2) / mapblockheight);
	}


	if (blkdatapt->tl || blkdatapt1->tl)
		return 1; 
	else
		return 0;
}
int PlayerMonsterCollision(Monster monster, Player *&player_ptr, int &newPosX, int &newPosY)
{
	if (monster.alive)
	{
		if (walk[RIGHT] &&
			WIDTH / 2 + player_ptr->imageWidth / 2.0 <= monster.m_PosX + monster.imageWidth &&
			WIDTH / 2 + player_ptr->imageWidth / 2.0 >= monster.m_PosX &&
			((HEIGHT / 2 - player_ptr->imageHeight / 2.0 <= monster.m_PosY + monster.imageHeight && HEIGHT / 2 - player_ptr->imageHeight / 2.0 >= monster.m_PosY) ||
			(HEIGHT / 2 + player_ptr->imageHeight / 2.0 <= monster.m_PosY + monster.imageHeight && HEIGHT / 2 + player_ptr->imageHeight / 2.0 >= monster.m_PosY)))
		{
			player_ptr->m_PosX = player_ptr->prevPosX;
			player_ptr->m_PosY = player_ptr->prevPosY;
			monster.m_PosX = monster.prevPosX;
			monster.m_PosY = monster.prevPosY;
			player_ptr->cur_speed = 0;

			newPosX = 0;
			newPosY = 0;
			WalkToDirection();
			SetWalkFalse();
			return 1;
		}
		else if (walk[LEFT] &&
			WIDTH / 2 - player_ptr->imageWidth / 2.0 <= monster.m_PosX + monster.imageWidth &&
			WIDTH / 2 - player_ptr->imageWidth / 2.0 >= monster.m_PosX &&
			((HEIGHT / 2 - player_ptr->imageHeight / 2.0 <= monster.m_PosY + monster.imageHeight && HEIGHT / 2 - player_ptr->imageHeight / 2.0 >= monster.m_PosY) ||
			(HEIGHT / 2 + player_ptr->imageHeight / 2.0 <= monster.m_PosY + monster.imageHeight && HEIGHT / 2 + player_ptr->imageHeight / 2.0 >= monster.m_PosY)))
		{
			player_ptr->m_PosX = player_ptr->prevPosX;
			player_ptr->m_PosY = player_ptr->prevPosY;
			monster.m_PosX = monster.prevPosX;
			monster.m_PosY = monster.prevPosY;
			player_ptr->cur_speed = 0;

			newPosX = 0;
			newPosY = 0;
			WalkToDirection();
			SetWalkFalse();
			return 1;
		}
		else if (walk[UP] &&
			((WIDTH / 2 - player_ptr->imageWidth / 2.0 <= monster.m_PosX + monster.imageWidth && WIDTH / 2 - player_ptr->imageWidth / 2.0 >= monster.m_PosX) ||
			(WIDTH / 2 + player_ptr->imageWidth / 2.0 <= monster.m_PosX + monster.imageWidth && WIDTH / 2 + player_ptr->imageWidth / 2.0 >= monster.m_PosX)) &&
			HEIGHT / 2 - player_ptr->imageHeight / 2.0 <= monster.m_PosY + monster.imageHeight &&
			HEIGHT / 2 - player_ptr->imageHeight / 2.0 >= monster.m_PosY)
		{
			player_ptr->m_PosX = player_ptr->prevPosX;
			player_ptr->m_PosY = player_ptr->prevPosY;
			monster.m_PosX = monster.prevPosX;
			monster.m_PosY = monster.prevPosY;
			player_ptr->cur_speed = 0;

			newPosX = 0;
			newPosY = 0;
			WalkToDirection();
			SetWalkFalse();
			return 1;
		}
		else if (walk[DOWN] &&
			((WIDTH / 2 - player_ptr->imageWidth / 2.0 <= monster.m_PosX + monster.imageWidth && WIDTH / 2 - player_ptr->imageWidth / 2.0 >= monster.m_PosX) ||
			(WIDTH / 2 + player_ptr->imageWidth / 2.0 <= monster.m_PosX + monster.imageWidth && WIDTH / 2 + player_ptr->imageWidth / 2.0 >= monster.m_PosX)) &&
			HEIGHT / 2 + player_ptr->imageHeight / 2.0 <= monster.m_PosY + monster.imageHeight &&
			HEIGHT / 2 + player_ptr->imageHeight / 2.0 >= monster.m_PosY)
		{
			player_ptr->m_PosX = player_ptr->prevPosX;
			player_ptr->m_PosY = player_ptr->prevPosY;
			monster.m_PosX = monster.prevPosX;
			monster.m_PosY = monster.prevPosY;
			player_ptr->cur_speed = 0;

			newPosX = 0;
			newPosY = 0;
			WalkToDirection();
			SetWalkFalse();

			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}
int OnPath(Player *player_ptr)
{
	BLKSTR * blkdatapt;
	BLKSTR * blkdatapt1;
	float x = player_ptr->m_PosX;
	float y = player_ptr->m_PosY;

	blkdatapt = MapGetBlock((x + WIDTH / 2 - player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - player_ptr->imageHeight / 2) / mapblockheight);
	blkdatapt1 = MapGetBlock((x + WIDTH / 2 - player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - player_ptr->imageHeight / 2) / mapblockheight);

	if (walk[DOWN])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - 0.7 * player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 + player_ptr->imageHeight / 2) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 + 0.7 * player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 + player_ptr->imageHeight / 2) / mapblockheight);
	}
	if (walk[UP])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - 0.7 * player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - player_ptr->imageHeight / 2) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 + 0.7 * player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - player_ptr->imageHeight / 2) / mapblockheight);
	}
	if (walk[LEFT])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - 0.7 * player_ptr->imageHeight) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 - player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 + 0.7 * player_ptr->imageHeight) / mapblockheight);
	}
	if (walk[RIGHT])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 + player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - 0.7 * player_ptr->imageHeight / 2) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 + player_ptr->imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 + 0.7 * player_ptr->imageHeight / 2) / mapblockheight);
	}

	if (blkdatapt->tr && blkdatapt1->tr)
		return 1;
	else
		return 0;
}

void DrawHPHUD()
{
	ALLEGRO_FONT * font = al_load_font("ArchitectHand.ttf", 17, 0);
	al_draw_filled_rounded_rectangle(-10, -10, 150, 80, 10, 10, al_map_rgb(120, 120, 120));
	al_draw_text(font, al_map_rgb(255, 255, 255), 30, 5, ALLEGRO_ALIGN_RIGHT, "HP");
	al_draw_text(font, al_map_rgb(255, 255, 255), 30, 30, ALLEGRO_ALIGN_RIGHT, "EXP");
	al_draw_text(font, al_map_rgb(255, 255, 255), 30, 55, ALLEGRO_ALIGN_RIGHT, "LVL");
	al_destroy_font(font);
}
void DrawEqMenu()
{
	al_draw_filled_rectangle(WIDTH, 0, FULL_WIDTH, HEIGHT, al_map_rgb(60, 0, 0));
	al_draw_filled_rectangle(WIDTH + (FULL_WIDTH - WIDTH) / 2 - 25, 50, WIDTH + (FULL_WIDTH - WIDTH) / 2 + 25, 100, al_map_rgb(0, 0, 0)); // head slot
	al_draw_filled_rectangle(WIDTH + 20, 110, WIDTH + 70, 160, al_map_rgb(0, 0, 0)); // main hand
	al_draw_filled_rectangle(WIDTH + (FULL_WIDTH - WIDTH) / 2 - 25, 110, WIDTH + (FULL_WIDTH - WIDTH) / 2 + 25, 160, al_map_rgb(0, 0, 0)); // chest
	al_draw_filled_rectangle(FULL_WIDTH - 70, 110, FULL_WIDTH - 20, 160, al_map_rgb(0, 0, 0)); // off hand
	al_draw_filled_rectangle(WIDTH + (FULL_WIDTH - WIDTH) / 2 - 25, 170, WIDTH + (FULL_WIDTH - WIDTH) / 2 + 25, 220, al_map_rgb(0, 0, 0)); // pants
	al_draw_filled_rectangle(WIDTH + (FULL_WIDTH - WIDTH) / 2 - 25, 230, WIDTH + (FULL_WIDTH - WIDTH) / 2 + 25, 280, al_map_rgb(0, 0, 0)); // boots
	al_draw_filled_rectangle(WIDTH + 20, 170, WIDTH + 70, 220, al_map_rgb(0, 0, 0)); // additional slot 1
	al_draw_filled_rectangle(FULL_WIDTH - 70, 170, FULL_WIDTH - 20, 220, al_map_rgb(0, 0, 0)); // additional slot 2
}

void EnterMenu(bool &done, int &option, int &load_option, ALLEGRO_USTR *&name)
{
	ALLEGRO_FONT *gd_font48;
	gd_font48 = al_load_font("contrast.ttf", 48, 0);
	ALLEGRO_FONT *gd_font36;
	gd_font36 = al_load_font("contrast.ttf", 36, 0);

	Button new_game_button(FULL_WIDTH / 2 - 200, FULL_WIDTH / 2 + 200, HEIGHT / 2 - 180, HEIGHT / 2 - 110, FULL_WIDTH / 2, HEIGHT / 2 - 170, gd_font48, "NEW GAME");
	Button load_game_button(FULL_WIDTH / 2 - 200, FULL_WIDTH / 2 + 200, HEIGHT / 2 - 100, HEIGHT / 2 - 30, FULL_WIDTH / 2, HEIGHT / 2 - 85, gd_font36, "LOAD CHARACTER");
	Button exit_button(FULL_WIDTH / 2 - 200, FULL_WIDTH / 2 + 200, HEIGHT / 2 + 150, HEIGHT / 2 + 220, FULL_WIDTH / 2, HEIGHT / 2 + 160, gd_font48, "EXIT");

	ALLEGRO_MOUSE_STATE state;
	ALLEGRO_EVENT_QUEUE *menu_ev_queue = NULL;
	menu_ev_queue = al_create_event_queue();
	al_register_event_source(menu_ev_queue, al_get_keyboard_event_source());
	al_register_event_source(menu_ev_queue, al_get_mouse_event_source());

	bool exit = false;
	bool render = true;

	while (!exit)
	{
		if (render)
		{
			//DRAWINGS
			al_clear_to_color(al_map_rgb(180, 180, 180)); // background color
			new_game_button.DrawButton();
			load_game_button.DrawButton();
			exit_button.DrawButton();
			al_flip_display();
			render = false;
		}

		ALLEGRO_EVENT menu_ev;
		al_wait_for_event(menu_ev_queue, &menu_ev);
		al_get_mouse_state(&state);

		//////////////// HIGHLIGHT WHEN ON BUTTON \\\\\\\\\\\\\\\\\\\
		// NEW GAME
		if (MouseCoursor(menu_ev, new_game_button))
			new_game_button.HighlightButton(render);
		// LOAD CHARACTER
		else if (MouseCoursor(menu_ev, load_game_button))
			load_game_button.HighlightButton(render);
		// EXIT
		else if (MouseCoursor(menu_ev, exit_button))
			exit_button.HighlightButton(render);
		else
			render = true;
		//////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


		if (menu_ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (menu_ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				exit = true;
				done = true;
			}
		}

		// MOUSE BUTTON PRESSED
		if (menu_ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			// button 1
			if (MouseCoursor(menu_ev, new_game_button))
			{
				ChooseCharacterMenu(done, exit, option, name);
				load_option = 0;
				render = true;
				al_flush_event_queue(menu_ev_queue);
			}
			// button 2
			if (MouseCoursor(menu_ev, load_game_button))
			{
				LoadGameMenu(exit, load_option, name);
				render = true;
				al_flush_event_queue(menu_ev_queue);
			}
			// button 3
			if (MouseCoursor(menu_ev, exit_button))
			{
				exit = true;
				done = true;
			}
		}
	}
	al_destroy_event_queue(menu_ev_queue);
	al_destroy_font(gd_font36);
	al_destroy_font(gd_font48);
}
void ChooseCharacterMenu(bool &done, bool &exit, int &option, ALLEGRO_USTR *&name)
{
	ALLEGRO_USTR *paddle = al_ustr_new("_");

	int pos = (int)al_ustr_size(name);

	ALLEGRO_BITMAP *war_face = al_load_bitmap("graphics/war_face.jpg");
	ALLEGRO_BITMAP *mage_face = al_load_bitmap("graphics/mage_face.jpg");
	ALLEGRO_BITMAP *rogue_face = al_load_bitmap("graphics/rogue_face.png");

	ALLEGRO_FONT * c_font48 = al_load_font("contrast.ttf", 48, 0);
	ALLEGRO_FONT * ah_font38 = al_load_font("arial.ttf", 30, 0);
	ALLEGRO_FONT * arial24 = al_load_font("arial.ttf", 24, 0);
	Button button_play(FULL_WIDTH / 2 + 200, FULL_WIDTH / 2 + 420, HEIGHT - 120, HEIGHT - 40, FULL_WIDTH / 2 + 310, HEIGHT - 110, c_font48, "Play");
	Button button_back(WIDTH / 2 - 300, WIDTH / 2 - 100, HEIGHT - 120, HEIGHT - 40, WIDTH / 2 - 200, HEIGHT - 110, c_font48, "Back");
	Button button_warrior(79, 381, 99, 491, 0, 0, c_font48, "");
	Button button_mage(FULL_WIDTH / 2 - 151, FULL_WIDTH / 2 + 151, 99, 491, 0, 0, c_font48, "");
	Button button_rogue(FULL_WIDTH - 381, FULL_WIDTH - 79, 99, 491, 0, 0, c_font48, "");

	ALLEGRO_MOUSE_STATE state;
	ALLEGRO_EVENT_QUEUE *menu_ev_queue = NULL;
	menu_ev_queue = al_create_event_queue();
	al_register_event_source(menu_ev_queue, al_get_keyboard_event_source());
	al_register_event_source(menu_ev_queue, al_get_mouse_event_source());

	bool exit_menu = false;
	bool input_name = false;
	bool render = true;
	bool class_warning = false;
	bool nick_warning = false;
	bool nick_length_warning = false;
	int count = 0;
	option = 0;
	
	while (!exit_menu)
	{
		if (render)
		{
			//DRAWINGS
			al_clear_to_color(al_map_rgb(180, 180, 180)); // background color
			button_warrior.DrawButton();
			button_mage.DrawButton();
			button_rogue.DrawButton();
			al_draw_bitmap(war_face, button_warrior._left + 1, button_warrior._upper + 1, 0);
			al_draw_bitmap(mage_face, button_mage._left + 1, button_mage._upper + 1, 0);
			al_draw_bitmap(rogue_face, button_rogue._left + 1, button_rogue._upper + 1, 0);
			render = false;
		}

		ALLEGRO_EVENT menu_ev;
		al_wait_for_event(menu_ev_queue, &menu_ev);
		al_get_mouse_state(&state);

		if (input_name)
		{
			if (!al_ustr_has_suffix_cstr(name, "_"))
				al_ustr_append_cstr(name, "_");

			switch (menu_ev.type)
			{

				/*case ALLEGRO_EVENT_DISPLAY_CLOSE:
					exit_menu = true;
					break;*/
			case ALLEGRO_EVENT_KEY_DOWN:
				if (menu_ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{
					exit_menu = true;
				}
				break;
			case ALLEGRO_EVENT_KEY_CHAR:
				if (menu_ev.keyboard.unichar >= 32)
				{
					if (pos < 20)
					{
						pos += al_ustr_insert_chr(name, name->slen - 1, menu_ev.keyboard.unichar);
					}
					if (pos >= 19)
						nick_length_warning = true;
					else
						nick_length_warning = false;
				}
				else if (menu_ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
				{
					if (al_ustr_prev(name, &pos))
						al_ustr_truncate(name, pos);
				}
				break;
			}
		}

		if (menu_ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (menu_ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				exit_menu = true;
			}
		}

		if (menu_ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (menu_ev.mouse.x > FULL_WIDTH / 2 - 200 &&
				menu_ev.mouse.x < FULL_WIDTH / 2 + 200 &&
				menu_ev.mouse.y > 550 &&
				menu_ev.mouse.y < 620)
				input_name = true;
			else
			{
				input_name = false;
				if (al_ustr_has_suffix_cstr(name, "_"))
					al_ustr_remove_chr(name, pos);
			}

			if (MouseCoursor(menu_ev, button_play))
			{
				if (option != 0 && name->slen > 0)
				{
					exit_menu = true;
					exit = true;
				}
				if (option == 0)
					class_warning = true;
				if (name->slen == 0)
					nick_warning = true;
			}
			else
			{
				class_warning = false;
				nick_warning = false;
			}
			if (MouseCoursor(menu_ev, button_back))
				exit_menu = true;
			if (MouseCoursor(menu_ev, button_warrior))
				option = 1;
			if (MouseCoursor(menu_ev, button_mage))
				option = 2;
			if (MouseCoursor(menu_ev, button_rogue))
				option = 3;
		}

		if (option == 1)
		{
			render = true;
			al_draw_rectangle(button_warrior._left, button_warrior._upper, button_warrior._right, button_warrior._lower, al_map_rgb(255, 0, 0), 5);
		}
			
		if (option == 2)
		{
			render = true;
			al_draw_rectangle(button_mage._left, button_mage._upper, button_mage._right, button_mage._lower, al_map_rgb(255, 0, 0), 5);
		}
		if (option == 3)
		{
			render = true;
			al_draw_rectangle(button_rogue._left, button_rogue._upper, button_rogue._right, button_rogue._lower, al_map_rgb(255, 0, 0), 5);
		}

		if (class_warning && option == 0)
			al_draw_text(c_font48, al_map_rgb(255, 0, 0), FULL_WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER,
			"You must choose a class.");
		if (nick_warning && name->slen == 0)
			al_draw_text(c_font48, al_map_rgb(255, 0, 0), FULL_WIDTH / 2, HEIGHT / 2 + 50, ALLEGRO_ALIGN_CENTER,
			"You must enter a name.");
		if (nick_length_warning)
			al_draw_text(arial24, al_map_rgb(255, 0, 0), FULL_WIDTH / 2 + 230, HEIGHT / 2 + 570, ALLEGRO_ALIGN_CENTER,
			"Maximum 20 characters.");

		al_draw_filled_rounded_rectangle(FULL_WIDTH / 2 - 200, 550, FULL_WIDTH / 2 + 200, 620, 20, 20, al_map_rgb(130, 130, 130)); // name box
		al_draw_rounded_rectangle(FULL_WIDTH / 2 - 200, 550, FULL_WIDTH / 2 + 200, 620, 20, 20, al_map_rgb(90, 90, 90), 4); // name box border

		if (MouseCoursor(menu_ev, button_play))
			button_play.HighlightRoundedButtonNoFlip();
		else
			button_play.DrawRoundedButton();
		if (MouseCoursor(menu_ev, button_back))
			button_back.HighlightRoundedButtonNoFlip();
		else
			button_back.DrawRoundedButton();

		al_draw_ustr(ah_font38, al_map_rgb_f(1, 1, 1), FULL_WIDTH / 2 - 180, 565, ALLEGRO_ALIGN_LEFT, name);
		al_flip_display();
	}

	al_destroy_bitmap(war_face);
	al_destroy_bitmap(mage_face);
	al_destroy_bitmap(rogue_face);
	al_destroy_font(c_font48);
	al_destroy_event_queue(menu_ev_queue);
}
void LoadGameMenu(bool &exit, int &load_option, ALLEGRO_USTR *&name)
{
	ALLEGRO_EVENT_QUEUE *menu_ev_queue = al_create_event_queue();
	ALLEGRO_FONT * crst36 = al_load_font("Contrast.ttf", 36, 0);
	ALLEGRO_FONT * crst40 = al_load_font("Contrast.ttf", 40, 0);

	char *name_slot1 = "empty slot";
	char *name_slot2 = "empty slot";
	char *name_slot3 = "empty slot";

	ReadSlotNames(name_slot1, name_slot2, name_slot3);

	Button slot1_button(FULL_WIDTH / 2 - 260, FULL_WIDTH / 2 + 260, 200, 290, FULL_WIDTH / 2, 220, crst40, name_slot1);
	Button slot2_button(FULL_WIDTH / 2 - 260, FULL_WIDTH / 2 + 260, 300, 390, FULL_WIDTH / 2, 320, crst40, name_slot2);
	Button slot3_button(FULL_WIDTH / 2 - 260, FULL_WIDTH / 2 + 260, 400, 490, FULL_WIDTH / 2, 420, crst40, name_slot3);
	Button back_button(FULL_WIDTH / 2 - 250, FULL_WIDTH / 2 - 50, HEIGHT - 200, HEIGHT - 140, FULL_WIDTH / 2 - 150, HEIGHT - 190, crst36, "Back");

	bool render = true;
	bool exit_menu = false;
	load_option = 0;

	al_register_event_source(menu_ev_queue, al_get_mouse_event_source());
	al_register_event_source(menu_ev_queue, al_get_keyboard_event_source());

	while (!exit_menu)
	{
		if (render)
		{
			render = false;
			al_clear_to_color(al_map_rgb(180, 180, 180)); // background color
			al_draw_text(crst36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, "choose character:");

			slot1_button.DrawButton();
			slot2_button.DrawButton();
			slot3_button.DrawButton();

			back_button.DrawRoundedButton();

			al_flip_display();
		}

		ALLEGRO_EVENT menu_ev;
		al_wait_for_event(menu_ev_queue, &menu_ev);


		// HIGHLIGHT
		if (MouseCoursor(menu_ev, slot1_button))
			slot1_button.HighlightButton(render);
		else if (MouseCoursor(menu_ev, slot2_button))
			slot2_button.HighlightButton(render);
		else if (MouseCoursor(menu_ev, slot3_button))
			slot3_button.HighlightButton(render);
		else if (MouseCoursor(menu_ev, back_button))
			back_button.HighlightRoundedButton(render);
		else
			render = true;

		// PRESS
		if (menu_ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			if (MouseCoursor(menu_ev, back_button))
				exit_menu = true;
			if (MouseCoursor(menu_ev, slot1_button) && strcmp(name_slot1, "empty slot"))
			{
				load_option = 1;
				name = al_ustr_new(name_slot1);
				exit_menu = true;
				exit = true;
			}
			if (MouseCoursor(menu_ev, slot2_button) && strcmp(name_slot2, "empty slot"))
			{
				load_option = 2;
				name = al_ustr_new(name_slot2);
				exit_menu = true;
				exit = true;
			}
			if (MouseCoursor(menu_ev, slot3_button) && strcmp(name_slot3, "empty slot"))
			{
				load_option = 3;
				name = al_ustr_new(name_slot3);
				exit_menu = true;
				exit = true;
			}
		}
	}

	al_destroy_event_queue(menu_ev_queue);
	al_destroy_font(crst36);
}
void EscapeMenu(bool &done, Player *player_ptr)
{
	ALLEGRO_FONT *gd_font36;
	gd_font36 = al_load_font("contrast.ttf", 36, 0);
	ALLEGRO_FONT *gd_font40;
	gd_font40 = al_load_font("contrast.ttf", 40, 0);

	Button continue_button(FULL_WIDTH / 2 - 120, FULL_WIDTH / 2 + 120, HEIGHT / 2 - 180, HEIGHT / 2 - 110, FULL_WIDTH / 2, HEIGHT / 2 - 165, gd_font36, "CONTINUE");
	Button stats_button(FULL_WIDTH / 2 - 120, FULL_WIDTH / 2 + 120, HEIGHT / 2 - 100, HEIGHT / 2 - 30, FULL_WIDTH / 2, HEIGHT / 2 - 85, gd_font40, "STATS");
	Button save_button(FULL_WIDTH / 2 - 120, FULL_WIDTH / 2 + 120, HEIGHT / 2 - 20, HEIGHT / 2 + 60, FULL_WIDTH / 2, HEIGHT / 2 - 5, gd_font40, "SAVE");
	Button exit_button(FULL_WIDTH / 2 - 120, FULL_WIDTH / 2 + 120, HEIGHT / 2 + 150, HEIGHT / 2 + 220, FULL_WIDTH / 2, HEIGHT / 2 + 165, gd_font40, "EXIT");

	ALLEGRO_MOUSE_STATE state;
	ALLEGRO_EVENT_QUEUE *menu_ev_queue = NULL;
	menu_ev_queue = al_create_event_queue();
	al_register_event_source(menu_ev_queue, al_get_keyboard_event_source());
	al_register_event_source(menu_ev_queue, al_get_mouse_event_source());
	bool exit = false;
	bool render = true;

	while (!exit)
	{
		if (render)
		{
			al_draw_filled_rectangle(FULL_WIDTH / 2 - 130, HEIGHT / 2 - 190, FULL_WIDTH / 2 + 130, HEIGHT / 2 + 230, al_map_rgb(180, 180, 180)); // background

			continue_button.DrawButton();
			stats_button.DrawButton();
			save_button.DrawButton();
			exit_button.DrawButton();

			al_flip_display();
			render = false;
		}
		ALLEGRO_EVENT menu_ev;
		al_wait_for_event(menu_ev_queue, &menu_ev);
		al_get_mouse_state(&state);


		// HIGHLIGHT
		if (MouseCoursor(menu_ev, continue_button))
			continue_button.HighlightButton(render);
		else if (MouseCoursor(menu_ev, stats_button))
			stats_button.HighlightButton(render);
		else if (MouseCoursor(menu_ev, save_button))
			save_button.HighlightButton(render);
		else if (MouseCoursor(menu_ev, exit_button))
			exit_button.HighlightButton(render);
		else
			render = true;
		/////////////////////////////////////////

		if (menu_ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (menu_ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				exit = true;
		}
		if (menu_ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			// button 1
			if (MouseCoursor(menu_ev, continue_button))
			{
				exit = true;
			}
			if (MouseCoursor(menu_ev, stats_button))
			{
				StatsMenu(player_ptr, exit);

				al_flush_event_queue(menu_ev_queue);
				render = true;
			}
			if (MouseCoursor(menu_ev, save_button))
			{
				SaveMenu(player_ptr, exit);

				al_flush_event_queue(menu_ev_queue);
				render = true;
			}
			// button 3
			if (MouseCoursor(menu_ev, exit_button))
			{
				exit = true;
				done = true;
			}
		}
	}

	al_destroy_event_queue(menu_ev_queue);
	al_destroy_font(gd_font36);
	al_destroy_font(gd_font40);
}
void StatsMenu(Player *player_ptr, bool &done)
{
	ALLEGRO_FONT *font36 = NULL;
	font36 = al_load_font("contrast.ttf", 36, 0);
	ALLEGRO_FONT *ah_font30 = NULL;
	ah_font30 = al_load_font("ArchitectHand.ttf", 34, 0);

	Button back_button(FULL_WIDTH / 2 - 80, FULL_WIDTH / 2 + 80, HEIGHT / 2 + 150, HEIGHT / 2 + 210, FULL_WIDTH / 2, HEIGHT / 2 + 160,
		font36, "BACK");

	ALLEGRO_EVENT_QUEUE *menu_ev_queue = NULL;
	menu_ev_queue = al_create_event_queue();
	al_register_event_source(menu_ev_queue, al_get_keyboard_event_source());
	al_register_event_source(menu_ev_queue, al_get_mouse_event_source());
	bool exit = false;
	bool render = true;

	while (!exit)
	{
		if (render)
		{
			// DRAWINGS
			al_draw_filled_rectangle(FULL_WIDTH / 2 - 280, HEIGHT / 2 - 190, FULL_WIDTH / 2 + 280, HEIGHT / 2 + 230, al_map_rgb(180, 180, 180));
			al_draw_rectangle(FULL_WIDTH / 2 - 280, HEIGHT / 2 - 190, FULL_WIDTH / 2 + 280, HEIGHT / 2 + 230, al_map_rgb(120, 120, 120), 5);

			// printing static background
			al_draw_text(font36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2 - 50, HEIGHT / 2 - 180, ALLEGRO_ALIGN_LEFT, "HP");
			al_draw_text(font36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2 - 50, HEIGHT / 2 - 140, ALLEGRO_ALIGN_LEFT, "LVL");
			al_draw_text(font36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2 - 50, HEIGHT / 2 - 100, ALLEGRO_ALIGN_LEFT, "EXP");
			al_draw_text(font36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2 - 260, HEIGHT / 2 - 50, ALLEGRO_ALIGN_LEFT, "Strength");
			al_draw_text(font36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2 - 260, HEIGHT / 2 - 15, ALLEGRO_ALIGN_LEFT, "Agility");
			al_draw_text(font36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2 - 260, HEIGHT / 2 + 20, ALLEGRO_ALIGN_LEFT, "Stamina");
			al_draw_text(font36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2 - 260, HEIGHT / 2 + 55, ALLEGRO_ALIGN_LEFT, "Intelligence");
			al_draw_text(font36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2 - 260, HEIGHT / 2 + 90, ALLEGRO_ALIGN_LEFT, "Spirit");
			al_draw_text(font36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2 + 60, HEIGHT / 2 - 50, ALLEGRO_ALIGN_LEFT, "ATK");
			al_draw_text(font36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2 + 60, HEIGHT / 2 - 15, ALLEGRO_ALIGN_LEFT, "CRIT");
			al_draw_text(font36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2 + 60, HEIGHT / 2 + 20, ALLEGRO_ALIGN_LEFT, "DEF");
			al_draw_text(font36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2 + 60, HEIGHT / 2 + 55, ALLEGRO_ALIGN_LEFT, "HST");
			al_draw_text(font36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2 + 60, HEIGHT / 2 + 90, ALLEGRO_ALIGN_LEFT, "SPD");

			player_ptr->DrawStats(ah_font30);				// printing numbers

			back_button.DrawRoundedButton();

			al_flip_display();
			render = false;
		}

		ALLEGRO_EVENT menu_ev;
		al_wait_for_event(menu_ev_queue, &menu_ev);


		if (MouseCoursor(menu_ev, back_button))
		{
			back_button.HighlightRoundedButton(render);
			render = false;
		}
		else
			render = true;

		if (menu_ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (menu_ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				exit = true;
				done = true;
			}
		}
		if (menu_ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (MouseCoursor(menu_ev, back_button))
			{
				exit = true;
				done = true;
			}
		}

	}
	al_destroy_font(font36);
	al_destroy_font(ah_font30);
}
void SaveMenu(Player *player_ptr, bool &exit)
{
	ALLEGRO_EVENT_QUEUE * menu_ev_queue = al_create_event_queue();
	al_register_event_source(menu_ev_queue, al_get_mouse_event_source());

	ALLEGRO_FONT * crst36 = al_load_font("contrast.ttf", 36, 0);
	ALLEGRO_FONT * crst40 = al_load_font("contrast.ttf", 40, 0);
	ALLEGRO_FONT * crst50 = al_load_font("contrast.ttf", 50, 0);

	char *name_slot1 = "empty slot";
	char *name_slot2 = "empty slot";
	char *name_slot3 = "empty slot";

	ReadSlotNames(name_slot1, name_slot2, name_slot3);

	char * cur_name = new char[player_ptr->name.length() + 1];
	strcpy(cur_name, player_ptr->name.c_str());

	Button slot1_button(FULL_WIDTH / 2 - 260, FULL_WIDTH / 2 + 260, 200, 290, FULL_WIDTH / 2, 220, crst40, name_slot1);
	Button slot2_button(FULL_WIDTH / 2 - 260, FULL_WIDTH / 2 + 260, 300, 390, FULL_WIDTH / 2, 320, crst40, name_slot2);
	Button slot3_button(FULL_WIDTH / 2 - 260, FULL_WIDTH / 2 + 260, 400, 490, FULL_WIDTH / 2, 420, crst40, name_slot3);
	Button back_button(FULL_WIDTH / 2 - 250, FULL_WIDTH / 2 - 50, HEIGHT - 200, HEIGHT - 140, FULL_WIDTH / 2 - 150, HEIGHT - 190, crst36, "Back");

	bool exit_menu = false;
	bool render = true;
	int save_option = 0;

	while (!exit_menu)
	{
		if (render)
		{
			render = false;
			al_clear_to_color(al_map_rgb(180, 180, 180));
			al_draw_text(crst50, al_map_rgb(100, 100, 100), FULL_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTER, "SAVE");
			al_draw_text(crst36, al_map_rgb(100, 100, 100), FULL_WIDTH / 2, 100, ALLEGRO_ALIGN_CENTER, "choose slot:");

			slot1_button.DrawButton();
			slot2_button.DrawButton();
			slot3_button.DrawButton();

			back_button.DrawRoundedButton();

			al_flip_display();
		}

		ALLEGRO_EVENT menu_ev;
		al_wait_for_event(menu_ev_queue, &menu_ev);

		// HIGHLIGHT
		if (MouseCoursor(menu_ev, slot1_button))
			slot1_button.HighlightButton(render);
		else if (MouseCoursor(menu_ev, slot2_button))
			slot2_button.HighlightButton(render);
		else if (MouseCoursor(menu_ev, slot3_button))
			slot3_button.HighlightButton(render);
		else if (MouseCoursor(menu_ev, back_button))
			back_button.HighlightRoundedButton(render);
		else
			render = true;

		// PRESS
		if (menu_ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			if (MouseCoursor(menu_ev, back_button))
			{
				exit_menu = true;
				exit = true;
			}
			if (MouseCoursor(menu_ev, slot1_button))
			{
				save_option = 1;
				SaveSlotNames(cur_name, name_slot2, name_slot3);
				// player_ptr->SaveCharactersData(save_option, FileToChar("CharactersData.xml"));
				exit_menu = true;
				exit = true;
			}
			if (MouseCoursor(menu_ev, slot2_button))
			{
				save_option = 2;
				SaveSlotNames(name_slot1, cur_name, name_slot3);
				// SaveCharactersData();
				exit_menu = true;
				exit = true;
			}
			if (MouseCoursor(menu_ev, slot3_button))
			{
				save_option = 3;
				SaveSlotNames(name_slot1, name_slot2, cur_name);
				// SaveCharactersData();
				exit_menu = true;
				exit = true;
			}
		}
	}
	al_destroy_font(crst36);
	al_destroy_font(crst40);
	al_destroy_font(crst50);
}

bool MouseCoursor(ALLEGRO_EVENT menu_ev, Button button)
{
	if (menu_ev.mouse.x > button._left &&
		menu_ev.mouse.x < button._right &&
		menu_ev.mouse.y > button._upper &&
		menu_ev.mouse.y < button._lower)
		return true;
	else
		return false;
}

void destroyMonster(Monster &monst)
{
	al_destroy_bitmap(monst.m_Back);
	al_destroy_bitmap(monst.m_Front);
	al_destroy_bitmap(monst.m_Left);
	al_destroy_bitmap(monst.m_Right);
	al_destroy_bitmap(monst.m_dead);
	for (int i = 0; i < 2; i++)
	{
		al_destroy_bitmap(monst.m_WalkBack[i]);
		al_destroy_bitmap(monst.m_WalkFront[i]);
		al_destroy_bitmap(monst.m_WalkLeft[i]);
		al_destroy_bitmap(monst.m_WalkRight[i]);
	}
}
void initWolf(Wolf &wolf)
{
	wolf.m_Front = al_load_bitmap("graphics/wolf_front.png");
	wolf.m_Back = al_load_bitmap("graphics/wolf_back.png");
	wolf.m_Left = al_load_bitmap("graphics/wolf_left.png");
	wolf.m_Right = al_load_bitmap("graphics/wolf_right.png");

	wolf.m_WalkFront[0] = al_load_bitmap("graphics/wolf_front_walk1.png");
	wolf.m_WalkFront[1] = al_load_bitmap("graphics/wolf_front_walk2.png");
	wolf.m_WalkBack[0] = al_load_bitmap("graphics/wolf_back_walk1.png");
	wolf.m_WalkBack[1] = al_load_bitmap("graphics/wolf_back_walk2.png");
	wolf.m_WalkLeft[0] = al_load_bitmap("graphics/wolf_left_walk1.png");
	wolf.m_WalkLeft[1] = al_load_bitmap("graphics/wolf_left_walk2.png");
	wolf.m_WalkRight[0] = al_load_bitmap("graphics/wolf_right_walk1.png");
	wolf.m_WalkRight[1] = al_load_bitmap("graphics/wolf_right_walk2.png");
	wolf.m_dead = al_load_bitmap("graphics/wolf_dead.png");
}
void initSnake(Snake &snake)
{
	snake.m_Front = al_load_bitmap("graphics/snake_front1.png");
	snake.m_Back = al_load_bitmap("graphics/snake_back1.png");
	snake.m_Left = al_load_bitmap("graphics/snake_left1.png");
	snake.m_Right = al_load_bitmap("graphics/snake_right1.png");

	snake.m_WalkFront[0] = snake.m_Front;
	snake.m_WalkFront[1] = al_load_bitmap("graphics/snake_front2.png");
	snake.m_WalkBack[0] = snake.m_Back;
	snake.m_WalkBack[1] = al_load_bitmap("graphics/snake_back2.png");
	snake.m_WalkLeft[0] = snake.m_Left;
	snake.m_WalkLeft[1] = al_load_bitmap("graphics/snake_left2.png");
	snake.m_WalkRight[0] = snake.m_Right;
	snake.m_WalkRight[1] = al_load_bitmap("graphics/snake_right2.png");
	snake.m_dead = al_load_bitmap("graphics/snake_dead.png");
}
void MoveMonster(Monster &monster, Player *player_ptr)
{
	monster.prevPosX = monster.m_PosX;
	monster.prevPosY = monster.m_PosY;


	player_ptr->cur_speed = abs(player_ptr->m_PosX - player_ptr->prevPosX + player_ptr->m_PosY - player_ptr->prevPosY);

	monster.m_PosX -= walk[RIGHT] * player_ptr->cur_speed;
	monster.m_PosX += walk[LEFT] * player_ptr->cur_speed;
	monster.m_PosY += walk[UP] * player_ptr->cur_speed;
	monster.m_PosY -= walk[DOWN] * player_ptr->cur_speed;
}
void WalkCountMonster(Monster &monster, Player *player_ptr, int count)
{
	if (!monster.wounded && (count % monster.count == 0))
	{
		if (monster.monster_walk[0] ||
			monster.monster_walk[1] ||
			monster.monster_walk[2] ||
			monster.monster_walk[3])
		{
			for (int i = 0; i < 4; i++)
				monster.monster_walk[i] = false;
		}
		else
		{
			random_device rd;
			mt19937 rng(rd());
			uniform_int_distribution<int> uni(0, 3);
			int j = uni(rng);

			monster.monster_walk[j] = true;

			//monster.monster_walk[LEFT] = true;
		}
	}

	monster.WalkMonster();

	if (MonsterCollision(monster, player_ptr))
	{
		monster.m_PosX = monster.prevPosX;
		monster.m_PosY = monster.prevPosY;
		MoveMonster(monster, player_ptr);
	}
}
void DrawStandingMonster(Monster &monster)
{
	if (!(monster.monster_walk[0] ||
		monster.monster_walk[1] ||
		monster.monster_walk[2] ||
		monster.monster_walk[3]))
	{
		al_draw_bitmap(monster.m_Front, monster.m_PosX, monster.m_PosY, 0);
	}

}
void DrawWalkingMonster(Monster &monster, int curFrame)
{
	if (monster.monster_walk[DOWN])
		al_draw_bitmap(monster.m_WalkBack[curFrame], monster.m_PosX, monster.m_PosY, 0);
	if (monster.monster_walk[UP])
		al_draw_bitmap(monster.m_WalkFront[curFrame], monster.m_PosX, monster.m_PosY, 0);
	if (monster.monster_walk[LEFT])
		al_draw_bitmap(monster.m_WalkRight[curFrame], monster.m_PosX, monster.m_PosY, 0);
	if (monster.monster_walk[RIGHT])
		al_draw_bitmap(monster.m_WalkLeft[curFrame], monster.m_PosX, monster.m_PosY, 0);
}
void DrawDeadMonster(Monster monster)
{
	al_draw_bitmap(monster.m_dead, monster.m_PosX, monster.m_PosY, 0);
}
bool MonsterCollision(Monster monster, Player *player_ptr)
{
	BLKSTR * blkdatapt;
	BLKSTR * blkdatapt1;
	BLKSTR * blkdatapt2;
	BLKSTR * blkdatapt3;

	// Collision with terrain
	blkdatapt = MapGetBlock((player_ptr->m_PosX + monster.m_PosX) / mapblockheight, (player_ptr->m_PosY + monster.m_PosY) / mapblockheight);
	blkdatapt1 = MapGetBlock((player_ptr->m_PosX + monster.m_PosX + monster.imageWidth) / mapblockheight, (player_ptr->m_PosY + monster.m_PosY) / mapblockheight);
	blkdatapt2 = MapGetBlock((player_ptr->m_PosX + monster.m_PosX) / mapblockheight, (player_ptr->m_PosY + monster.m_PosY + monster.imageHeight) / mapblockheight);
	blkdatapt3 = MapGetBlock((player_ptr->m_PosX + monster.m_PosX + monster.imageWidth) / mapblockheight, (player_ptr->m_PosY + monster.m_PosY + monster.imageHeight) / mapblockheight);


	// Collision with player
	// monster goes up
	if (monster.monster_walk[DOWN] &&
		((monster.m_PosX > WIDTH / 2 - player_ptr->imageWidth / 2.0  && monster.m_PosX < WIDTH / 2 + player_ptr->imageWidth / 2.0) ||
		(monster.m_PosX + monster.imageWidth > WIDTH / 2 - player_ptr->imageWidth / 2.0 && monster.m_PosX + monster.imageWidth < WIDTH / 2 + player_ptr->imageWidth / 2.0)) &&
		monster.m_PosY < HEIGHT / 2 + player_ptr->imageHeight / 2.0 &&
		monster.m_PosY > HEIGHT / 2 - player_ptr->imageHeight / 2.0)
		return 1;
	// monster goes up
	else if (monster.monster_walk[UP] &&
		((monster.m_PosX > WIDTH / 2 - player_ptr->imageWidth / 2.0  && monster.m_PosX < WIDTH / 2 + player_ptr->imageWidth / 2.0) ||
		(monster.m_PosX + monster.imageWidth > WIDTH / 2 - player_ptr->imageWidth / 2.0 && monster.m_PosX + monster.imageWidth < WIDTH / 2 + player_ptr->imageWidth / 2.0)) &&
		monster.m_PosY + monster.imageHeight < HEIGHT / 2 + player_ptr->imageHeight / 2.0 &&
		monster.m_PosY + monster.imageHeight > HEIGHT / 2 - player_ptr->imageHeight / 2.0)
		return 1;
	// monster goes right
	else if (monster.monster_walk[LEFT] &&
		monster.m_PosX + monster.imageWidth < WIDTH / 2 + player_ptr->imageWidth / 2.0 &&
		monster.m_PosX + monster.imageWidth > WIDTH / 2 - player_ptr->imageWidth / 2.0 &&
		((monster.m_PosY > HEIGHT / 2 - player_ptr->imageHeight / 2.0 && monster.m_PosY < HEIGHT / 2 + player_ptr->imageHeight / 2.0) ||
		(monster.m_PosY + monster.imageHeight > HEIGHT / 2 - player_ptr->imageHeight / 2.0 && monster.m_PosY + 60 < HEIGHT / 2 + player_ptr->imageHeight / 2.0)))
		return 1;
	// monster goes left
	else if (monster.monster_walk[RIGHT] &&
		monster.m_PosX < WIDTH / 2 + player_ptr->imageWidth / 2.0 &&
		monster.m_PosX > WIDTH / 2 - player_ptr->imageWidth / 2.0 &&
		((monster.m_PosY > HEIGHT / 2 - player_ptr->imageHeight / 2.0 && monster.m_PosY < HEIGHT / 2 + player_ptr->imageHeight / 2.0) ||
		(monster.m_PosY + monster.imageHeight > HEIGHT / 2 - player_ptr->imageHeight / 2.0 && monster.m_PosY + monster.imageHeight < HEIGHT / 2 + player_ptr->imageHeight / 2.0)))
		return 1;

	// return collision with terrain
	if (blkdatapt->tl || blkdatapt1->tl || blkdatapt2->tl || blkdatapt3->tl)
		return 1;
	else
		return 0;
}
void DrawMonsterHP(Monster &monster, Player *&player_ptr)
{
	double hp_coefficient = double(monster.cur_hp) / double(monster.max_hp);

	ALLEGRO_FONT *font;
	font = al_load_font("contrast.ttf", 14, 0);
	al_draw_text(font, al_map_rgb(255, 255, 255), monster.m_PosX + 15, monster.m_PosY - 25, ALLEGRO_ALIGN_CENTER, monster.m_name);
	al_draw_filled_rectangle(monster.m_PosX, monster.m_PosY - 7, monster.m_PosX + 30, monster.m_PosY - 3, al_map_rgb(60, 60, 60));
	if (hp_coefficient > 0.7)
		al_draw_filled_rectangle(monster.m_PosX, monster.m_PosY - 7, monster.m_PosX + hp_coefficient * 30, monster.m_PosY - 3, al_map_rgb(0, 250, 0));
	else if (hp_coefficient <= 0.7 && hp_coefficient > 0.3)
		al_draw_filled_rectangle(monster.m_PosX, monster.m_PosY - 7, monster.m_PosX + hp_coefficient * 30, monster.m_PosY - 3, al_map_rgb(250, 250, 0));
	else	// (hp_coefficient <= 0.3 && hp_coefficient > 0)
		al_draw_filled_rectangle(monster.m_PosX, monster.m_PosY - 7, monster.m_PosX + hp_coefficient * 30, monster.m_PosY - 3, al_map_rgb(250, 0, 0));


	al_destroy_font(font);
}
int TargetMonster(Monster monster, float mouse_x, float mouse_y)
{
	if (mouse_x > monster.m_PosX &&
		mouse_x < monster.m_PosX + 60 &&
		mouse_y > monster.m_PosY &&
		mouse_y < monster.m_PosY + 60)
		return 1;
	else
		return 0;
}
void DrawTargetSquare(Monster monster)
{
	al_draw_rectangle(monster.m_PosX - 5, monster.m_PosY - 5, monster.m_PosX + monster.imageWidth + 5, monster.m_PosY + monster.imageHeight + 5, al_map_rgb(250, 30, 0), 3);
}
void WalkToDirection()
{
	if (walk[DOWN])
		direction[DOWN] = true;
	else if (walk[UP])
		direction[UP] = true;
	else if (walk[LEFT])
		direction[LEFT] = true;
	else if (walk[RIGHT])
		direction[RIGHT] = true;
	else
		return;
}
void MoveBolt(float &boltX, float &boltY, Monster monster, bool &bolt)
{
	// float a = (boltY - monster.m_PosY) / (boltX - monster.m_PosX);
	// float b = boltY - boltX * a;
	float x = 0, y = 0;

	x = 0.25 * (monster.m_PosX + monster.imageWidth / 2 - WIDTH / 2);
	y = 0.25 * (monster.m_PosY + monster.imageHeight / 2 - HEIGHT / 2);

	boltX += x;
	boltY += y;

	if ((x <= 0 && y <= 0 && (boltX <= monster.m_PosX || boltY <= monster.m_PosY)) ||
		(x >= 0 && y <= 0 && (boltX >= monster.m_PosX || boltY <= monster.m_PosY)) || 
		(x <= 0 && y >= 0 && (boltX <= monster.m_PosX || boltY >= monster.m_PosY)) ||
		(x >= 0 && y >= 0 && (boltX >= monster.m_PosX || boltY >= monster.m_PosY)))
	{
		boltX = WIDTH / 2;
		boltY = HEIGHT / 2;
		bolt = false;
	}

}
int PrintDeathMessage()
{
	ALLEGRO_FONT * crst_font36 = al_load_font("contrast.ttf", 36, 0);

	Button revive_here_button(FULL_WIDTH / 2 - 100, FULL_WIDTH / 2 + 100, HEIGHT / 2, HEIGHT / 2 + 70, FULL_WIDTH / 2, HEIGHT / 2 + 15, crst_font36, "Revive");
	Button main_menu_button(FULL_WIDTH / 2 - 100, FULL_WIDTH / 2 + 100, HEIGHT / 2 + 85, HEIGHT / 2 + 155, FULL_WIDTH / 2, HEIGHT / 2 + 100, crst_font36, "Main menu");

	ALLEGRO_MOUSE_STATE state;
	ALLEGRO_EVENT_QUEUE *menu_ev_queue = NULL;
	menu_ev_queue = al_create_event_queue();
	al_register_event_source(menu_ev_queue, al_get_keyboard_event_source());
	al_register_event_source(menu_ev_queue, al_get_mouse_event_source());
	bool render = true;

	while (1)
	{
		if (render)
		{
			render = false;
			al_draw_filled_rectangle(FULL_WIDTH / 2 - 300, HEIGHT / 2 - 200, FULL_WIDTH / 2 + 300, HEIGHT / 2 + 200, al_map_rgb(220, 0, 0));
			al_draw_rectangle(FULL_WIDTH / 2 - 300, HEIGHT / 2 - 200, FULL_WIDTH / 2 + 300, HEIGHT / 2 + 200, al_map_rgb(200, 30, 30), 5);
			al_draw_text(crst_font36, al_map_rgb(0, 0, 0), FULL_WIDTH / 2, HEIGHT / 2 - 100, ALLEGRO_ALIGN_CENTER,
				"You have died.");
			revive_here_button.DrawButton();
			main_menu_button.DrawButton();


			al_flip_display();
		}

		ALLEGRO_EVENT menu_ev;
		al_wait_for_event(menu_ev_queue, &menu_ev);
		al_get_mouse_state(&state);

		if (MouseCoursor(menu_ev, revive_here_button))
			revive_here_button.HighlightButton(render);
		else if (MouseCoursor(menu_ev, main_menu_button))
			main_menu_button.HighlightButton(render);
		else
			render = true;

		if (menu_ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (menu_ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				al_destroy_event_queue(menu_ev_queue);
				al_destroy_font(crst_font36);
				return 0;
				break;
			}
		}
		if (state.buttons & 1)
		{
			if (MouseCoursor(menu_ev, revive_here_button))
			{
				al_destroy_event_queue(menu_ev_queue);
				al_destroy_font(crst_font36);
				return 1;
				break;
			}
			if (MouseCoursor(menu_ev, main_menu_button))
			{
				al_destroy_event_queue(menu_ev_queue);
				al_destroy_font(crst_font36);
				return 0;
				break;
			}
		}
	}
}
void RespawnMonster(Monster &monster, Player *player_ptr)
{
	monster.cur_hp = monster.max_hp;
	monster.alive = true;
	monster.wounded = false;

	int minX = 0;
	int maxX = mapwidth * mapblockwidth - WIDTH / 2;
	int minY = 0;
	int maxY = mapheight * mapblockheight - HEIGHT / 2;

	if (player_ptr->m_PosX + WIDTH / 2 - 1000 > minX)
		minX = WIDTH / 2 - 1000;
	if (player_ptr->m_PosX + WIDTH / 2 + 1000 < maxX)
		maxX = WIDTH / 2 + 1000;

	if (player_ptr->m_PosY + HEIGHT / 2 - 800 > minY)
		minY = HEIGHT / 2 - 800;
	if (player_ptr->m_PosY + HEIGHT / 2 + 800 < maxY)
		maxY = HEIGHT / 2 + 800;

	do
	{
		random_device rd;     // only used once to initialise (seed) engine
		mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		uniform_int_distribution<int> uni(minX, maxX); // guaranteed unbiased
		monster.m_PosX = uni(rng);

		random_device rd1;     // only used once to initialise (seed) engine
		mt19937 rng1(rd1());    // random-number engine used (Mersenne-Twister in this case)
		uniform_int_distribution<int> uni1(minY, maxY); // guaranteed unbiased
		monster.m_PosY = uni1(rng1);
	} while (MonsterCollision(monster, player_ptr));
}
void ReadSlotNames(char *&name_slot1, char *&name_slot2, char *&name_slot3)
{
	string slot1;
	string slot2;
	string slot3;

	ifstream file("Characters.txt");
	if (file.bad())
		return;
	else
	{
		file.seekg(0);
		getline(file, slot1);
		getline(file, slot2);
		getline(file, slot3);

		if (slot1.compare(""))
		{
			name_slot1 = new char[slot1.length() + 1];
			strcpy(name_slot1, slot1.c_str());
		}
		if (slot2.compare(""))
		{
			name_slot2 = new char[slot2.length() + 1];
			strcpy(name_slot2, slot2.c_str());
		}
		if (slot3.compare(""))
		{
			name_slot3 = new char[slot3.length() + 1];
			strcpy(name_slot3, slot3.c_str());
		}

	}

	file.close();
}
char * FileToChar(char * file_name)
{
	ifstream file(file_name);
	if (file.bad())
	{
		cout << "The file doesn't exist!" << endl;
		exit(-1);
	}

	filebuf * pbuf = file.rdbuf();
	long size_of_file = pbuf->pubseekoff(0, ios::end, ios::in);
	file.seekg(0);

	char * output = new char[size_of_file + 1];

	file.read(output, size_of_file);

	return output;
}
int ReadClassID(char * file_content, int load_option)
{
	char *slot;
	if (load_option == 1)
		slot = "Slot1";
	else if (load_option == 2)
		slot = "Slot2";
	else
		slot = "Slot3";

	auto_ptr < char > zawartosc_pliku(file_content);

	xml_document <> doc;
	try {
		doc.parse < 0 >(zawartosc_pliku.get());
	}
	catch (parse_error p) {
		p.what();
	}
	xml_node <> *player_node = doc.first_node();
	for (xml_node <> *inner_node = player_node->first_node(); inner_node; inner_node = inner_node->next_sibling())
	{
		if (!strcmp(inner_node->name(), slot))
			for (xml_node <> *more_inner_node = inner_node->first_node(); more_inner_node; more_inner_node = more_inner_node->next_sibling())
				if (!strcmp(more_inner_node->name(), "class"))
					return atoi(more_inner_node->value());
	}
}
void SaveSlotNames(char *name_slot1, char *name_slot2, char *name_slot3)
{
	ofstream file("Characters.txt");

	if (file.bad())
		return;
	else
	{
		file << name_slot1 << endl;
		file << name_slot2 << endl;
		file << name_slot3 << endl;
	}

	file.close();
}