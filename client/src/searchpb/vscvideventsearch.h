#ifndef __VSC_VID_EVENT_SEARCH_H__
#define __VSC_VID_EVENT_SEARCH_H__

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
#include "ui_vscvideventsearch.h"
#include "vvidpb1.h"
#include "common/vscvidtreeinf.h"

using  namespace tthread;

class VSCVidEventSearch : public QWidget
{
    Q_OBJECT

public:
	VSCVidEventSearch(ClientFactory &pFactory, VSCVidTreeInf &pCamTree, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VSCVidEventSearch();
	void EventTableUpdate(VidEvent &cEvent);
	void SetEventUI(VidEvent pEvent);
		
signals:
	void SignalSectionClicked(int row, int column);
public slots:
	void SlotEvent(std::string strEvent);
	void SlotSectionClicked(int row, int column);
	void SlotSearch();
	void SlotStartDateTimeChanged(const QDateTime &dateTime);
	
public:
    	Ui::VSCVidEventSearch ui;
private:
	ClientFactory &m_pFactory;
	VVidPB1 *m_pVideo;
	VSCVidTreeInf &m_pCamTree;
	s64 m_startTime;

	QDateTime m_endMax;
	QDateTime m_endMin;

};


#endif // __VSC_EVENT_CONSOLE_H__
