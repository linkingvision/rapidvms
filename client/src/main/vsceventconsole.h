#ifndef __VSC_EVENT_CONSOLE_H__
#define __VSC_EVENT_CONSOLE_H__

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QMimeData>
#include <QDrag>

#define NOMINMAX
#define NOMINMAX 

#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "ui_vsceventconsole.h"
#include "vvidpb1.h"

using  namespace tthread;

#define VSC_MAX_EVENT_NUM 1000
typedef std::map<s64, VidEvent> EventMap;


class VidEventTableItem : public QTableWidgetItem
{
public:
	VidEventTableItem(VidEvent &pEvent, const QString & text)
		: m_sEvent(pEvent), QTableWidgetItem(text)
	{
	}
	~VidEventTableItem(){}
public:
	VidEvent GetEvent(){return m_sEvent;}
	void SetEvent(VidEvent &pEvent)
	{
		m_sEvent = pEvent;
	}
private:
	VidEvent m_sEvent;
};

class VSCEventConsole : public QWidget
{
    Q_OBJECT

public:
	VSCEventConsole(ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSCEventConsole();
	void EventTableUpdate(VidEvent &cEvent);
	void SetEventUI(VidEvent pEvent);

signals:
	void SignalSectionClicked(int row, int column);

public slots:
	void SlotEvent(std::string strEvent);
	void SlotSectionClicked(int row, int column);
	void SlotHold();

public:
    	Ui::VSCEventConsole ui;
private:
	ClientFactory &m_pFactory;
	VVidPB1 *m_pVideo;
	EventMap m_EventMap;
	s64 m_EventId;
	BOOL m_bHolded;
	s64 m_nLastUpdateTime;

};


#endif // __VSC_EVENT_CONSOLE_H__
