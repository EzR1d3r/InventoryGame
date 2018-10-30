#pragma once
#include <QString>
#include <QVector>
#include <QHash>
#include <QDebug>
#include "ig_consts.h"

struct item_params
{
	uint __idx						= 0;
	QString __name					= "";
	uint __state					= 0;
	uint __max_state				= 0;
	InteractionType __interact_type	= InteractionType::None;
	QHash <uint, QString> __states;
	QHash <uint, QString> __snds;
	bool __mutable					= true;
};

class IG_Item
{
	public:
		IG_Item(){}
		IG_Item (const item_params& params);
		inline uint getIndex() const {return __idx;}
		inline QString getImg (uint state) const {return __states.value( state );}
//		inline void addImg (uint state, QString path) { __states.insert(state, path); }
		inline QString getSnd( uint state ) const {return __snds.value( state );}
		inline uint getState() const {return __state;}
		inline InteractionType getInteractType() const {return __interact_type;}
		void setState(uint state);
		void interactWith (IG_Item &other);

	private:
		uint __idx;
		QString __name;
		uint __state;
		uint __max_state;
		InteractionType __interact_type;
		QHash <uint, QString> __states;
		QHash <uint, QString> __snds;
		bool __mutable = true;

		//
		template <InteractionType type> void interactAs (IG_Item &other);
};
