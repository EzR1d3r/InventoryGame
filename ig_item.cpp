#include "ig_item.h"

IG_Item::IG_Item(const item_params &params)
{
	__idx			= params.__idx;
	__name			= params.__name;
	__state			= params.__state;
	__interact_type	= params.__interact_type;
	__states		= params.__states;
	__snd			= params.__snd;

	qDebug() << __interact_type << __name;
}

void IG_Item::interactWith(IG_Item &other)
{
	InteractionType it = __interact_type & other.getInteractType();
	switch (it) {
	case InteractionType::Volumes:
		interactAs<InteractionType::Volumes>(other);
		break;
	default:
		break;
	}
}

template<InteractionType type> void IG_Item::interactAs(IG_Item &other)
{
	//Нет проверки что max state
	if (other.getState())
	{
		++__state;
		other.setState( other.getState() - 1 );
	}
	qDebug() << "thes.state = " << __state << "; other.state =" << other.getState();
}
