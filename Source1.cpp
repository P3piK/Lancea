#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "mappy_A5.h"

#define WIDTH 1024
#define HEIGHT 768
#define imageWidth 39
#define imageHeight 61

enum KEYS { DOWN, UP, LEFT, RIGHT };
bool keys[4] = { true, false, false, false };
bool walk[4] = { false, false, false, false };

class Player
{
public:
	ALLEGRO_BITMAP *Front, *Back, *Left, *Right = NULL;
	ALLEGRO_BITMAP *WalkFront[2], *WalkBack[2], *WalkLeft[2], *WalkRight[2];

	Player();
};
Player::Player()
{
}

void initStandingPlayer(Player player);
void initWalkingPlayer(Player player);
void initWalkingPlayer(ALLEGRO_BITMAP *mageWalkFront[2], ALLEGRO_BITMAP *mageWalkBack[2], ALLEGRO_BITMAP *mageWalkLeft[2], ALLEGRO_BITMAP *mageWalkRight[2]);
void DrawStandingCharacter(ALLEGRO_BITMAP *charFront, ALLEGRO_BITMAP *charBack, ALLEGRO_BITMAP *charLeft, ALLEGRO_BITMAP *charRight);
void DrawWalkingCharacter(ALLEGRO_BITMAP *walkFront, ALLEGRO_BITMAP *walkBack, ALLEGRO_BITMAP *walkLeft, ALLEGRO_BITMAP *walkRight);
void SetWalkFalse();
void SetStandFalse();
int Collision(float x, float y);
int OnPath(float x, float y);


int main()
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;



	bool done = false;
	bool render = false;

	const int maxFrame = 2;
	int curFrame = 0;
	int frameCount = 0;
	int frameDelay = 20;

	float magePosX = 300;
	float magePosY = 100;
	float prevPosX = magePosX;
	float prevPosY = magePosY;

	Player player();
	void initStandingPlayer(&player);
	void initWalkingPlayer(&player);


	// INITIALIZATION
	if (!al_init())
		return -1;

	display = al_create_display(WIDTH, HEIGHT);

	if (!display)
		return -1;

	al_install_keyboard();
	al_init_image_addon();

	if (MapLoad("bg1.FMP", 1))
		return -5;






	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_DOWN:
				walk[DOWN] = true;
				walk[UP] = false;
				walk[LEFT] = false;
				walk[RIGHT] = false;
				SetStandFalse();
				break;
			case ALLEGRO_KEY_UP:
				walk[DOWN] = false;
				walk[UP] = true;
				walk[LEFT] = false;
				walk[RIGHT] = false;
				SetStandFalse();
				break;
			case ALLEGRO_KEY_LEFT:
				walk[DOWN] = false;
				walk[UP] = false;
				walk[LEFT] = true;
				walk[RIGHT] = false;
				SetStandFalse();
				break;
			case ALLEGRO_KEY_RIGHT:
				walk[DOWN] = false;
				walk[UP] = false;
				walk[LEFT] = false;
				walk[RIGHT] = true;
				SetStandFalse();
				break;
			}
		}

		if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				keys[UP] = false;
				keys[LEFT] = false;
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[DOWN] = false;
				keys[UP] = true;
				keys[LEFT] = false;
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[DOWN] = false;
				keys[UP] = false;
				keys[LEFT] = true;
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[DOWN] = false;
				keys[UP] = false;
				keys[LEFT] = false;
				keys[RIGHT] = true;
				break;
			}

			SetWalkFalse();
			curFrame = 0;
			frameCount = 0;
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (++frameCount >= frameDelay)
			{
				if (++curFrame >= maxFrame)
					curFrame = 0;

				frameCount = 0;
			}

			prevPosX = magePosX;
			prevPosY = magePosY;
			
			if (OnPath(magePosX, magePosY))
			{
				magePosX += walk[RIGHT] * 2;
				magePosX -= walk[LEFT] * 2;
				magePosY -= walk[UP] * 2;
				magePosY += walk[DOWN] * 2;
			}
			else
			{
				magePosX += walk[RIGHT] * 1.5;
				magePosX -= walk[LEFT] * 1.5;
				magePosY -= walk[UP] * 1.5;
				magePosY += walk[DOWN] * 1.5;
			}

			if (Collision(magePosX, magePosY))
			{
				magePosX = prevPosX;
				magePosY = prevPosY;
			}

			if (magePosX < 0)
				magePosX = 0;
			if (magePosY < 0)
				magePosY = 0;
			if (magePosX > (mapwidth * mapblockwidth) - WIDTH)
				magePosX = (mapwidth * mapblockwidth) - WIDTH;
			if (magePosY >(mapheight * mapblockheight) - HEIGHT)
				magePosY = (mapheight * mapblockheight) - HEIGHT;

			render = true;
		}

		if (render && al_is_event_queue_empty(event_queue))
		{
			render = false;

			MapChangeLayer(0);
			MapDrawBG(magePosX, magePosY, 0, 0, WIDTH, HEIGHT);
			/*if (magePosX > 80 &&
				magePosX < 400 &&
				magePosY > 80 &&
				magePosY < 400)
			{
				MapChangeLayer(1);
				MapDrawBG(magePosX, magePosY, 0, 0, WIDTH, HEIGHT);
			}*/

			DrawStandingCharacter(mageFront, mageBack, mageLeft, mageRight);
			DrawWalkingCharacter(mageWalkFront[curFrame], mageWalkBack[curFrame], mageWalkLeft[curFrame], mageWalkRight[curFrame]);

			al_flip_display();
			al_clear_to_color(al_map_rgb(255, 255, 0));
		}

	}

	for (int i = 0; i < maxFrame; i++)
	{
		al_destroy_bitmap(mageWalkFront[i]);
		al_destroy_bitmap(mageWalkBack[i]);
		al_destroy_bitmap(mageWalkLeft[i]);
		al_destroy_bitmap(mageWalkRight[i]);
	}
	MapFreeMem();
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_bitmap(mageFront);

	return 0;
}

