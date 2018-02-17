#include "Armor.h"


Armor::Armor()
{
}

Armor::Armor(ALLEGRO_BITMAP  *armor_bitmap, int armor_value, int class_ID)
{
	m_armor_bitmap = armor_bitmap;
	m_armor_value = armor_value;
	m_class_ID = class_ID;
}

Armor::~Armor()
{
}
