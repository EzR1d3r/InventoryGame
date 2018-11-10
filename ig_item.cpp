#include "ig_item.h"

IG_Item::IG_Item(const item_params &params)
{
	__idx			= params.__idx;
	__name			= params.__name;
	__state			= params.__state;
	__max_state		= params.__max_state;
	__interact_type	= params.__interact_type;
	__states		= params.__states;
	__snds			= params.__snds;
	__mutable		= params.__mutable;
}

void IG_Item::setState(int state)
{
	if (state > __max_state || !__mutable)
		return;
	else
	__state = state;
}

void IG_Item::interactWith(IG_Item &other)
{
	InteractionType it = __interact_type & other.getInteractType();
	switch (it)
	{
		case InteractionType::Volumes:
			interactAs<InteractionType::Volumes>(other);
			break;
		default:
			break;
	}
}

template<InteractionType type> void IG_Item::interactAs(IG_Item &other)
{
	if (other.getState())
	{
		setState (__state + 1); //через функцию, тк проверка на макс. стейт
		other.setState( other.getState() - 1 );
	}
}
