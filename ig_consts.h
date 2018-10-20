#pragma once

enum class Fruit
{
	None,
	Apple,
	Banana,
	Qiwi
};

enum class NetworkRole
{
	None,
	Server,
	Client
};

enum InteractionType
{
	None    = 0x0,
	Stack   = 0x1,
	Volumes = 0x2,

	Stack_Volumes = Stack | Volumes,
};
