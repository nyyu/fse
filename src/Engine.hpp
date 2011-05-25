#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "QtGui"
#include "Asteroid.hpp"

class Engine
{
	public:
		static Engine* getInstance();
		static void kill();
		qint32 getMoney();
		void setMoney(qint32 m);
		QStringList getAsteroidsNames();
		Asteroid* getAsteroid(const QString& name);
		bool charger(const QString& file);
		bool sauver(const QString& fileName);
		enum { MAX_ORES = 10 };

	private:
		Engine();
		~Engine();

		static Engine* engine;
		QList<Asteroid*> asteroids;
		qint32 money;
};

#endif // ENGINE_HPP
