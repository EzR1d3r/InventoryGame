#pragma once
#include <QString>
#include <QVector>
#include <QHash>
#include <QDebug>
#include "ig_consts.h"

class IG_Item
{
	public:
		IG_Item(){}
		IG_Item (uint idx, QString name, uint state, QHash <int, QString> states):
			__idx(idx), __name(name), __state(state), __states(states) {}
		inline uint getIndex() const {return __idx;}
		inline QString getImg (uint state) const {return __states.value( state );}
		inline QString getSnd() const {return __snd;}
		inline uint getState() const {return __state;}
	private:
		uint __idx;
		QString __name;
		uint __state;
		QHash <int, QString> __states;
		QString __snd;
};