void initStandingPlayer(Player player)
{
	player.Front = al_load_bitmap("mag_front.png");
	player.Back = al_load_bitmap("mag_back.png");
	player.Left = al_load_bitmap("mag_left.png");
	player.Right = al_load_bitmap("mag_right.png");
}
void initWalkingPlayer(Player player)
{
	player.WalkFront[0] = al_load_bitmap("mag_front_walk1.png");
	player.WalkFront[1] = al_load_bitmap("mag_front_walk2.png");
	player.WalkBack[0] = al_load_bitmap("mag_back_walk1.png");
	player.WalkBack[1] = al_load_bitmap("mag_back_walk2.png");
	player.WalkLeft[0] = al_load_bitmap("mag_left_walk1.png");
	player.WalkLeft[1] = al_load_bitmap("mag_left_walk2.png");
	player.WalkRight[0] = al_load_bitmap("mag_right_walk1.png");
	player.WalkRight[1] = al_load_bitmap("mag_right_walk2.png");
}
void DrawStandingCharacter(ALLEGRO_BITMAP *charFront, ALLEGRO_BITMAP *charBack, ALLEGRO_BITMAP *charLeft, ALLEGRO_BITMAP *charRight)
{
	if (keys[DOWN])
		al_draw_bitmap(charFront, WIDTH / 2 - imageWidth / 2, HEIGHT / 2 - imageHeight / 2, 0);
	if (keys[UP])
		al_draw_bitmap(charBack, WIDTH / 2 - imageWidth / 2, HEIGHT / 2 - imageHeight / 2, 0);
	if (keys[LEFT])
		al_draw_bitmap(charLeft, WIDTH / 2 - imageWidth / 2, HEIGHT / 2 - imageHeight / 2, 0);
	if (keys[RIGHT])
		al_draw_bitmap(charRight, WIDTH / 2 - imageWidth / 2, HEIGHT / 2 - imageHeight / 2, 0);

}
void DrawWalkingCharacter(ALLEGRO_BITMAP *walkFront, ALLEGRO_BITMAP *walkBack, ALLEGRO_BITMAP *walkLeft, ALLEGRO_BITMAP *walkRight)
{
	if (walk[DOWN])
		al_draw_bitmap(walkFront, WIDTH / 2 - imageWidth / 2, HEIGHT / 2 - imageHeight / 2, 0);
	if (walk[UP])
		al_draw_bitmap(walkBack, WIDTH / 2 - imageWidth / 2, HEIGHT / 2 - imageHeight / 2, 0);
	if (walk[LEFT])
		al_draw_bitmap(walkLeft, WIDTH / 2 - imageWidth / 2, HEIGHT / 2 - imageHeight / 2, 0);
	if (walk[RIGHT])
		al_draw_bitmap(walkRight, WIDTH / 2 - imageWidth / 2, HEIGHT / 2 - imageHeight / 2, 0);
}
void SetWalkFalse()
{
	for (int i = 0; i < 4; i++)
		walk[i] = false;
}
void SetStandFalse()
{
	for (int i = 0; i < 4; i++)
		keys[i] = false;
}
int Collision(float x, float y)
{
	BLKSTR * blkdatapt;
	BLKSTR * blkdatapt1;

	blkdatapt = MapGetBlock((x + WIDTH / 2 - imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2) / mapblockheight);
	blkdatapt1 = MapGetBlock((x + WIDTH / 2 - imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2) / mapblockheight);

	if (walk[DOWN])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 60) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 - imageWidth / 2 + 30) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 60) / mapblockheight);
	}
	if (walk[UP])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 4) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 - imageWidth / 2 + 26) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 4) / mapblockheight);
	}
	if (walk[LEFT])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 4) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 - imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 60) / mapblockheight);
	}
	if (walk[RIGHT])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - imageWidth / 2 + 30) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 4) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 - imageWidth / 2 + 30) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 60) / mapblockheight);
	}

	if (blkdatapt->tl || blkdatapt1->tl)
		return 1; 
	else 
		return 0;
}
int OnPath(float x, float y)
{
	BLKSTR * blkdatapt;
	BLKSTR * blkdatapt1;

	blkdatapt = MapGetBlock((x + WIDTH / 2 - imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2) / mapblockheight);
	blkdatapt1 = MapGetBlock((x + WIDTH / 2 - imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2) / mapblockheight);

	if (walk[DOWN])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 60) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 - imageWidth / 2 + 30) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 60) / mapblockheight);
	}
	if (walk[UP])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 4) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 - imageWidth / 2 + 26) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 4) / mapblockheight);
	}
	if (walk[LEFT])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 4) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 - imageWidth / 2) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 60) / mapblockheight);
	}
	if (walk[RIGHT])
	{
		blkdatapt = MapGetBlock((x + WIDTH / 2 - imageWidth / 2 + 30) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 4) / mapblockheight);
		blkdatapt1 = MapGetBlock((x + WIDTH / 2 - imageWidth / 2 + 30) / mapblockheight, (y + HEIGHT / 2 - imageHeight / 2 + 60) / mapblockheight);
	}

	if (blkdatapt->tr || blkdatapt1->tr)
		return 1;
	else
		return 0;
}