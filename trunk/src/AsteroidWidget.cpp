#include "AsteroidWidget.hpp"
#include "Engine.hpp"
#include "Window.hpp"
#include <climits>

AsteroidWidget::AsteroidWidget()
{
	const static QString oresName[] = { "Sélénium", "Astéros", "Barium", "Crystalite", "Quazinc", "Bytanium", "Korellium", "Dragonium", "Traxium", "Nexos" };

	QFormLayout* layout = new QFormLayout;

	name = new QLineEdit;
	name->setMaxLength(0xF);
	connect(name, SIGNAL(textEdited(const QString&)), this, SLOT(textEdited(const QString&)));

	layout->addRow(tr("Name"), name);

	for(int i = 0; i < Engine::MAX_ORES; ++i)
	{
		QSpinBox* tmp = new QSpinBox;
		tmp->setRange(0, USHRT_MAX);
		tmp->setSingleStep(1000);
		ores << tmp;
		layout->addRow(oresName[i], tmp);
		connect(tmp, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
	}

	totalOres = new QLineEdit;
	totalOres->setReadOnly(true);
	layout->addRow("TOTAL", totalOres);

	maxOres = new QSpinBox;
	maxOres->setRange(0, USHRT_MAX);
	maxOres->setSingleStep(1000);
	layout->addRow("MAX", maxOres);
	connect(maxOres, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));

	setLayout(layout);
	setTitle(tr("Asteroid"));
	hide();
}

void AsteroidWidget::setName(const QString& n)
{
	name->setText(n);
	key = n;
}

void AsteroidWidget::setAsteroid(const Asteroid* asteroid)
{
	const quint16* o = asteroid->getOres();

	for(int i = 0; i < Engine::MAX_ORES; ++i)
		ores[i]->setValue(o[i]);

	maxOres->setValue(asteroid->getMaxOres());
}

void AsteroidWidget::valueChanged(int value)
{
	QSpinBox* W = qobject_cast<QSpinBox*>(sender());

	if(W != maxOres)
	{
		int total = 0;

		for(int i = 0; i < Engine::MAX_ORES; ++i)
		{
			if(W == ores.at(i))
				Engine::getInstance()->getAsteroid(key)->setOre(i, value);

			total += ores.at(i)->value();
		}

		totalOres->setText(QString::number(total));
	}
	else
		Engine::getInstance()->getAsteroid(key)->setMaxOres(value);
}

void AsteroidWidget::textEdited(const QString& text)
{
	if(Engine::getInstance()->getAsteroid(text) == NULL)
	{
		Engine::getInstance()->getAsteroid(key)->setName(text);
		key = text;

		Window* window = Window::getInstance();
		window->getStandardModel()->itemFromIndex(window->getTreeView()->selectionModel()->currentIndex())->setText(text);
	}
	else
		name->setText(key);
}

