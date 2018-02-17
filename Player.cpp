#include "Player.h"



Player::Player()
{
	m_strength = 10;
	m_agility = 10;
	m_stamina = 10;
	m_intelligence = 10;
	m_spirit = 10;
}

void Player::DrawEq()
{
	if (m_weapon.m_weapon_bitmap)
		al_draw_bitmap(m_weapon.m_weapon_bitmap, WIDTH + 29, 119, 0); // weapon
	if (m_head_armor.m_armor_bitmap)
		al_draw_bitmap(m_head_armor.m_armor_bitmap, WIDTH + (FULL_WIDTH - WIDTH) / 2 - 16, 59, 0); // head
	if (m_chest_armor.m_armor_bitmap)
		al_draw_bitmap(m_chest_armor.m_armor_bitmap, WIDTH + (FULL_WIDTH - WIDTH) / 2 + 4, 119, 0); // chest
	if (m_legs_armor.m_armor_bitmap)
		al_draw_bitmap(m_legs_armor.m_armor_bitmap, WIDTH + (FULL_WIDTH - WIDTH) / 2 + 4, 179, 0); // legs
	if (m_boots_armor.m_armor_bitmap)
		al_draw_bitmap(m_boots_armor.m_armor_bitmap, WIDTH + (FULL_WIDTH - WIDTH) / 2 + 4, 239, 0); // feet
}
void Player::DrawHPHUD()
{
	ALLEGRO_COLOR color;
	double hp_ratio = double(m_hp) / double(m_total_hp);
	if (hp_ratio > 0.8)
		color = al_map_rgb(0, 240, 0);
	else if (hp_ratio > 0.3)
		color = al_map_rgb(255, 255, 0);
	else
		color = al_map_rgb(255, 0, 0);

	if (hp_ratio < 0)
		hp_ratio = 0;

	al_draw_filled_rectangle(35, 20, 140, 30, al_map_rgb(0, 0, 0));
	al_draw_filled_rectangle(35, 20, 35 + hp_ratio * 105, 30, color);

	ALLEGRO_COLOR color2;
	color2 = al_map_rgb(255, 255, 0);

	al_draw_filled_rectangle(35, 40, 140, 50, al_map_rgb(0,0,0));
	al_draw_filled_rectangle(35, 40, 35 + double(experience) / double(expPerLVL()) * 105, 50, color2);

	ALLEGRO_FONT *font = al_load_font("ArchitectHand.ttf", 22, 0);
	al_draw_textf(font, al_map_rgb(255, 255, 255), 130, 55, ALLEGRO_ALIGN_RIGHT, "%d", LVL);

	al_destroy_font(font);
}

int Player::expPerLVL()
{
	return LVL * 30;
}

