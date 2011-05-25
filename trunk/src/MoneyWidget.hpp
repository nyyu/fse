#ifndef MONEYWIDGET_HPP
#define MONEYWIDGET_HPP

#include <QtGui>

class MoneyWidget : public QGroupBox
{
	Q_OBJECT

	public:
		MoneyWidget();
		int getMoney();
		void setMoney(int money);
		void setName(const QString& value);

	private slots:
		void valueChanged(int value);

	private:
		QSpinBox* money;

};

#endif // MONEYWIDGET_HPP
