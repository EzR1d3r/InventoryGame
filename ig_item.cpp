#include "ig_pyextension.h"
#include "ig_utils.h"
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
	IG_PyCaller py_caller;
	py_caller.setPyModule(PY_ITEMS_INTERACTION);
	py_caller.callFunc<IG_Item>(PY_FUNC_INTERACT, this, &other);
}

bool IG_Item::checkStackable(IG_Item &other)
{
	IG_PyCaller py_caller;
	py_caller.setPyModule(PY_ITEMS_INTERACTION);
	return py_caller.callFunc<IG_Item, bool>(PY_FUNC_CHECK_STACKABLE, this, &other);
}
