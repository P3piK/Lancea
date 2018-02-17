#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_primitives.h>

class Button
{
	int _textWidth;
	int _textHeight;
	ALLEGRO_FONT *_font;
	char * _text;
public:
	int _left;
	int _right;
	int _upper;
	int _lower;

	void DrawButton();
	void DrawRoundedButton();
	void HighlightButton(bool &render);
	void HighlightRoundedButton(bool &render);
	void HighlightButtonNoFlip();
	void HighlightRoundedButtonNoFlip();

	Button(int left, int right, int upper, int lower, int textWidth, int textHeight, ALLEGRO_FONT *font, char * text);
	~Button();
};

