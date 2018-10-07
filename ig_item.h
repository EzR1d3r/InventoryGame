#pragma once
#include <QString>
#include <QVector>
#include <QDebug>
#include "ig_consts.h"

class Item
{
	public:
		Item(): __type(Fruit::None), __img(""), __snd("") {}
		Item (Fruit type, QString img_path,  QString snd_path):
			__type(type), __img(img_path), __snd(snd_path) {}
		inline Fruit getType() const {return __type;}
		inline QString getImg() const {return __img;}
		inline QString getSnd() const {return __snd;}
	private:
		Fruit __type;
		QString __img;
		QString __snd;
};
