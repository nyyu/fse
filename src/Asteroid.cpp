#include "Asteroid.hpp"

Asteroid::Asteroid(QString n, quint16* o, quint16 m)
{
	name = n;
	ores = o;
	maxOres = m;
}

Asteroid::~Asteroid()
{
	delete[] ores;
}

QString Asteroid::getName() const
{
	return name;
}

const quint16* Asteroid::getOres() const
{
	return ores;
}

quint16 Asteroid::getMaxOres() const
{
	return maxOres;
}

void Asteroid::setName(const QString& value)
{
	name = value;
}

void Asteroid::setOre(int i, quint16 value)
{
	ores[i] = value;
}

void Asteroid::setMaxOres(quint16 value)
{
	maxOres = value;
}
