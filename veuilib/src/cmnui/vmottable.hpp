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
#ifndef __V_MOT_TABLE__
#define __V_MOT_TABLE__

#include <QWidget>
#define NOMINMAX
#define NOMINMAX 
#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "debug.hpp"
#include <QTimer>
#include <QMenu>
#include <QMimeData>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "recordwrapper.hpp"
#include "vsmotalgo.hpp"
using  namespace tthread;

class VMotTablePri;

class VMotTable : public QWidget
{
	Q_OBJECT

public:
	VMotTable(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VMotTable();
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

public:
	bool GetCurrentMot(VVidMotReg &pMot);
	bool SetCurrentMot(VVidMotReg &pMot);
	/* Set current  */
	bool UpdateMotType(bool type);
	bool SetMotAll();
	bool SetMotClear();
	bool UpdateImage(astring strUrl);


public slots:
	void SlotMotOff();
	void SlotMotOn();
	void SlotNetowkManagerFinished(QNetworkReply *reply);
	
private:
	VMotTablePri * m_pPri;
	bool m_bPressed;
    
};


#endif // __V_MOT_TABLE__
