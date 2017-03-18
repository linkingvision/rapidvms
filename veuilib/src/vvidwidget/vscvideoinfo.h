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
#ifndef __VIDEO_INFO_H
#define __VIDEO_INFO_H

#include <QWidget>

#define NOMINMAX
#define NOMINMAX 
//#include <inttypes.h>


#include "tinythread.h"
#include "fast_mutex.h"
#include "utility.hpp"
#include "ui_vscvideoinfo.h"
//#include "mpipeline.hpp"
using  namespace tthread;


class VSCVideoInfo : public QWidget
{
    Q_OBJECT

public:
    VSCVideoInfo(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~VSCVideoInfo();
	bool event(QEvent *e);
signals:
	void signalFocus();

public:
    Ui::VSCVideoInfo ui;
private:
	bool m_isRunning;
};

#endif // __VIDEO_INFO_H