void Player::DrawStats(ALLEGRO_FONT *ah_font30)
{
	al_draw_textf(ah_font30, al_map_rgb(50, 50, 50), FULL_WIDTH / 2 + 260, HEIGHT / 2 - 180, ALLEGRO_ALIGN_RIGHT, "%d / %d", m_hp, m_total_hp);
	al_draw_textf(ah_font30, al_map_rgb(50, 50, 50), FULL_WIDTH / 2 + 260, HEIGHT / 2 - 140, ALLEGRO_ALIGN_RIGHT, "%d", LVL);
	al_draw_textf(ah_font30, al_map_rgb(50, 50, 50), FULL_WIDTH / 2 + 260, HEIGHT / 2 - 100, ALLEGRO_ALIGN_RIGHT, "%d / %d", experience, expPerLVL());
	al_draw_textf(ah_font30, al_map_rgb(50, 50, 50), FULL_WIDTH / 2 + 20, HEIGHT / 2 - 50, ALLEGRO_ALIGN_RIGHT, "%d", m_strength);
	al_draw_textf(ah_font30, al_map_rgb(50, 50, 50), FULL_WIDTH / 2 + 20, HEIGHT / 2 - 15, ALLEGRO_ALIGN_RIGHT, "%d", m_agility);
	al_draw_textf(ah_font30, al_map_rgb(50, 50, 50), FULL_WIDTH / 2 + 20, HEIGHT / 2 + 20, ALLEGRO_ALIGN_RIGHT, "%d", m_stamina);
	al_draw_textf(ah_font30, al_map_rgb(50, 50, 50), FULL_WIDTH / 2 + 20, HEIGHT / 2 + 55, ALLEGRO_ALIGN_RIGHT, "%d", m_intelligence);
	al_draw_textf(ah_font30, al_map_rgb(50, 50, 50), FULL_WIDTH / 2 + 20, HEIGHT / 2 + 90, ALLEGRO_ALIGN_RIGHT, "%d", m_spirit);
	al_draw_textf(ah_font30, al_map_rgb(50, 50, 50), FULL_WIDTH / 2 + 260, HEIGHT / 2 - 50, ALLEGRO_ALIGN_RIGHT, "%d", atk_stat());
	al_draw_textf(ah_font30, al_map_rgb(50, 50, 50), FULL_WIDTH / 2 + 260, HEIGHT / 2 - 15, ALLEGRO_ALIGN_RIGHT, "%d", crit());
	al_draw_textf(ah_font30, al_map_rgb(50, 50, 50), FULL_WIDTH / 2 + 260, HEIGHT / 2 + 20, ALLEGRO_ALIGN_RIGHT, "%d", def());
	al_draw_textf(ah_font30, al_map_rgb(50, 50, 50), FULL_WIDTH / 2 + 260, HEIGHT / 2 + 55, ALLEGRO_ALIGN_RIGHT, "%.2f", haste());
	al_draw_textf(ah_font30, al_map_rgb(50, 50, 50), FULL_WIDTH / 2 + 260, HEIGHT / 2 + 90, ALLEGRO_ALIGN_RIGHT, "%.1f", base_speed);
}
int Player::CheckForLvlUp()
{
	if (experience >= expPerLVL())
		return 1;
	else
		return 0;
}
int Player::increase_hp()
{
	return m_stamina * 3;
}
void Player::LoadCharacter(char * slot, char * file_content)
{
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
			{
				if (!strcmp(more_inner_node->name(), "class"))
					class_id = atoi(more_inner_node->value());
				if (!strcmp(more_inner_node->name(), "lvl"))
					LVL = atoi(more_inner_node->value());
				if (!strcmp(more_inner_node->name(), "exp"))
					experience = atoi(more_inner_node->value());
				if (!strcmp(more_inner_node->name(), "posX"))
					m_PosX = atoi(more_inner_node->value());
				if (!strcmp(more_inner_node->name(), "posY"))
					m_PosY = atoi(more_inner_node->value());

				for (xml_node <> *much_more_inner_node = more_inner_node->first_node(); much_more_inner_node; much_more_inner_node = much_more_inner_node->next_sibling())
				{
					if (!strcmp(much_more_inner_node->name(), "chest"))
						;			//chest_armor = much_more_inner_node->value();
					if (!strcmp(much_more_inner_node->name(), "str"))
						m_strength = atoi(much_more_inner_node->value());
					if (!strcmp(much_more_inner_node->name(), "agi"))
						m_agility = atoi(much_more_inner_node->value());
					if (!strcmp(much_more_inner_node->name(), "sta"))
						m_stamina = atoi(much_more_inner_node->value());
					if (!strcmp(much_more_inner_node->name(), "int"))
						m_intelligence = atoi(much_more_inner_node->value());
					if (!strcmp(much_more_inner_node->name(), "spt"))
						m_spirit = atoi(much_more_inner_node->value());
				}
			}
	}
	doc.clear();
}

void Player::SaveCharactersData(int slot_number, char * file_content)
{
	char *slot;
	string temp;
	if (slot_number == 1)
		slot = "Slot1";
	else if (slot_number == 2)
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
			{
				if (!strcmp(more_inner_node->name(), "class"))
				{
					temp = to_string(class_id);
					more_inner_node->value(temp.c_str());
				}


				if (!strcmp(more_inner_node->name(), "lvl"))
					LVL = atoi(more_inner_node->value());
				if (!strcmp(more_inner_node->name(), "exp"))
					experience = atoi(more_inner_node->value());
				if (!strcmp(more_inner_node->name(), "posX"))
					m_PosX = atoi(more_inner_node->value());
				if (!strcmp(more_inner_node->name(), "posY"))
					m_PosY = atoi(more_inner_node->value());

				for (xml_node <> *much_more_inner_node = more_inner_node->first_node(); much_more_inner_node; much_more_inner_node = much_more_inner_node->next_sibling())
				{
					if (!strcmp(much_more_inner_node->name(), "chest"))
						;			//chest_armor = much_more_inner_node->value();
					if (!strcmp(much_more_inner_node->name(), "str"))
						m_strength = atoi(much_more_inner_node->value());
					if (!strcmp(much_more_inner_node->name(), "agi"))
						m_agility = atoi(much_more_inner_node->value());
					if (!strcmp(much_more_inner_node->name(), "sta"))
						m_stamina = atoi(much_more_inner_node->value());
					if (!strcmp(much_more_inner_node->name(), "int"))
						m_intelligence = atoi(much_more_inner_node->value());
					if (!strcmp(much_more_inner_node->name(), "spt"))
						m_spirit = atoi(much_more_inner_node->value());
				}
			}
	}
	ofstream file("CharactersData.xml");
	file << "<?xml version=\1.0\" encoding=\"utf-8?>" << endl;
	file << doc;
	doc.clear();
	file.close();
}

Player::~Player()
{
}