/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
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
