#ifndef VSCCALCULATE_H
#define VSCCALCULATE_H
#include "utility.hpp"
#include "debug.hpp"
#include <QObject>
#include <QLabel>

using namespace UtilityLib;

class VE_LIBRARY_API VSCCalculate: public QObject
{
	Q_OBJECT
public:
	VSCCalculate(QLabel* label);
	~VSCCalculate()
	{
		delete m_Timer;
	}
public slots:
	void show_cpu_memory_state();

private:
	QLabel* m_label;
	QTimer* m_Timer;
};

#endif