#include "MoneyWidget.hpp"
#include "Engine.hpp"
#include <climits>

MoneyWidget::MoneyWidget()
{
	money = new QSpinBox;
	money->setRange(0, INT_MAX);
	money->setSingleStep(1000);
	connect(money, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
	QFormLayout* layout = new QFormLayout;
	layout->addRow(tr("Money"), money);
	setLayout(layout);
	hide();
}

int MoneyWidget::getMoney()
{
	return money->value();
}

void MoneyWidget::setMoney(int value)
{
	money->setValue(value);
}

void MoneyWidget::setName(const QString& value)
{
	setTitle(value);
}

void MoneyWidget::valueChanged(int value)
{
	Engine::getInstance()->setMoney(value);
}
