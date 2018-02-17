#include "Button.h"


Button::Button(int left, int right, int upper, int lower, int textWidth, int textHeight, ALLEGRO_FONT *font, char * text)
{
	_left = left;
	_right = right;
	_upper = upper;
	_lower = lower;
	_textWidth = textWidth;
	_textHeight = textHeight;
	_font = font;
	_text = text;
}

void Button::DrawButton() // int left, int upper, int right, int lower, int textWidth, int textHeight, ALLEGRO_FONT *gd_font, char* text
{
	al_draw_filled_rectangle(_left, _upper, _right, _lower, al_map_rgb(180, 180, 180));
	al_draw_rectangle(_left, _upper, _right, _lower, al_map_rgb(120, 120, 120), 5);
	al_draw_text(_font, al_map_rgb(100, 100, 100), _textWidth, _textHeight, ALLEGRO_ALIGN_CENTER, _text);
}
void Button::DrawRoundedButton() // int left, int upper, int right, int lower, int textWidth, int textHeight, ALLEGRO_FONT *gd_font, char* text
{
	al_draw_filled_rounded_rectangle(_left, _upper, _right, _lower, 15, 15, al_map_rgb(180, 180, 180));
	al_draw_rounded_rectangle(_left, _upper, _right, _lower, 15, 15, al_map_rgb(120, 120, 120), 5);
	al_draw_text(_font, al_map_rgb(100, 100, 100), _textWidth, _textHeight, ALLEGRO_ALIGN_CENTER, _text);
}
void Button::HighlightButton(bool &render)
//int left, int upper, int right, int lower, int textWidth, int textHeight, ALLEGRO_FONT *gd_font, char* text, bool &render)
{
	al_draw_filled_rectangle(_left, _upper, _right, _lower, al_map_rgb(140, 140, 140));
	al_draw_rectangle(_left, _upper, _right, _lower, al_map_rgb(120, 120, 120), 5);
	al_draw_text(_font, al_map_rgb(100, 100, 100), _textWidth, _textHeight, ALLEGRO_ALIGN_CENTER, _text);
	al_flip_display();
	render = false;
}
void Button::HighlightRoundedButton(bool &render)
{
	al_draw_filled_rounded_rectangle(_left, _upper, _right, _lower, 15, 15, al_map_rgb(140, 140, 140));
	al_draw_rounded_rectangle(_left, _upper, _right, _lower, 15, 15, al_map_rgb(120, 120, 120), 5);
	al_draw_text(_font, al_map_rgb(100, 100, 100), _textWidth, _textHeight, ALLEGRO_ALIGN_CENTER, _text);
	al_flip_display();
	render = false;
}
void Button::HighlightButtonNoFlip()
{
	al_draw_filled_rectangle(_left, _upper, _right, _lower, al_map_rgb(140, 140, 140));
	al_draw_rectangle(_left, _upper, _right, _lower, al_map_rgb(120, 120, 120), 5);
	al_draw_text(_font, al_map_rgb(100, 100, 100), _textWidth, _textHeight, ALLEGRO_ALIGN_CENTER, _text);
}
void Button::HighlightRoundedButtonNoFlip()
{
	al_draw_filled_rounded_rectangle(_left, _upper, _right, _lower, 15, 15, al_map_rgb(140, 140, 140));
	al_draw_rounded_rectangle(_left, _upper, _right, _lower, 15, 15, al_map_rgb(120, 120, 120), 5);
	al_draw_text(_font, al_map_rgb(100, 100, 100), _textWidth, _textHeight, ALLEGRO_ALIGN_CENTER, _text);
}
Button::~Button()
{
}
