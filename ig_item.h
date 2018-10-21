#pragma once
#include <QString>
#include <QVector>
#include <QHash>
#include <QDebug>
#include "ig_consts.h"

struct item_params
{
	uint __idx = 0;
	QString __name = "";
	uint __state = 0;
	InteractionType __interact_type = InteractionType::None;
	QHash <int, QString> __states;
	QString __snd = "";
};

class IG_Item
{
	public:
		IG_Item(){}
		IG_Item (const item_params& params);
		inline uint getIndex() const {return __idx;}
		inline QString getImg (uint state) const {return __states.value( state );}
		inline void addImg (uint state, QString path) { __states.insert(state, path); }
		inline QString getSnd() const {return __snd;}
		inline uint getState() const {return __state;}
		inline InteractionType getInteractType() const {return __interact_type;}
		inline void setState(uint state){ __state = state; }
		void interactWith (IG_Item &other);

	private:
		uint __idx;
		QString __name;
		uint __state;
		InteractionType __interact_type;
		QHash <int, QString> __states;
		QString __snd;

		//
		template <InteractionType type> void interactAs (IG_Item &other);
};
