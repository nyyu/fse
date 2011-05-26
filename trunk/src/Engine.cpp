#include "Engine.hpp"

Engine* Engine::engine = NULL;

Engine* Engine::getInstance()
{
	if(engine == NULL)
		engine = new Engine;

	return engine;
}

void Engine::kill()
{
	delete engine;
}

Engine::Engine()
{
}

Engine::~Engine()
{
	foreach(Asteroid * asteroid, asteroids)
	{
		delete asteroid;
	}
	asteroids.clear();
}

void Engine::setMoney(qint32 m)
{
	money = m;
}

qint32 Engine::getMoney()
{
	return money;
}

QStringList Engine::getAsteroidsNames()
{
	QStringList names;

	foreach(Asteroid * asteroid, asteroids)
	{
		names << asteroid->getName();
	}

	return names;
}

Asteroid* Engine::getAsteroid(const QString& name)
{
	foreach(Asteroid * asteroid, asteroids)
	{
		if(asteroid->getName() == name)
			return asteroid;
	}
	return NULL;
}

bool Engine::charger(const QString& fileName)
{
	QFile file(fileName);

	if(!file.open(QIODevice::ReadOnly))
		return false;

	QDataStream stream(&file);
	stream.setByteOrder(QDataStream::LittleEndian);

	char tmp[9];
	stream.readRawData(tmp, sizeof tmp);

	if(strcmp(tmp, "SAVEGAME") != 0)
		return false;

	this->~Engine();

	stream.skipRawData(0x9B);
	stream >> money;
	stream.skipRawData(0x35584);

	do
	{
		char name[0xF];
		stream.readRawData(name, sizeof name);

		if(name[0] == 0x50 && name[1] == 0x00 && name[2] == -48) // 0xD0 0xFF
			break;

		stream.skipRawData(0xF5);

		quint16* ores = new quint16[Engine::MAX_ORES];
		stream.readRawData(reinterpret_cast<char*>(ores), Engine::MAX_ORES * sizeof(quint16));

		quint16 maxOres;
		stream >> maxOres;

		stream.skipRawData(0xC2);

		asteroids << new Asteroid(QTextCodec::codecForName("CP850")->toUnicode(name), ores, maxOres);
	}
	while(true);

	file.close();
	return true;
}

bool Engine::sauver(const QString& fileName)
{
	QFile file(fileName);

	if(!file.open(QIODevice::ReadWrite))
		return false;

	QDataStream stream(&file);
	stream.setByteOrder(QDataStream::LittleEndian);

	stream.skipRawData(0xA4);
	stream << money;
	stream.skipRawData(0x35584);

	foreach(Asteroid * asteroid, asteroids)
	{
		char name[0xF] = {0};
		strncpy(name, QTextCodec::codecForName("CP850")->fromUnicode(asteroid->getName()), sizeof name);

		stream.writeRawData(name, 0xF);

		stream.skipRawData(0xF5);

		stream.writeRawData(reinterpret_cast<const char*>(asteroid->getOres()), 10 * sizeof(quint16));

		stream << asteroid->getMaxOres();

		stream.skipRawData(0xC2);
	}

	file.close();
	return true;
}
