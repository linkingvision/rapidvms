#ifndef VSCCALCULATE_H
#define VSCCALCULATE_H
#include "utility.hpp"
#include "debug.hpp"
#include <QObject>
#include <QLabel>
#include <QTimer>

using namespace UtilityLib;

class VE_LIBRARY_API VSCCalculate: public QObject
{
	Q_OBJECT
public:
	VSCCalculate(QLabel* label);
	~VSCCalculate()
	{
		m_Timer->stop();
		delete m_Timer;
	}
public slots:
	void showCpuMemoryState();

private:
	QLabel* m_label;
	QTimer* m_Timer;
};

#endif