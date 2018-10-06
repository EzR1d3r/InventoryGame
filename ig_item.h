#pragma once
#include <QString>
#include <QVector>
#include <QDebug>

enum class Fruit
{
	None,
	Apple,
	Banana
};

class Item
{
	public:
		Item(){}
		Item (Fruit type, QString img_path,  QString snd_path):
			m_Type(type), m_Img(img_path), m_Snd(snd_path) {}
		inline Fruit getType() const {return m_Type;}
		inline QString getImg() const {return m_Img;}
	private:
		Fruit m_Type;
		QString m_Img;
		QString m_Snd;
};
