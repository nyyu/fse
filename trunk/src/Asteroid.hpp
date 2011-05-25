#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include <QtGui>

class Asteroid
{
	public:
		Asteroid(QString n, quint16* o, quint16 m);
		~Asteroid();
		QString getName() const;
		const quint16* getOres() const;
		quint16 getMaxOres() const;
		void setName(const QString& value);
		void setOre(int i, quint16 value);
		void setMaxOres(quint16 value);

	private:
		QString name;
		quint16* ores;
		quint16 maxOres;
};

#endif // ASTEROID_HPP
