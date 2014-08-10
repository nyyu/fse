#ifndef ASTEROIDWIDGET_HPP
#define ASTEROIDWIDGET_HPP

#include <QtWidgets>
#include "Asteroid.hpp"

class AsteroidWidget : public QGroupBox
{
		Q_OBJECT

	public:
		AsteroidWidget();
		void setAsteroid(const Asteroid* asteroid);
		void setName(const QString& name);

	private slots:
		void valueChanged(int value);
		void textEdited(const QString& text);

	private:
		QList<QSpinBox*> ores;
		QSpinBox* maxOres;
		QLineEdit* name;
		QLineEdit* totalOres;
		QString key;
};

#endif // ASTEROIDWIDGET_HPP
