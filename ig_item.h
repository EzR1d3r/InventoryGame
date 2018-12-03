#pragma once
#include <QString>
#include <QVector>
#include <QHash>
#include <QDebug>

struct item_params
{
	uint __idx					= 0;
	QString __name				= "";
	int __state					= 0;
	int __max_state				= 0;
	int __interact_type			= 0;
	QHash <int, QString> __states;
	QHash <int, QString> __snds;
	bool __mutable				= true;
};

class IG_Item
{
	public:
		IG_Item(){}
		IG_Item (const item_params& params);
		inline uint getIndex() const {return __idx;}
		inline QString getImg () const {return __states.value( __state );}
//		inline void addImg (uint state, QString path) { __states.insert(state, path); }
		inline QString getSnd() const {return __snds.value( __state );}
		inline int getState() const {return __state;}
		inline int getInteractType() const {return __interact_type;}
		void setState(int state);
		void interactWith (IG_Item &other);
		bool checkStackable (IG_Item &other);

	private:
		uint __idx;
		QString __name;
		int __state;
		int __max_state;
		int __interact_type;
		QHash <int, QString> __states;
		QHash <int, QString> __snds;
		bool __mutable = true;
};
