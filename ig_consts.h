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

inline InteractionType operator|(InteractionType a, InteractionType b)
{return static_cast<InteractionType>(static_cast<int>(a) | static_cast<int>(b));}

inline InteractionType operator&(InteractionType a, InteractionType b)
{return static_cast<InteractionType>(static_cast<int>(a) & static_cast<int>(b));}

//#include "winnt.h"
//DEFINE_ENUM_FLAG_OPERATORS(AnimalFlags)
