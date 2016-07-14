#ifndef VSCCALCULATE_H
#define VSCCALCULATE_H
#include "utility.hpp"
#include "debug.hpp"
#include <QObject>
#include <QLabel>
#include <QTimer>

using namespace UtilityLib;

class cpuOccupy;

class VE_LIBRARY_API VSCCalculate: public QObject
{
	Q_OBJECT
public:
	VSCCalculate(QLabel* label);
	~VSCCalculate();
public slots:
	void showCpuMemoryState();

private:
	QLabel* m_label;
	QTimer* m_Timer;
#ifndef WIN32
	cpuOccupy *m_LastCPU;
#endif
};

#endif