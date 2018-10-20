#include "ig_item.h"

IG_Item::IG_Item(const item_params &params)
{
	__idx			= params.__idx;
	__name			= params.__name;
	__state			= params.__state;
	__interact_type	= params.__interact_type;
	__states		= params.__states;
	__snd			= params.__snd;
}
